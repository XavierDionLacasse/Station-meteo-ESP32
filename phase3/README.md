# Phase 3 — WiFi + InfluxDB + Grafana Cloud

> Statut : ✅ Complétée

## Objectifs

- Connecter l'ESP32 au réseau WiFi avec reconnexion automatique
- Implémenter la fusion de données BME280 + AHT21 via filtre de Kalman
- Envoyer les données vers InfluxDB Cloud avec SSL validé
- Visualiser les données en temps réel sur Grafana Cloud

## Fonctionnalités implémentées

### WiFi

- Connexion au démarrage avec timeout de 10 secondes
- Reconnexion automatique toutes les 30 secondes si déconnecté
- Credentials sécurisés dans `secrets.h` (exclu du dépôt via `.gitignore`)

### Fusion de données — Filtre de Kalman

- Correction de biais AHT21 : offset empirique de 2.0°C (chaleur ENS160)
- Filtre de Kalman simplifié fusionnant BME280 + AHT21

### InfluxDB Cloud

- Envoi toutes les 10 secondes via HTTP line protocol
- SSL validé — certificat ISRG Root X1 (Let's Encrypt) embarqué dans le firmware
- Synchronisation NTP au démarrage pour validation du certificat
- Measurement : `meteo`, tag : `location=salon`
- Fields : `temperature`, `humidite`, `pression`, `eco2`, `tvoc`, `aqi`

### Grafana Cloud

- Dashboard **Station Météo ESP32** avec 6 panels :
  - Température (°C) — valeur fusionnée Kalman
  - Humidité (%) — valeur fusionnée Kalman
  - Pression (hPa)
  - eCO2 (ppm)
  - TVOC (ppb)
  - AQI

## Structure des fichiers



\~\~\~

phase3/

└── station\_meteo/

&#x20;   ├── station\_meteo.ino   — code principal

&#x20;   ├── secrets.h           — credentials (non versionné)

&#x20;   └── secrets.h.example   — template vide (versionné)

\~\~\~

## Décisions techniques

Voir [DECISION.md](../DECISION.md) — sections 9, 10, 11.

## Prochaine étape

[Phase 4](../phase4/) — Batterie + Deep Sleep + LED RGB + Buzzer + Alertes email + Boîtier 3D + PCB