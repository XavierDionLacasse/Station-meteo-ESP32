# Décisions techniques — Station Météo ESP32

> Version 1.0 — Phase 1

Ce document retrace toutes les options évaluées et les décisions
prises pour le choix des capteurs du projet.

---

## 1. Capteur CO2 / Qualité d'air

### Options évaluées

| Capteur | Mesures | Statut | Décision |
|---------|---------|--------|----------|
| CCS811 | eCO2, TVOC | Discontinué | ❌ Abandonné |
| ENS160 | eCO2, TVOC, AQI | Actif | ✅ Choisi |

### Justification

Le CCS811 a été écarté pour trois raisons :
- Prix élevé (~33$) pour un composant discontinué
- Bibliothèque de compensation non maintenue
- Remplacé officiellement par le ENS160 selon ScioSense

Le ENS160 a été choisi car il est le successeur direct du CCS811,
moins cher, actif, avec un algorithme TrueVOC intégré sur puce,
une compensation température/humidité native, et une compensation
ozone — impossible avec le CCS811.

---

## 2. Capteur température + humidité

### Options évaluées

| Capteur | Précision Temp | Précision Humidité | Interface | Décision |
|---------|---------------|-------------------|-----------|----------|
| ENS211 | ±0.15°C | ±2% RH | I2C | ❌ Introuvable |
| DHT22 | ±0.5°C | ±2% RH | Single-wire | ❌ Écarté |
| AHT21 | ±0.3°C | ±2% RH | I2C | ✅ Bonus inclus |
| BME280 | ±1°C | ±3% RH | I2C | ✅ Choisi |

### Justification

**ENS211** — Premier choix pour sa compatibilité native avec le ENS160
(même fabricant ScioSense). Abandonné car introuvable en module breakout
sur le marché canadien. Le chip nu (2x2mm) est impossible à utiliser
sur breadboard.

**DHT22** — Écarté malgré sa bonne précision en température (±0.5°C) :
- Interface single-wire instable et sujette aux erreurs de timing
- Ne mesure pas la pression atmosphérique

**BME280** — Choisi car il remplace à lui seul le ENS211 et un capteur
de pression séparé. Trois mesures en un seul composant (température,
humidité, pression), bibliothèque Adafruit bien maintenue, largement
disponible. Sa résolution de 0.01°C et la fiabilité de son interface
I2C compensent sa précision légèrement inférieure au DHT22.

**AHT21** — Inclus gratuitement dans le module ENS160+AHT21. Plus précis
que le BME280 en température (±0.3°C vs ±1°C). Utilisé comme capteur
de redondance pour valider les mesures du BME280 — technique standard
en instrumentation professionnelle.

---

## 3. Capteur de pression atmosphérique

### Options évaluées

| Capteur | Précision absolue | Précision relative | Décision |
|---------|------------------|--------------------|----------|
| BMP390 | ±0.50 hPa | ±0.03 hPa | ❌ Remplacé |
| BMP388 | ±0.50 hPa | ±0.03 hPa | ❌ Remplacé |
| BME280 | ±1 hPa | — | ✅ Inclus |

### Justification

Le BMP390 et BMP388 ont été envisagés pour leur excellente précision.
Abandonnés car le BME280 inclut déjà la pression atmosphérique en plus
de la température et de l'humidité — moins de composants, moins de
câblage, précision suffisante pour une station météo intérieure.

---

## 4. Capteur tout-en-un envisagé

| Capteur | Mesures | Décision |
|---------|---------|----------|
| BME680 | Temp, humidité, pression, VOC | ❌ Abandonné |

### Justification

Ne mesure pas le CO2 directement. Sa consommation élevée sur le capteur
gaz (9-12mA) est incompatible avec l'objectif d'autonomie sur batterie.

---

## 5. Configuration finale retenue

| Capteur | Mesures | Rôle |
|---------|---------|------|
| BME280 | Température, humidité, pression | Capteur principal + compensation ENS160 |
| ENS160 | eCO2, TVOC, AQI | Qualité d'air intérieur |
| AHT21 | Température, humidité | Redondance et validation croisée |

### Avantages de cette configuration

- Redondance température/humidité (BME280 + AHT21)
- Validation croisée possible entre les deux capteurs
- Compensation environnementale précise du ENS160
- Tous les capteurs sur le même bus I2C (GPIO21/GPIO22)
- Aucun conflit d'adresse I2C
- Bibliothèques bien maintenues pour tous les capteurs
