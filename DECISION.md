# Décisions techniques — Station Météo ESP32

Ce document retrace toutes les options envisagées et les décisions
prises pour le choix des capteurs du projet.

---

## 1. Capteur CO2 / Qualité d'air

### Options évaluées

| Capteur | Mesures | Statut | Prix | Décision |
|---------|---------|--------|------|---------|
| CCS811 | eCO2, TVOC | Discontinué | ~33$ | Abandonné |
| ENS160 | eCO2, TVOC, AQI | Actif | Abordable | Choisi |

### Justification

Le CCS811 a été écarté pour trois raisons :
- Prix élevé (~33$) pour un composant discontinué
- Bibliothèque de compensation (Maarten Pennings) non maintenue
- Remplacé officiellement par le ENS160 selon ScioSense

Le ENS160 a été choisi parce qu'il est le successeur direct du CCS811,
moins cher, actif, avec un algorithme TrueVOC intégré sur puce,
une compensation température/humidité native, et une compensation
ozone — technique impossible avec le CCS811.

---

## 2. Capteur température + humidité

### Options évaluées

| Capteur | Précision Temp | Précision Humidité | Interface | Disponibilité | Décision |
|---------|---------------|-------------------|-----------|---------------|---------|
| ENS211 | ±0.15°C | ±2% RH | I2C | Introuvable en module | Abandonné |
| DHT22 | ±0.5°C | ±2% RH | Single-wire | Facile | Écarté |
| AHT21 | ±0.3°C | ±2% RH | I2C | Inclus avec ENS160 | Bonus |
| BME280 | ±1°C | ±3% RH | I2C | Facile | Choisi |

### Justification

**ENS211** — Premier choix en raison de sa compatibilité native avec
le ENS160 (même format de données, même fabricant ScioSense).
Abandonné car introuvable en module breakout sur le marché canadien.
Le chip nu (2x2mm) est impossible à utiliser sur breadboard.

**DHT22** — Écarté malgré sa précision en température (±0.5°C)
supérieure au BME280 pour deux raisons :
- Son interface single-wire est instable et sujette aux erreurs
  de timing, contrairement à I2C
- Il ne mesure pas la pression atmosphérique

**BME280** — Choisi car il remplace à lui seul le ENS211 et un
capteur de pression séparé. Trois mesures en un seul composant
(température, humidité, pression), bibliothèque Adafruit bien
maintenue, et largement disponible.

Note : La précision en température du BME280 (±1°C) est inférieure
au DHT22 (±0.5°C), mais sa résolution (0.01°C vs 0.1°C) et la
fiabilité de son interface I2C compensent largement cet écart pour
notre application.

**AHT21** — Inclus gratuitement dans le module ENS160+AHT21.
Plus précis que le DHT22 en température (±0.3°C vs ±0.5°C) et
équivalent en humidité (±2% RH). Utilisé comme capteur de redondance
pour valider les mesures du BME280 — technique utilisée en
instrumentation professionnelle.

---

## 3. Capteur de pression atmosphérique

### Options évaluées

| Capteur | Précision absolue | Précision relative | Disponibilité | Décision |
|---------|------------------|--------------------|---------------|---------|
| BMP390 | ±0.50 hPa | ±0.03 hPa | Disponible | Remplacé |
| BMP388 | ±0.50 hPa | ±0.03 hPa | Disponible | Remplacé |
| BME280 | ±1 hPa | — | Facile | Inclus |

### Justification

Le BMP390 et BMP388 ont été envisagés pour leur excellente précision.
Ils ont été abandonnés car le BME280 inclut déjà la pression
atmosphérique en plus de la température et de l'humidité — ce qui
simplifie le montage (moins de composants, moins de câblage) tout en
restant suffisamment précis pour une station météo intérieure.

---

## 4. Capteur tout-en-un envisagé

| Capteur | Mesures | Problème | Décision |
|---------|---------|---------|---------|
| BME680 | Temp, humidité, pression, VOC | Ne mesure pas le CO2 directement. Consommation élevée du capteur gaz (9-12mA) incompatible avec objectif d'autonomie batterie | Abandonné |

---

## 5. Configuration finale retenue

| Capteur | Mesures | Rôle |
|---------|---------|------|
| BME280 | Température, humidité, pression | Capteur principal environnemental + compensation ENS160 |
| ENS160 | eCO2, TVOC, AQI | Qualité d'air intérieur |
| AHT21 | Température, humidité | Redondance et validation croisée avec BME280 |

### Avantages de cette configuration

- Redondance de mesure température/humidité (BME280 + AHT21)
- Validation croisée possible entre les deux capteurs
- Compensation environnementale précise du ENS160
- Tous les capteurs sur le même bus I2C (GPIO21/GPIO22)
- Aucun conflit d'adresse I2C
- Bibliothèques bien maintenues pour tous les capteurs
