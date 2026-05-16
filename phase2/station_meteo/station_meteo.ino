#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_BME280 bme;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  // Démarrage du moniteur série pour le débogage
  Serial.begin(115200);
  
  // Initialisation du bus I2C (SDA=GPIO21, SCL=GPIO22)
  Wire.begin(21, 22);
  
  // Vérification de la présence du BME280 (adresse 0x76)
  if (!bme.begin(0x76)) {
    Serial.println("BME280 non trouvé!");
    while (1);
  }
  
  // Vérification de la présence de l'OLED (adresse 0x3C)
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("OLED non trouvé!");
    while (1);
  }
  
  // Effacement de l'écran au démarrage
  display.clearDisplay();
  display.display();
}

void loop() {
  // Lecture des données du BME280
  float temperature = bme.readTemperature();
  float humidite = bme.readHumidity();
  float pression = bme.readPressure() / 100.0;

  // Affichage des données dans le moniteur série
  Serial.print("Température : ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidité : ");
  Serial.print(humidite);
  Serial.println(" %");

  Serial.print("Pression : ");
  Serial.print(pression);
  Serial.println(" hPa");

  // Préparation de l'écran
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Affichage température
  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(temperature);
  display.println(" C");

  // Affichage humidité
  display.setCursor(0, 20);
  display.print("Hum: ");
  display.print(humidite);
  display.println(" %");

  // Affichage pression
  display.setCursor(0, 40);
  display.print("Pres: ");
  display.print(pression);
  display.println(" hPa");

  // Envoi du buffer à l'écran
  display.display();

  // Attente de 2 secondes avant la prochaine lecture
  delay(2000);
}