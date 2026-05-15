# Notes Arduino / C++ — Apprentissage

## Structure de base d'un programme Arduino

```cpp
void setup() {
  // S'exécute UNE SEULE FOIS au démarrage
  // Configuration des pins, initialisation des capteurs
}

void loop() {
  // S'exécute EN BOUCLE INFINIE
  // Logique principale du programme
}
```

## Types de variables en C++

| Type | Description | Exemple |
|------|-------------|---------|
| int | Nombre entier | int compteur = 0; |
| float | Nombre décimal | float temp = 23.5; |
| bool | Vrai ou faux | bool actif = true; |
| String | Texte | String nom = "ESP32"; |
| char | Un caractère | char lettre = 'A'; |

## Variables — Règles importantes

### Variable globale (persiste entre les loops)
```cpp
int compteur = 0;  // Déclarée AVANT setup()

void setup() { }

void loop() {
  compteur++;  // Continue d'augmenter
}
```

### Variable locale (réinitialisée à chaque loop)
```cpp
void loop() {
  int compteur = 0;  // Repart à 0 à chaque tour !
}
```

## Contrôle des GPIO

```cpp
// Configurer une pin
pinMode(GPIO2, OUTPUT);   // Pin en sortie
pinMode(GPIO4, INPUT);    // Pin en entrée

// Contrôler une pin
digitalWrite(GPIO2, HIGH);  // Allumer (3.3V)
digitalWrite(GPIO2, LOW);   // Éteindre (0V)

// Lire une pin
int valeur = digitalRead(GPIO4);
```

## Temporisation

```cpp
delay(1000);        // Pause de 1000ms = 1 seconde
delay(200);         // Pause de 200ms = 0.2 seconde
```

## Boucles

### Boucle for
```cpp
// Répéter 3 fois
for (int i = 0; i < 3; i++) {
  // Code répété
}
```

### Boucle while
```cpp
while (condition) {
  // Répète tant que condition est vraie
}
```

## Conditions

```cpp
if (valeur == "ON") {
  // Si valeur est ON
}
else if (valeur == "OFF") {
  // Sinon si valeur est OFF
}
else {
  // Sinon
}
```

## Communication Série (Serial Monitor)

```cpp
// Initialiser dans setup()
Serial.begin(115200);  // Vitesse 115200 baud

// Envoyer des messages
Serial.print("Texte");     // Sans saut de ligne
Serial.println("Texte");   // Avec saut de ligne

// Afficher une variable
Serial.print("Température: ");
Serial.println(temperature);

// Lire une entrée
if (Serial.available()) {
  String commande = Serial.readStringUntil('\n');
  commande.trim();  // Enlever espaces invisibles
}
```

## Protocole I2C

```cpp
// Pins ESP32 DOIT DevKit V1
// SDA : GPIO21
// SCL : GPIO22

// Initialiser I2C
Wire.begin();

// Adresses I2C de nos capteurs
// BME280  : 0x76 ou 0x77
// ENS160  : 0x52 ou 0x53
// AHT21   : 0x38
// OLED    : 0x3C ou 0x3D
```

## Baud Rate

Le baud rate est la vitesse de communication en bits par seconde.
Serial.begin() et le Serial Monitor doivent avoir
la même vitesse sinon les données sont illisibles.
Valeur standard pour ESP32 : 115200

## Concepts importants

### LED_BUILTIN
Constante qui représente la LED intégrée de la carte.
Sur ESP32 DOIT DevKit V1 : GPIO2

### HIGH / LOW
HIGH = 3.3V = allumé
LOW  = 0V   = éteint

### void
Indique qu'une fonction ne retourne aucune valeur.
Exemple : void setup(), void loop()

### ++ et --
compteur++  → Incrémente de 1 (compteur = compteur + 1)
compteur--  → Décrémente de 1 (compteur = compteur - 1)

## Bonnes pratiques

- Toujours initialiser les variables avec une valeur de départ
- Déclarer les variables globales AVANT setup()
- Utiliser des noms de variables descriptifs
- Commenter le code pour expliquer la logique
- Tester chaque fonctionnalité séparément avant de combiner