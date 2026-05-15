int compteurSeconde = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("ESP32 démarré !");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Seconde: ");
  Serial.println(compteurSeconde);
  delay(1000);
  compteurSeconde++;
}
