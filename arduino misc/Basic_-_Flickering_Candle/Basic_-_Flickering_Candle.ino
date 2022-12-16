void setup()
{
  pinMode(2, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(0, OUTPUT);
}

void loop()
{
  analogWrite(2, random(120) + 135);
  analogWrite(1, random(120) + 135);
  analogWrite(0, random(120) + 135);
  delay(random(100));
}
