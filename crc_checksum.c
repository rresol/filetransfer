#include<stdio.h>
#include<sys/types.h>
/*
This header file includes the definitions of structures required for sockets
*/
#include<sys/socket.h>
/*
This header file includes constants and structures needed for internet domain addresses
*/
#include<netinet/in.h>
/*
This function is called when a system call fails. It displays message about the fail and abort the program.
*/
#include<netdb.h>
/*
This function defines the structure hostent
*/
#include<strings.h>
#include<string.h>
void error(char *msg)
{
  perror(msg);
  //exit(1);
}
void server(int argc, char* argv[])
{
  int sockfd, newsockfd, portno, clilen, n;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr; //sockaddr_in is a structure defined in netinet.h which contain internet addresses
  if(argc<2)
  {
    fprintf(stderr, "Error no port provided\n");
    //exit(1);
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
  n = read(newsockfd, buffer, 255);
  if(n<0) error("Error reading from socket");
  printf("Here is the message %s\n",buffer);
  n = write(newsockfd, "I got your message",18);
  if(n<0) error("Error writing to the socket.\n");
  //return 0;
}
void client(int argc, char* argv[])
{
  int sockfd, portno, n;
  struct sockaddr_in serv_addr; //Address of the server to which we have to connect
  struct hostent *server; //pointing to struct of type hostent which is defined in netdb.h
  char buffer[256];
  if(argc<3)
  {
    fprintf(stderr, "usage %s hostname port\n",argv[0] );
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

  printf("Please enter the message that is to be sent...\n");
  bzero(buffer,256);
  fgets(buffer,255,stdin);
  n = write(sockfd, buffer, strlen(buffer));
  if(n<0)
  {
    error("Error writing to socket");
  }
  n = read(sockfd,buffer,255);
  if (n < 0)
    error("ERROR reading from socket");
  printf("%s\n",buffer);
  //return 0;
}
int main(int argc, char* argv[])
{
  if(argc<2)
    error("No specified format mentioned");
  if(argc==2)
    server(argc,argv);
  if(argc==3)
    client(argc,argv);
  return 0;
}
