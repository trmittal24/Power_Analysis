void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
    Serial.println("Starting-------------------------------------------------------------!");

  digitalWrite(13,0);
}
int x=0;
void loop() {
  // put your main code here, to run repeatedly:

  while(!x)
  {
    Serial.println("Glitch me if you can!");
    Serial.println("HAHAHAHAHA");
    Serial.println("You can't");

    if(x == 1)
    break;

    
    if(x == 2)
    break;

    
    if(x != 0)
    break;

    if(x) break;

    if(x) break;

    if(x) break;

    if(!!x) break;

    

    //delay(00);
  }
//  digitalWrite(13,1);
  while(1)
  {
    while(1)
    {
      Serial.println(" You did it :) ");



      Serial.println("Hurray!");

      
    }
  }
  
}
