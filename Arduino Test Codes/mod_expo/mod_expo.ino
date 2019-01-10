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

////////////////////////////////////////////////////////////////////////////////

void print64(uint64_t num)
{
  char rev[128]; 
  char *p = rev+1;
  while (num)
  {
    *p++ = '0' + num % 10;
    num /= 10;
  }
  p--;
  while(p > rev)
  {
    Serial.print(*p--);
  }
  Serial.println();
}

////////////////////////////////////////////////////////////////////////////////

uint64_t n, p, q, M, e, C, x;

////////////////////////////////////////////////////////////////////////////////

void setup()
{
  p = 23831;
  q = 65657;
  n = p * q;
  M = 65;
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);




  
//  e = 4428079;
//  modn = 3233;
//  //expo = 0b1011100010;
//  num = 123;
//  // should get back r = 855
  Serial.begin(9600);
}

////////////////////////////////////////////////////////////////////////////////

void loop()
{
  e = 3427;
  C = 1;
  x = M;
  while(e > 0)
  {
    
    if(e % 2)
    {
      C = (C * x) % n;
    }
    print64(C);
    e >>= 1;
    x = (x * x) % n;
  }
  print64(C);
  delay(5000);

//  r = 1;
//  expo = 0b10111001;
//  PORTB &= ~0x10;
//  while(expo > 0)
//  {
//    //PORTB &= ~0x08;
//    
//    r = (r*r) % modn;
//
//    if((expo % 2) == 1)
//    {
//      r = (r*num) % modn;
//    }
//
//    expo = expo >> 1;
//    //PORTB |= 0x08;
//  }
//  
//  PORTB |= 0x10;
}
