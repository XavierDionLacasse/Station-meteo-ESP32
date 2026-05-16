# Station Météo ESP32 — Monitoring Environnemental Connecté

## Description

Système d'instrumentation multi-capteurs embarqué sur ESP32 avec
acquisition de données en temps réel, transmission WiFi et
visualisation sur dashboard cloud (Grafana/InfluxDB).

## Objectifs

* Apprendre l'électronique embarquée et le protocole I2C
* Acquérir des données environnementales en temps réel
* Visualiser les données sur un dashboard connecté

## Matériel utilisé
| Matériel | Description | Prix lors de l'achat |
|----------|-------------|----------------------|
| Système sur puce | ESP32 DOIT DevKit V1 | 24$ |
| Capteur de temperature, humidité et de pression | Capteur BME280 | 13$ |
| Capteur CO2, TVOC et AQI | ENS160 | 8$ |
| Capteur température et humidité | AHT21 | 8$ |
| Écran | Écran OLED 0.96" SSD1306 | 10$ |

## Progression

* Phase 1 — Prise en main (Terminé)
* Phase 2 — Intégration capteurs + OLED (En cours)
* Phase 3 — WiFi + InfluxDB + Grafana Cloud
* Phase 4 — Batterie + Deep Sleep + Alertes + Boîtier

## Environnement de développement

### Logiciels installés

* Arduino IDE 2.x
* Git 2.47
* Pilote CP2102

### Bibliothèques Arduino

* Adafruit BME280 Library
* Adafruit SSD1306
* Adafruit GFX Library
* DFRobot ENS160
* Adafruit AHTX0

### Services Cloud

* InfluxDB Cloud (base de données time-series)
* Grafana Cloud (dashboard de visualisation)

## Compétences développées

* Programmation C/C++ embarqué
* Protocole I2C
* IoT et communication WiFi
* Visualisation de données
* Fusion et redondance de capteurs

