#include "BigNumber.h"

////////////////////////////////////////////////////////////////////////////////

// declarations
BigNumber p, q, n, e, d, M, C, temp;

// p = first prime number
// q = second prime number
// n = modulus
// e = public encryption key
// d = private decryption key
// M = plaintext message
// C = ciphertext message

////////////////////////////////////////////////////////////////////////////////

// load the set by commenting out the others
void load_parameters()
{
//p = "88993"; q = "652361"; e = "25728589759"; d = "65599";
//p = "23831"; q = "65657"; e = "342202493"; d = "34277";
//p = "82839349"; q = "64629403"; e = "663618299"; d = "2324818243004987";
//p = "1598669"; q = "9654637"; e = "59796733427"; d = "850022758091";
//p = "8546999"; q = "9764663"; e = "6137965949564399"; d = "70994704783375";
//p = "792179"; q = "98796457"; e = "98756664949787954613259"; d = "4436988421939";
//p = "9547133"; q = "7798798571"; e = "655332465895669"; d = "18702745650660789";
//p = "2134657957"; q = "541324799"; e = "32164325997845"; d = "384945969131490773";
//p = "654328001"; q = "325681"; e = "3231245796371"; d = "10097080277531";
//p = "766423243"; q = "8756645239"; e = "6325486985"; d = "6676181896971476057";
//p = "96457993"; q = "38632240001"; e = "64321349"; d = "992493553292811149";
//p = "632146589"; q = "3213465821"; e = "97321465798543"; d = "107763315118965647";
//p = "796423207"; q = "97463231"; e = "3321873249562633"; d = "8948760622563097";
//p = "2319546799"; q = "3216479579"; e = "32165785348564519"; d = "6827596968971589571";
//p = "646796663"; q = "3216467977"; e = "3216452587559543"; d = "422890635578730455";
//p = "9666571"; q = "3223496387"; e = "327894135562325868983"; d = "24967491312354467";
//p = "23246581"; q = "3216579599"; e = "86475478514821"; d = "51280870137213061";
p = "56629561"; q = "658998563"; e = "58765268265842237"; d = "26499063470653493";
//p = "796854397"; q = "445125607"; e = "5452445456525"; d = "55468967563257125";
//p = "465821567"; q = "545654519"; d = "1501199875790165"; e = "225944803239810553";
//p = "7659569"; q = "886266257"; d = "375299968947541"; e = "2854008828413";
//p = "6562679"; q = "645413207"; d = "93824992236885"; e = "4188087330657813";
//p = "432564851"; q = "255322657"; d = "23456248059221"; e = "108350090592771581";
//p = "763285427"; q = "46243331"; d = "5864062014807"; e = "6153389883703543";
//p = "58756499"; q = "485415883"; d = "1466015503703"; e = "19834803414905483";
//p = "5358751"; q = "877845251"; d = "366503875927"; e = "130542716600863";
//p = "7657571"; q = "986254613"; d = "91625968981"; e = "1643994285535421";
//p = "752952671"; q = "87525481"; d = "22906492249"; e = "42269236847669449";
}

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

void setup()
{
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  Serial.begin(2000000);
  SREG &= ~0x80;
  BigNumber::begin(); // initialize BigNumber library
  BigNumber::setScale(0); // use 0 decimal places (i.e. use integers only)
}

////////////////////////////////////////////////////////////////////////////////

void loop()
{
  // set the RSA parameters
  load_parameters();
  n = p * q;
  M = "58373258";
  
  // encrypt 'M' to get 'C'
  C = "1";
  temp = M;
  while(e > 0)
  {
    if(e % BigNumber(2))
    {
      C = C * temp % n;
    }
    e /= 2;
    temp = temp * temp % n;
  }
  Serial.print("C = ");
  Serial.println(C);

  // convert BigNumber to uint64_t
  int cipher[64] = {0};
  for(int i = 0; C > 0; i++, C /= 2)
  {
    if(C % BigNumber(2))
    {
      cipher[i] = 1;
    }
  }
  uint64_t result = 0;
  for(int i = 63; i >= 0; i--)
  {
    if(cipher[i] == 1)
    {
      result = result * 2 + 1;
    }
    else
    {
      result = result * 2;
    }
  }
  Serial.print("C = ");
  print64(result);
  // break 'result' into 8 chunks, each one of 8 bytes
  // send those 8 bytes one after another over UART
}
