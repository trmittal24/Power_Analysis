void setup() {
 //  Serial.begin(115200); // maximum baud rate
   pinMode(11, OUTPUT);
   pinMode(12, OUTPUT);

   SREG &= ~0x80; // pervent power spikes at every 1 ms intervals
}
unsigned long int state = 78;
unsigned long int key = 42069;
unsigned long int left; // more significant 4 bits of state
unsigned long int right; // less significant 4 bits of state
unsigned long int swap; // the swapper variable
unsigned long int ct, dct;
int count;

void loop() {  
  // split the plaintext
  left = state >> 16;
  right = state % 65536;
  
  // reference signal to indicate start of loop
  PORTB &= ~0x10;
  
  // loop
  for(count = 0; count < 16; count++)
  {
    //PORTB |= 0x08;
    // operation performed only if key is odd
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
    key = ((key >> 1) + (key << 15)) % 65536;
  }
  
  // reference signal to indicate end of loop
  PORTB |= 0x10;
  
  // obtain encrypted value and send it
  ct = (left << 4) + right;
 // UDR0 = ct;
  
  // decoding
  /*
  for(count = 0; count < 4; count++)
  {
    // rotate key left
    key = ((key >> 3) + (key << 1)) % 16;

    // undo the effects of XOR only if LSB of key is high
    if(key % 2)
    {
      right ^= key;
      left ^= key ^ right;
      swap = left;
      left = right;
      right = swap;
    }
  }
  dct = (left << 4) + right;
  if(dct == state){ 
    PORTB |= 0x10;
  }
  */
}
