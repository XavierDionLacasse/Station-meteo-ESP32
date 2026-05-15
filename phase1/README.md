# Phase 1 — Prise en main ESP32

## Objectif
Apprendre les bases de la programmation embarquée avec l'ESP32
sans aucun composant externe — uniquement la carte et le câble USB.

## Matériel utilisé
- ESP32 DOIT DevKit V1
- Câble USB micro-B
- Ordinateur Windows 11

## Observations importantes
- Le bouton BOOT doit être maintenu lors de l'upload sur certaines
  cartes DOIT DevKit V1 — relâcher quand "Connecting..." apparaît
- Vitesse Serial Monitor : 115200 baud obligatoire
- LED intégrée : GPIO2 (LED_BUILTIN fonctionne directement)

## Programmes

### 1. blink
Premier programme uploadé sur l'ESP32.
La LED clignote 3 fois rapidement puis fait une longue pause. /n
Compétences : setup(), loop(), digitalWrite(), delay(), boucle for, logique de timing

### 2. Serial Monitor
Affiche un compteur qui s'incrémente chaque seconde. /n
Compétences : Serial.begin(), Serial.print(), Serial.println(),
variables globales, concaténation en C++

### 3. LED Control
Contrôle de la LED via commandes texte (ON/OFF) depuis
le Serial Monitor. /n
Compétences : Serial.available(), Serial.readStringUntil(),
String.trim(), conditions if/else

## Compétences acquises
- Structure de base Arduino (setup/loop)
- Contrôle des GPIO en sortie
- Communication série bidirectionnelle
- Lecture et traitement de commandes texte
- Débogage via Serial Monitor
- Versioning avec Git et GitHub
