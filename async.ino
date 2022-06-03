unsigned long zeitstempel = 0;
int zaehler = 0;

void setup()
{
  Serial.begin(9600);
  zeitstempel = millis();
}

void loop()
{
  if (zeitstempel + 1000 < millis())
  {
    Serial.println(zaehler);
    zaehler += 1;
    zeitstempel = millis();
  }
}
