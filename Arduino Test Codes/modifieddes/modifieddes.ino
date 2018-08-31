void setup() {
  // put your setup code here, to run once:
   Serial.begin(115200);
   pinMode(11, OUTPUT);
   pinMode(12, OUTPUT);

   SREG &= ~0x80 ; // pervent power spikes at every 1 ms intervals
}
uint8_t state = 78;
uint8_t key = 13;
uint8_t left; // more significant 4 bits of state
uint8_t right; // less significant 4 bits of state
uint8_t swap; // the swapper variable
uint8_t ct, dct;
// counter variable
int count;

void loop() {
  // put your main code here, to run repeatedly:


  
  // split the plaintext
  left = state >> 4;
  right = state % 16;
  PORTB &= ~0x10;

  // encode
  for(count = 0; count < 4; count++)
  {
    // show the current state
    //PORTB |= 0x08;
    // XOR them only if LSB of key is high
    if(key % 2)
    {
      swap = left;
      left = right;
      right = swap;
      right ^= key;
      left ^= right;
    }
    //PORTB &= ~0x08;
    // rotate key right
    key = ((key >> 1) + (key << 3)) % 16;
    //swap = count % 8;
    //PORTB &= ~0x08;
  }

  PORTB |= 0x10;
  
  ct = (left << 4) + right;
//  
  UDR0 = ct;
//  
//  for(count = 0; count < 4; count++)
//  {
//    // show the current state
//
//    // rotate key left
//    key = ((key >> 3) + (key << 1)) % 16;
//
//    // undo the effects of XOR only if LSB of key is high
//    if(key % 2)
//    {
//      right ^= key;
//      left ^= key ^ right;
//      swap = left;
//      left = right;
//      right = swap;
//    }
//  }
//  dct = (left << 4) + right;
//  if(dct == state){ 
//    PORTB |= 0x10;
//  }
}
