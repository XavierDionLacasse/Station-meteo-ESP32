# Phase 1 — Prise en main ESP32

> Statut : ✅ Complétée | Version 1.0

## Objectif
Apprendre les bases de la programmation embarquée avec l'ESP32
sans aucun composant externe — uniquement la carte et le câble USB.

## Matériel
- ESP32 DOIT DevKit V1
- Câble USB micro-B

## Notes importantes
- Le bouton BOOT doit être maintenu lors de l'upload sur certaines
  cartes DOIT DevKit V1 — relâcher quand "Connecting..." apparaît
- Vitesse Serial Monitor : 115200 baud
- LED intégrée : GPIO2 (`LED_BUILTIN` fonctionne directement)

## Programmes

| Programme | Description | Concepts clés |
|-----------|-------------|---------------|
| blink | LED clignote 3x rapidement puis longue pause | `setup()`, `loop()`, `digitalWrite()`, `delay()`, boucle `for` |
| Serial_Monitor | Compteur qui s'incrémente chaque seconde | `Serial.begin()`, `Serial.print()`, variables globales |
| led_control | Contrôle ON/OFF de la LED via Serial Monitor | `Serial.available()`, `Serial.readStringUntil()`, `if/else` |
