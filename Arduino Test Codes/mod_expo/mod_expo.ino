/**
 * 
 * HOW TO BREAK:
 * 
 * LOOK FOR BLOCK-SPIKE-BLOCK REPRESENTING A 1 WHILE A BLOCK REPRESENTS A 0 
 * 
 * The MSB (last signature) will always be a '1' since the operations stop when exponent becomes 0.
 * 
 * 
 */


long int expo, num, modn, r;

void setup() {
  // put your setup code here, to run once:
  modn = 3233;
  //expo = 0b1011100010;
  num = 123;
  // should get back r = 855
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  //Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  r = 1;
  expo = 0b10111001;
  PORTB &= ~0x10;
  while(expo > 0)
  {
    //PORTB &= ~0x08;
    
    r = (r*r) % modn;

    if((expo % 2) == 1)
    {
      r = (r*num) % modn;
    }

    expo = expo >> 1;
    //PORTB |= 0x08;
  }
  
  PORTB |= 0x10;
}

