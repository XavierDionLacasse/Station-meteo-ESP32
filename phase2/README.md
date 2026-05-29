# Phase 2 — Intégration Capteurs + OLED

> Statut : ✅ Complétée | Version 1.1

## Description
Lecture des données environnementales via les capteurs BME280 et ENS160+AHT21,
affichage sur écran OLED SSD1306, communication via protocole I2C.

## Matériel
| Composant | Rôle | Statut |
|-----------|------|--------|
| ESP32 DOIT DevKit V1 | Microcontrôleur principal | ✅ Reçu |
| BME280 | Température, humidité, pression | ✅ Reçu |
| ENS160 + AHT21 (DKARDU) | CO2, TVOC, AQI + redondance | ✅ Reçu |
| OLED 0.96" SSD1306 | Affichage local | ✅ Reçu |

## Schéma de câblage
Schéma réalisé dans Cirkit Designer — photo à ajouter lors de la validation matérielle.

### Connexions I2C
| Composant | SDA | SCL | VCC | GND |
|-----------|-----|-----|-----|-----|
| BME280 | GPIO21 | GPIO22 | 3.3V | GND |
| ENS160 + AHT21 | GPIO21 | GPIO22 | 3.3V | GND |
| OLED SSD1306 | GPIO21 | GPIO22 | 3.3V | GND |

### Adresses I2C
| Composant | Adresse |
|-----------|---------|
| BME280 | 0x76 |
| ENS160 | 0x52 |
| AHT21 | 0x38 |
| OLED SSD1306 | 0x3C |

## Fonctionnalités implémentées
- Barre de progression au démarrage (warm-up ENS160 — 3 minutes)
- 3 pages OLED défilantes automatiquement (4s chacune)
  - Page 1 — BME280 : température, humidité, pression
  - Page 2 — AHT21 : température, humidité + écart vs BME280
  - Page 3 — ENS160 : eCO2, TVOC, AQI
- Affichage Serial Monitor complet pour débogage
- Compensation ENS160 avec valeurs BME280 en temps réel

## Résultats de validation
| Capteur | Adresse détectée | Valeurs réalistes |
|---------|-----------------|-------------------|
| BME280 | ✅ 0x76 | ✅ ~22°C, ~50% RH, ~997 hPa |
| ENS160 | ✅ 0x52 | ✅ Calibration initiale en cours |
| AHT21 | ✅ 0x38 | ✅ ~24°C, ~48% RH |
| OLED | ✅ 0x3C | ✅ Pages défilantes fonctionnelles |

## Notes
- Écart température BME280/AHT21 : ~2°C — normal, l'ENS160 chauffe l'AHT21
- Calibration initiale ENS160 : 24h continues requises une seule fois
- Système fonctionnel en autonomie via USB sans ordinateur

## Bibliothèques
- Adafruit BME280 Library
- Adafruit SSD1306
- Adafruit GFX Library
- DFRobot ENS160
- Adafruit AHTX0
