#include<stdio.h>
#include<string.h>
#include<stdint.h>

#define POLYNOMIAL 0xD8

typedef uint8_t crc;

#define WIDTH (8* sizeof(crc))
#define TOPBIT (1<<(WIDTH-1))

crc crcTable[256];

void crcInit(void)
{
  crc remainder;
  int dividend,bit;
  for(dividend =0; dividend <256; dividend++)
  {
    remainder = dividend << (WIDTH-8);
    for(bit=8;bit>0;--bit)
    {
      if(remainder & TOPBIT)
      {
        remainder = (remainder<<1)^POLYNOMIAL;
      }
      else{
        remainder = remainder<<1;
      }
    }
    crcTable[dividend] = remainder;
    //printf("%d %d\n",dividend,remainder );
  }
}

crc calcCrc(char message[], int n)
{
  crc rem =0;
  uint8_t data;
  int i;
  for(i=0;i<n;i++)
  {
    data = message[i]^((rem>>WIDTH-8));
    //printf("%u\n",data );
    rem = crcTable[data]^(rem<<8);
  }
  return rem;
}
int main()
{
  FILE *fp;
  crcInit();
  char message[1000];
  fp = fopen("data.txt","r");
  if(fp){
    size_t newLen  = fread(message,sizeof(char),450,fp);
    if(ferror(fp)!=0)
    {
      fputs("Error reading the file",stderr);
    }
    else{
      message[newLen++] = '\0';
    }
  }

  printf("%u\n",calcCrc(message,450));
}
