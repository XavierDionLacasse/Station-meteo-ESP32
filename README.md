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
| BME280 | Température, humidité, pression | 🚚 En transit |
| ENS160 + AHT21 (DKARDU) | CO2, TVOC, AQI + redondance | 🚚 En transit |
| OLED 0.96" SSD1306 | Affichage local | 🚚 En transit |

## Phases du projet
| Phase | Description | Statut |
|-------|-------------|--------|
| Phase 1 | Prise en main ESP32 | ✅ Complétée |
| Phase 2 | Intégration capteurs + OLED | 🔄 En cours |
| Phase 3 | WiFi + InfluxDB + Grafana Cloud | 🔒 À venir |
| Phase 4 | Batterie + Deep Sleep + Alertes + Boîtier | 🔒 À venir |

## Environnement de développement
**Logiciels :** Arduino IDE 2.x · Git 2.47 · Pilote CP2102

**Bibliothèques Arduino :** Adafruit BME280 · Adafruit SSD1306 · Adafruit GFX · DFRobot ENS160 · Adafruit AHTX0

**Services Cloud :** InfluxDB Cloud (time-series) · Grafana Cloud (dashboard)

## Documentation
- [Décisions techniques](DECISION.md) — choix des capteurs et justifications
