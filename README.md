# Station Météo ESP32 — Monitoring Environnemental Connecté

## Description
Station météo intérieure connectée basée sur ESP32 — acquisition 
multi-capteurs (température, humidité, pression, CO2, qualité d'air) 
avec affichage OLED local, transmission WiFi et dashboard cloud 
en temps réel via InfluxDB et Grafana.

## Objectifs
- Concevoir un système embarqué de monitoring environnemental
- Implémenter une architecture IoT complète de bout en bout
- Appliquer des techniques de fusion et redondance de capteurs

## Matériel
| Composant | Rôle | Statut |
|-----------|------|--------|
| ESP32 DOIT DevKit V1 | Microcontrôleur principal | ✅ Reçu |
| BME280 | Température, humidité, pression | ✅ Reçu |
| ENS160 + AHT21 (DKARDU) | CO2, TVOC, AQI + redondance | ✅ Reçu |
| OLED 0.96" SSD1306 | Affichage local | 🚚 En transit |

## Phases du projet
| Phase | Description | Statut |
|-------|-------------|--------|
| Phase 1 | Prise en main ESP32 | ✅ Complétée |
| Phase 2 | Intégration capteurs + OLED | 🔄 En cours |
| Phase 3 | WiFi + InfluxDB + Grafana Cloud | 🔒 À venir |
| Phase 4 | Batterie + Deep Sleep + Alertes + Boîtier | 🔒 À venir |

## Progression détaillée

### ✅ Phase 1 — Prise en main ESP32
- Clignotement LED intégrée (`blink`)
- Communication Serial Monitor (`Serial_Monitor`, `led_control`)
- Git + GitHub configurés avec SSH
- Release v1.0 publiée

### 🔄 Phase 2 — Intégration capteurs + OLED
- Schéma de câblage réalisé dans Cirkit Designer
- Code complet écrit et pushé (`station_meteo.ino`)
  - BME280 : température, humidité, pression
  - AHT21 : température, humidité (redondance)
  - ENS160 : eCO2, TVOC, AQI avec compensation BME280
  - OLED : 3 pages défilantes + barre de warm-up
- BME280 et ENS160+AHT21 reçus — câblage et tests à venir
- OLED en transit

### 🔒 Phase 3 — WiFi + InfluxDB + Grafana Cloud
- Services cloud configurés (InfluxDB Cloud + Grafana Cloud)
- Développement à venir

### 🔒 Phase 4 — Batterie + Deep Sleep + Alertes + Boîtier
- Deep Sleep 15-20 minutes (autonomie ~2 jours théoriques)
- LED RGB + Buzzer + Alertes email
- Boîtier 3D + PCB personnalisé

## Structure du dépôt

    Station-meteo-ESP32/
    ├── README.md
    ├── DECISION.md
    ├── phase1/
    │   ├── README.md
    │   ├── blink/
    │   ├── Serial_Monitor/
    │   └── led_control/
    └── phase2/
        ├── README.md
        └── station_meteo/
            └── station_meteo.ino

## Environnement de développement
**Logiciels :** Arduino IDE 2.x · Git 2.47 · Pilote CP2102  
**Bibliothèques Arduino :** Adafruit BME280 · Adafruit SSD1306 · Adafruit GFX · DFRobot ENS160 · Adafruit AHTX0  
**Services Cloud :** InfluxDB Cloud (time-series) · Grafana Cloud (dashboard)

## Documentation
- [Décisions techniques](DECISION.md) — choix des capteurs et justifications
