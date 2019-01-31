void setup() {
  // put your setup code here, to run once:
Serial.begin(2000000);
}

void loop() {
  // put your main code here, to run repeatedly:
  int x;
  while(Serial.available())
  {
    x = Serial.read();
    Serial.println(x);
  }
  

}
