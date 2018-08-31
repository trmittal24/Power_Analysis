 /* //-----------------------------------------------------------------
Side Channel Attack using Simple Power analysis

Authors: Santosh, Tarun, Vishal
Guide: Dr. Arulalan Rajan

Details:
This is a test code to verify if simple power analysis works on the target (i.e Arduino)
A 8 bit key is initialized.
Then the code goes through each bit of key starting from LSB and performs an operation (addition or multiplication) according to the key bit.

 Initially we used delays between each instructions to be able to identify them distinctly.
 Later on, it was removed and power analysis still worked.

//-----------------------------------------------------------------
*/
//thamba is an inline function used to create a delay not consuming any power
//This is done using by just writing lots of "nop"s (No Operation) as using a timer would consume power.

inline void thamba()
{
  //asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );asm ( "nop \n" );    
}

//inline void thamba()
//{
//  int count;
//  
//  for(count = 0 ; count < NUM ; count++)
//  {
//    asm( "nop \n" );
//  }
//}

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  // disable ADC
  ADCSRA = 0; 
  PRR = 0; // disable peripherals: timers, etc.

  // pervent power spikes at every 1 ms intervals by switching off millis counter
  SREG &= ~0x80 ;

  //Serial.begin(115200);
}

uint8_t i=1;
uint8_t j=99;
uint8_t a = 10;
uint8_t c;
uint8_t d,e,f;
uint8_t k=11;
uint8_t x=25;

void loop() {
  // put your main code here, to run repeatedly:
  d = 10;
  while(1)
  {
 
   
    x = 0xec;   //the key

     //toggling an output pin to use as a reference while observing
    PORTB ^= 0x10 ;  // same as digitalWRITE(12, ...) BUT LOWER POWER USAGE
    
   for(k=0; k<8;k++)
   {

    //toggling an output pin to use as a reference while observing
    PORTB ^= 0x08 ;  // same as digitalWRITE(11, ...) BUT LOWER POWER USAGE
    if(x % 2 == 0 )  //check if LSB is 1
    {
      thamba();     //introducing delay
      c = d + 1;    // add (less expensive operation)
      thamba();
    }
    else
    {
      thamba();
      c = d * j;  // multiply (more expensive operation)
      thamba();
    }
    thamba();
    UDR0 = c;   // sendinf value C through UART port
    thamba();
    x = x >> 1; //rotate x
    thamba();
   }
   //digitalWrite(12,0);  
  }

  
}
