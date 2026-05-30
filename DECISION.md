# Décisions techniques — Station Météo ESP32

> Version 1.3 — Phase 3

Ce document retrace toutes les options évaluées et les décisions
prises pour le choix des capteurs et l'architecture du projet.

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
disponible.

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

---

## 6. Intervalle Deep Sleep et autonomie

### Décision

Intervalle Deep Sleep retenu : **15-20 minutes**

### Justification

L'ENS160 requiert un warm-up de 3 minutes après chaque réveil avant
de produire des mesures fiables. Avec un intervalle de 5 minutes,
60% du temps actif serait consacré au warm-up.

La qualité de l'air intérieure varie lentement — une mesure toutes
les 15-20 minutes capture bien les tendances sans perte d'information
significative.

### Consommation théorique par cycle (15 minutes, batterie 18650 2500mAh)

| Phase | Durée | Courant | Consommation |
|-------|-------|---------|-------------|
| Démarrage + BME280 + AHT21 + OLED | 5s | ~181 mA | 0.251 mAh |
| Warm-up ENS160 + OLED | 180s | ~209 mA | 10.45 mAh |
| Lecture ENS160 + WiFi + OLED | 10s | ~290 mA | 0.806 mAh |
| Deep Sleep | 705s | ~0.02 mA | 0.004 mAh |
| **Total par cycle** | | | **~11.5 mAh** |

Autonomie théorique : **~2.25 jours** (~1.5-2 jours en conditions réelles)

### Note importante

Ces chiffres sont théoriques. La consommation réelle sera mesurée
avec le multimètre KAIWEETS HT118A en Phase 4.

---

## 7. Alimentation Phase 4 — Décision en attente

Options à évaluer en Phase 4 après mesure de la consommation réelle :

- Augmenter l'intervalle Deep Sleep (60 min+)
- Plusieurs batteries 18650 en parallèle
- Panneau solaire (si près d'une fenêtre)
- Sacrifier la précision ENS160 (pas de warm-up → ~23 jours)

**Décision reportée à la Phase 4.**

---

## 8. Affichage OLED — Stratégie par phase

### Décision

| Phase | Stratégie | Justification |
|-------|-----------|---------------|
| Phase 2-3 | 3 pages défilantes (4s chacune) | Validation de tous les capteurs |
| Phase finale | Page unique condensée | Expérience utilisateur épurée |

### Pages Phase 2-3

- **Page 1 — BME280 :** Température, humidité, pression
- **Page 2 — AHT21 :** Température, humidité + écart vs BME280 (dT, dH)
- **Page 3 — ENS160 :** eCO2, TVOC, AQI avec libellé textuel

---

## 9. Fusion de données — BME280 et AHT21

### Décision

Méthode retenue : **Filtre de Kalman simplifié** avec détection d'anomalie

### Justification

| Méthode | Complexité | Précision | Décision |
|---------|------------|-----------|----------|
| Moyenne simple | Faible | Moyenne | ❌ Écarté |
| Moyenne pondérée | Faible | Bonne | ❌ Écarté |
| Filtre de Kalman | Moyenne | Optimale | ✅ Choisi |

Le filtre de Kalman est la méthode standard en instrumentation
professionnelle. Il combine les mesures en tenant compte de
l'incertitude de chaque capteur, produisant une estimation optimale.

### Paramètres retenus

| Paramètre | Valeur | Signification |
|-----------|--------|---------------|
| Q | 0.01 | Bruit de processus faible — valeurs stables |
| R | 0.5 | Bruit de mesure — confiance modérée |
| Seuil anomalie temp | 1.0°C | Alerte si écart BME280/AHT21 > 1°C |
| Seuil anomalie humi | 5.0% | Alerte si écart BME280/AHT21 > 5% |

### Correction de biais AHT21

L'AHT21 affiche systématiquement +2°C par rapport au BME280 en raison
de la chaleur dégagée par l'ENS160 physiquement adjacent. Un offset
empirique de **2.0°C** est soustrait avant le filtre de Kalman.

Cette séparation est intentionnelle : le biais est une erreur
systématique (toujours même direction, même amplitude) — le filtre
de Kalman gère le bruit aléatoire résiduel.

---

## 10. Architecture cloud — Phase 3

### Décision

| Service | Rôle | Décision |
|---------|------|----------|
| InfluxDB Cloud | Stockage time-series | ✅ Choisi |
| Grafana Cloud | Visualisation + alertes | ✅ Choisi |

### Justification

**InfluxDB** — Base de données time-series optimisée pour les données
de capteurs. Le line protocol HTTP permet un envoi simple depuis
l'ESP32 sans bibliothèque dédiée.

**Grafana** — Standard industrie pour la visualisation de métriques.
Compatible nativement avec InfluxDB via requêtes Flux.

### Paramètres retenus

| Paramètre | Valeur | Justification |
|-----------|--------|---------------|
| Intervalle envoi | 10 secondes | Granularité fine pour Phase 3 |
| Protocole | HTTP line protocol | Simple, pas de bibliothèque externe |
| SSL | `setInsecure()` temporaire | **TODO Phase 4 : valider certificat** |

---

## 11. Sécurité credentials — Phase 3

### Décision

Credentials WiFi et InfluxDB stockés dans `secrets.h` exclu du dépôt
via `.gitignore`. Un fichier `secrets.h.example` est fourni comme
template dans le dépôt.

### Justification

Pratique standard dans l'industrie pour éviter l'exposition de
credentials sensibles dans un dépôt public.
