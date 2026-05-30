#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_AHTX0.h>
#include <DFRobot_ENS160.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include "secrets.h"
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

// ─── WiFi ─────────────────────────────────────────────────────────
#define WIFI_TIMEOUT_MS     10000
#define WIFI_RETRY_INTERVAL 30000
unsigned long lastWifiRetry = 0;

// ─── InfluxDB ─────────────────────────────────────────────────────
#define INFLUXDB_INTERVAL 10000
unsigned long dernierEnvoi = 0;

// ─── Configuration OLED ───────────────────────────────────────────
#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64
#define OLED_RESET      -1
#define SCREEN_ADDRESS  0x3C

// ─── Configuration capteurs ───────────────────────────────────────
#define BME280_ADDRESS  0x76
#define ENS160_ADDRESS  0x52

// ─── Constantes de temps ──────────────────────────────────────────
#define WARMUP_DURATION_MS  180000
#define LECTURE_INTERVALLE  2000
#define PAGE_INTERVALLE     4000

// ─── Correction de biais AHT21 ────────────────────────────────────
#define AHT21_TEMP_OFFSET 2.0f  // °C — biais empirique mesuré

// ─── Filtre de Kalman ─────────────────────────────────────────────
struct KalmanFilter {
  float x;  // estimation courante
  float P;  // incertitude courante
  float Q;  // bruit du processus
  float R;  // bruit de mesure
};

// ─── Objets capteurs ──────────────────────────────────────────────
Adafruit_BME280    bme;
Adafruit_AHTX0     aht;
DFRobot_ENS160_I2C ens160(&Wire, ENS160_ADDRESS);
Adafruit_SSD1306   display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ─── Variables globales ───────────────────────────────────────────
float    bme_temp, bme_hum, bme_pression;
float    aht_temp, aht_hum;
uint16_t ens_eco2, ens_tvoc;
uint8_t  ens_aqi;
bool     ens_pret = false;

float fusionTemp = 0.0f;
float fusionHumi = 0.0f;

unsigned long derniere_lecture = 0;
unsigned long derniere_page    = 0;
uint8_t       page_actuelle    = 0;

// Q faible = on fait confiance à la continuité
// R BME280 < R AHT21 = BME280 plus fiable
KalmanFilter kTemp = {0.0f, 1.0f, 0.01f, 0.5f};
KalmanFilter kHumi = {0.0f, 1.0f, 0.01f, 0.5f};

WiFiClientSecure secureClient;

// ═════════════════════════════════════════════════════════════════
// FONCTIONS UTILITAIRES
// ═════════════════════════════════════════════════════════════════

void erreur_fatale(const char* message) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.println("ERREUR:");
  display.println(message);
  display.display();
  while (1);
}

void afficher_warmup() {
  unsigned long debut = millis();

  while (true) {
    unsigned long elapsed = millis() - debut;
    if (elapsed >= WARMUP_DURATION_MS) break;

    uint8_t progression = (uint8_t)((elapsed * 100UL) / WARMUP_DURATION_MS);
    uint8_t barre_px    = progression;  // 0-100 pixels

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(20, 5);
    display.println("Station Meteo");

    display.setCursor(10, 22);
    display.println("Initialisation...");

    display.drawRect(14, 38, 100, 12, SSD1306_WHITE);
    if (barre_px > 0) {
      display.fillRect(14, 38, barre_px, 12, SSD1306_WHITE);
    }

    display.setCursor(50, 55);
    display.print(progression);
    display.println("%");

    display.display();
    delay(500);
  }
}

// ═════════════════════════════════════════════════════════════════
// LECTURE DES CAPTEURS
// ═════════════════════════════════════════════════════════════════

void lire_bme280() {
  bme_temp     = bme.readTemperature();
  bme_hum      = bme.readHumidity();
  bme_pression = bme.readPressure() / 100.0f;
}

void lire_aht21() {
  sensors_event_t hum_event, temp_event;
  aht.getEvent(&hum_event, &temp_event);
  aht_temp = temp_event.temperature;
  aht_hum  = hum_event.relative_humidity;
}

void lire_ens160() {
  ens160.setTempAndHum(bme_temp, bme_hum);

  if (ens160.getENS160Status() == 0) {
    ens_pret = true;
    ens_eco2 = ens160.getECO2();
    ens_tvoc = ens160.getTVOC();
    ens_aqi  = ens160.getAQI();
  } else {
    ens_pret = false;
  }
}

// ═════════════════════════════════════════════════════════════════
// FUSION KALMAN
// ═════════════════════════════════════════════════════════════════

float kalmanUpdate(KalmanFilter &kf, float mesure) {
  kf.P = kf.P + kf.Q;
  float K = kf.P / (kf.P + kf.R);
  kf.x = kf.x + K * (mesure - kf.x);
  kf.P = (1.0f - K) * kf.P;
  return kf.x;
}

void fusionCapteurs(float tempBME, float humisBME, float tempAHT, float humiAHT) {
  float tempAHTCorr = tempAHT - AHT21_TEMP_OFFSET;
  float tempMoyenne = (tempBME + tempAHTCorr) / 2.0f;
  float humiMoyenne = (humisBME + humiAHT) / 2.0f;
  fusionTemp = kalmanUpdate(kTemp, tempMoyenne);
  fusionHumi = kalmanUpdate(kHumi, humiMoyenne);
}

// ═════════════════════════════════════════════════════════════════
// AFFICHAGE OLED — 3 PAGES DÉFILANTES
// ═════════════════════════════════════════════════════════════════

void afficher_page_bme280() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(35, 0);  display.println("[ BME280 ]");
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
  display.setCursor(0, 15);  display.print("Temp : "); display.print(bme_temp, 1);     display.println(" C");
  display.setCursor(0, 30);  display.print("Hum  : "); display.print(bme_hum, 1);      display.println(" %");
  display.setCursor(0, 45);  display.print("Pres : "); display.print(bme_pression, 1); display.println(" hPa");
  display.setCursor(110, 56); display.println("1/3");
  display.display();
}

void afficher_page_aht21() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(35, 0);  display.println("[ AHT21 ]");
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
  display.setCursor(0, 15);  display.print("Temp : "); display.print(aht_temp, 1); display.println(" C");
  display.setCursor(0, 30);  display.print("Hum  : "); display.print(aht_hum, 1);  display.println(" %");
  display.setCursor(0, 48);
  display.print("dT:"); display.print(aht_temp - bme_temp, 2);
  display.print("C  dH:"); display.print(aht_hum - bme_hum, 1); display.println("%");
  display.setCursor(110, 56); display.println("2/3");
  display.display();
}

void afficher_page_ens160() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(35, 0);  display.println("[ ENS160 ]");
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

  if (ens_pret) {
    display.setCursor(0, 15); display.print("eCO2 : "); display.print(ens_eco2); display.println(" ppm");
    display.setCursor(0, 30); display.print("TVOC : "); display.print(ens_tvoc); display.println(" ppb");
    display.setCursor(0, 45); display.print("AQI  : "); display.print(ens_aqi);
    switch (ens_aqi) {
      case 1: display.println(" Excellent"); break;
      case 2: display.println(" Bon");       break;
      case 3: display.println(" Modere");    break;
      case 4: display.println(" Pauvre");    break;
      case 5: display.println(" Mauvais");   break;
    }
  } else {
    display.setCursor(10, 25); display.println("Calibration");
    display.setCursor(20, 40); display.println("en cours...");
  }

  display.setCursor(110, 56); display.println("3/3");
  display.display();
}

void gerer_pages_oled() {
  unsigned long maintenant = millis();
  if (maintenant - derniere_page >= PAGE_INTERVALLE) {
    derniere_page = maintenant;
    page_actuelle = (page_actuelle + 1) % 3;
  }
  switch (page_actuelle) {
    case 0: afficher_page_bme280(); break;
    case 1: afficher_page_aht21();  break;
    case 2: afficher_page_ens160(); break;
  }
}

// ═════════════════════════════════════════════════════════════════
// WIFI
// ═════════════════════════════════════════════════════════════════

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - start >= WIFI_TIMEOUT_MS) return;
    delay(500);
  }
}

void checkWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;
  if (millis() - lastWifiRetry < WIFI_RETRY_INTERVAL) return;
  lastWifiRetry = millis();
  WiFi.disconnect();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

// ═════════════════════════════════════════════════════════════════
// INFLUXDB
// ═════════════════════════════════════════════════════════════════

void envoyerInfluxDB() {
  if (WiFi.status() != WL_CONNECTED) return;

  String lineProtocol = "meteo,location=salon ";
  lineProtocol += "temperature=" + String(fusionTemp, 2) + ",";
  lineProtocol += "humidite="    + String(fusionHumi, 2) + ",";
  lineProtocol += "pression="    + String(bme_pression, 2);

  if (ens_pret) {
    lineProtocol += ",eco2=" + String(ens_eco2);
    lineProtocol += ",tvoc=" + String(ens_tvoc);
    lineProtocol += ",aqi="  + String(ens_aqi);
  }

  HTTPClient http;
  String url = String(INFLUXDB_URL) + "/api/v2/write?org=" + INFLUXDB_ORG
             + "&bucket=" + INFLUXDB_BUCKET + "&precision=s";

  http.begin(secureClient, url);
  http.addHeader("Authorization", "Token " + String(INFLUXDB_TOKEN));
  http.addHeader("Content-Type", "text/plain; charset=utf-8");
  http.POST(lineProtocol);
  http.end();
}

// ═════════════════════════════════════════════════════════════════
// SETUP
// ═════════════════════════════════════════════════════════════════

void setup() {
  Wire.begin(21, 22);
  secureClient.setCACert(ISRG_ROOT_X1);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) while (1);
  display.clearDisplay();
  display.display();

  if (!bme.begin(BME280_ADDRESS))      erreur_fatale("BME280 introuvable");
  if (!aht.begin())                    erreur_fatale("AHT21 introuvable");
  if (ens160.begin() != NO_ERR)        erreur_fatale("ENS160 introuvable");
  ens160.setPWRMode(ENS160_STANDARD_MODE);

  connectWiFi();

  // Synchronisation NTP — obligatoire pour valider le certificat SSL
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  struct tm timeinfo;
  int retry = 0;
  while (!getLocalTime(&timeinfo) && retry < 10) { delay(1000); retry++; }

  afficher_warmup();
  display.clearDisplay();
  display.display();
}

// ═════════════════════════════════════════════════════════════════
// LOOP
// ═════════════════════════════════════════════════════════════════

void loop() {
  unsigned long maintenant = millis();

  checkWiFi();

  if (maintenant - derniere_lecture >= LECTURE_INTERVALLE) {
    derniere_lecture = maintenant;
    lire_bme280();
    lire_aht21();
    lire_ens160();
    fusionCapteurs(bme_temp, bme_hum, aht_temp, aht_hum);
  }

  if (maintenant - dernierEnvoi >= INFLUXDB_INTERVAL) {
    dernierEnvoi = maintenant;
    envoyerInfluxDB();
  }

  gerer_pages_oled();
}