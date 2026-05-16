# Phase 2 — Intégration Capteurs + OLED



## Description
Lecture des données environnementales via les capteurs BME280 et ENS160+AHT21, affichage sur écran OLED SSD1306, communication via protocole I2C.



## Matériel
- ESP32 DOIT DevKit V1
- BME280 (température, humidité, pression)
- ENS160 + AHT21 (eCO2, TVOC, AQI + température/humidité redondante)
- Écran OLED 0.96" SSD1306

## Schéma de câblage
Schéma réalisé dans Cirkit Designer.

### Connexions I2C
| Composant      | SDA    | SCL    | VCC  | GND  |

|----------------|--------|--------|------|------|

| BME280         | GPIO21 | GPIO22 | 3.3V | GND  |

| ENS160 + AHT21 | GPIO21 | GPIO22 | 3.3V | GND  |

| OLED SSD1306   | GPIO21 | GPIO22 | 3.3V | GND  |


### Adresses I2C
| Composant    | Adresse |

|--------------|---------|

| BME280       | 0x76    |

| ENS160       | 0x52    |

| AHT21        | 0x38    |

| OLED SSD1306 | 0x3C    |



## Bibliothèques requises
- Adafruit BME280 Library
- Adafruit SSD1306
- Adafruit GFX Library
- DFRobot ENS160
- Adafruit AHTX0

