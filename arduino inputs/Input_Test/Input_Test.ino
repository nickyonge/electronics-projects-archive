byte inputPin = 0;
byte outputPin = 3;
void setup() {
  pinMode(inputPin,INPUT);
  pinMode(outputPin,OUTPUT);
}
void loop() {
  
  digitalWrite(outputPin, digitalRead(inputPin) ? HIGH : LOW);
  
}
