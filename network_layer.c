#include<stdio.h>
#include<stdlib.h>
#define PCKT_LEN 10000000
struct ipheader{
	unsigned char ip_hl:5, ip_v:4;
	unsigned char ip_tos;
	unsigned short int ip_len;
	unsigned short int ip_id;
	unsigned char ip_flag;
	unsigned short int ip_off;
	unsigned char ip_ttl;
	unsigned char ip_protocol;
	unsigned short int chksum;
	unsigned int ip_src;
	unsigned int ip_dst;
}
char *dest_ip_str;
unsigned char buffer[PCKT_LEN];
unsigned short csum(unsigned short *buf, int nwords)
{
	unsigned long sum;
	for(sum=0;nwords>0;nwords--) sum+= *buff++;
	sum = (sum >> 16) + (sum &0xffff);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}

void ipheader_init()
{
	struct ipheader* ip = (struct ipheader*) buffer;
	ip->ip_hl  = 5;
	ip->ip_v = 4;
	ip->ip_tos = 16;
	ip->ip_len = sizeof(struct ipheader);
	ip->ip_id = htons(54321);
	ip->ip_ttl = 64;
	ip->ip_protocol = 6; //Defines the layout of header that will immdeiately follow the IPV4 header. possible values 0-255
	ip->chksum = 0;
	ip->chksum = ((unsigned short * )buffer, sizeof(struct(ipheader)));
}

void sender()
{
	printf("Adding IP header...... \n");
	memset(buffer,0,PCKT_LEN);
	FILE *in, *out;
	in = fopen("transport_out","r");
	out = fopen("network_out","wb");
	unsigned char *data = (unsigned char*)(sizeof(struct ipheader) + buffer);
	int total = fread(data,1,PCKT_LEN-sizeof(struct ipheader),in );
	ipheader_init();
	total+=	sizeof(struct ipheader);
	fwrite(buffer,1,total,out);
	fclose(in);
	fclose(out);
}

void reciever()
{
	printf("Decoding IP header .....\n");
	memset(buffer,0,PCKT_LEN);
	FILE *in, *out;
	out = fopen("transport_in","wb");
	in = fopen("network_in","r");
	unsigned char *data = (unsigned char*) (sizeof(struct ipheader)+buffer);
	int total = fread(buffer, 1, PCKT_LEN, in);
	total -= sizeof(struct ipheader);
	if(csum((unsigned short*)buffer, sizeof(struct ipheader))==0)
	{
		fwrite(data,1,total,out);
	}
	else{
		printf("IP Header Error - CheckSum...!!\nAborting.....!");
			 exit(-1);
	}
	fclose(in);
	fclose(out);
}
int main()
{
	printf("In Networl layer .......\n");
	dest_ip_str = argv[2];
	if(argv[1][0]=='1') sender();
	else if(argv[1][0]=='2') reciever();
	else printf("Wrong Agruments.......!!\nAborting.......!!\n"),exit(-1);
  printf("Network Layer Work Done..........!!\n");
  return 0;
}
