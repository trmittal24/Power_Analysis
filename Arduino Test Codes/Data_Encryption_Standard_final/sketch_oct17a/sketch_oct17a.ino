#define FIRSTBIT 0x8000000000000000 

uint64_t data = 0xAAAA444AAAAA444A;
 uint64_t data_temp;
int const InitialPermutation[64] =
{
  58, 50, 42, 34, 26, 18, 10,  2,
  60, 52, 44, 36, 28, 20, 12,  4,
  62, 54, 46, 38, 30, 22, 14,  6,
  64, 56, 48, 40, 32, 24, 16,  8,
  57, 49, 41, 33, 25, 17,  9,  1,
  59, 51, 43, 35, 27, 19, 11,  3,
  61, 53, 45, 37, 29, 21, 13,  5,
  63, 55, 47, 39, 31, 23, 15,  7
};


 void addbit(uint64_t from, int position_from, int position_to, int ii)
{   
  //PORTB ^= 0x10;
  
  if(ii%5 != 0)
  {
    data_temp += (FIRSTBIT >> position_to);
    //*block *= ii ;
  }
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  pinMode(12, OUTPUT);
  SREG &= ~0x80 ; // pervent power spikes at every 1 ms intervals
  data_temp = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
   
  PORTB ^= 0x10;
 
  data_temp++;

  // write each bit of data_temp according to the permuted order
  for(int ii = 0; ii < 64; ii++)
  {
      addbit(data, InitialPermutation[ii] - 1, ii, ii);
//       if(ii%4 != 0)
//    {
//      data_temp += (FIRSTBIT >> ii);
//      //*block *= ii ;
//    }
  }
        


}
