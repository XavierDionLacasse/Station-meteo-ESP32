void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("Tape ON ou OFF pour contrôler la LED");
}

void loop() {
  if (Serial.available()) {
    String commande = Serial.readStringUntil('\n');
    commande.trim();

    if (commande == "ON") {
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("LED allumée !");
    } 
    else if (commande == "OFF") {
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println("LED éteinte !");
    } 
    else {
      Serial.println("Commande inconnue. Tape ON ou OFF");
    }
  }
}
