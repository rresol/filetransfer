#include<stdio.h>
#include<sys/types.h> /*This header file includes the definitions of structures required for sockets*/
#include<sys/socket.h>//This header file includes constants and structures needed for internet domain addresses
#include<netinet/in.h>//This function is called when a system call fails. It displays message about the fail and abort the program.
#include<netdb.h>     //This function defines the structure hostent
#include<strings.h>
#include<string.h>
#include<stdlib.h>


typedef uint8_t crc;
#define WIDTH (8*sizeof(crc));
#define TOPBIT(1<<(WIDTH-1));
#define POLYNOMIAL 0x04C11DB7 //Truncated POLYNOMIAL for CRC 32 calculation.

#define PCKT_LEN 10000
unsigned char buffer[PCKT_LEN];

struct frame{
  unsigned char preamble[8], dest[6], src[6];
  unsigned short int len;
  unsigned char data[PCKT_LEN];
  unsigned int crc;
}


void frame_init(struct frame* fr, char message[], unsigned int message_len)
{
  for(int i=0;i<7;i++)fr->preamble[i] = 0xAA;
  fr->preamble[7] = 0xAB;

  for(int i=0;i<strlen(message);i++)
  {
    fr->data[i] = message[i];
  }

  fr->len = total;

  fr->dest[0]
  fr->dest[1]
  fr->dest[2]
  fr->dest[3]
  fr->dest[4]
  fr->dest[5]

  fr->src[0]
  fr->src[1]
  fr->src[2]
  fr->src[3]
  fr->src[4]
  fr->src[5]

}


void error(char *msg)
{
  perror(msg);
  exit(-1);
}

//Server code to recieve data and send the response at the end.
void server(int argc, char* argv[])
{
  int sockfd, newsockfd, portno, clilen, n;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr; //sockaddr_in is a structure defined in netinet.h which contain internet addresses

  if(argc<2)
  {
    fprintf(stderr, "Error no port provided\n");
    exit(1);
  }

  sockfd = socket(AF_INET,SOCK_STREAM,0);

  if(sockfd<0)
    error("Error opening socket");

  bzero((char *) &serv_addr, sizeof(serv_addr)); //try printing the sizeof serv_addr;

  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(portno);
  serv_addr.sin_addr.s_addr = INADDR_ANY;

  if(bind(sockfd,(struct sockaddr*) &serv_addr, sizeof(serv_addr))<0)
    error("Error on binding");
  else
    printf("I am listenig...\n");

  listen(sockfd,5);
  clilen = sizeof(cli_addr);
  newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

  if(newsockfd<0)
    error("Error on accept");

  bzero(buffer,256);

  FILE *fp;
  int i=0;
  fp = fopen("data1.txt","w");
  printf("Recieving Data\n");

  while(read(newsockfd, buffer, 255)>0){
  fputs(buffer,fp);
  memset(buffer, 0, 255);
  }

  shutdown(newsockfd,0);
  printf("data Recieved\n");
  printf("%d\n",write(newsockfd, "Server is no longer Recieving the data", 38));
  shutdown(newsockfd,2);
  close(newsockfd);
}

// Generating CRC Look up table to accelarate the CRC computation for each byte.
void crcInit()
{
  int dividend;
  crc remainder;
  uint8_t bit;
  for(dividend=0;dividend<256;dividend++)
  {
    remainder = dividend<<(WIDTH-8);
    for(bit=8;bit>0;--bit)
    {
      if(remainder&TOPBIT)
      {
        remainder = (remainder<<1)^POLYNOMIAL;
      }
      else
      {
        remainder= (remainder<<1);
      }
    }
    crcTable[dividend] = remainder;
  }
}

//calculation of CRC using CRC look up table
crc crcCalc(char message[], int message_len)
{
  int i;
  crc remainder =0;
  uint8_t data;
  for(i=0;i<message_len;i++)
  {
    data = message[i]^(remainder>>(WIDTH-8));
    remainder = crcTable[data]^(remainder<<8);
  }
  return remainder;
}



//client code to send data over network.
void client(int argc, char* argv[])
{
  int sockfd, portno, n;
  struct sockaddr_in serv_addr; //Address of the server to which we have to connect
  struct hostent *server; //pointing to struct of type hostent which is defined in netdb.h
  char buffer[256];


  if(argc<3)
  {
    fprintf(stderr, "usage %s hostname port\n",argv[0] );
    exit(1);
  }


  portno = atoi(argv[2]);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd<0)
    error("Error opening socket");
  server = gethostbyname(argv[1]);
  if(server==NULL)
    fprintf(stderr, "Error! No such host\n");
  bzero((char*)&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr,
      (char *)&serv_addr.sin_addr.s_addr,
      server->h_length);
  serv_addr.sin_port = htons(portno);

  if (connect(sockfd,(struct sockaddr*) &serv_addr,sizeof(serv_addr)) < 0)
    error("ERROR connecting");

  bzero(buffer,256);

  FILE *fp;
  fp = fopen("data.txt","r");
  printf("Sending message please wait...\n");

  while(fgets(buffer,255,fp)!=NULL)
  {
    n = write(sockfd,buffer,strlen(buffer));
    memset(buffer, 0, 255);
  }

  printf("All the data have from the file %s have been sent\n", argv[3]);
  shutdown(sockfd,1);
  read(sockfd,buffer,38);
  printf("%s\n",buffer);
  shutdown(sockfd,2);
  close(sockfd);
}

int main(int argc, char* argv[])
{

  if(argc==2){
    server(argc,argv);}
  else if(argc==4){
    printf("coming alpha...\n");
    client(argc,argv);}
  else{
    error("Invalid arguments! Aborting...");
  }
  return 0;
}
