#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#define SRV_IP "130.237.84.186"
#define BUFLEN 512
#define NPACK 10
#define PORT atoi(argv[1])

void diep(char *s)
{
perror(s);
exit(1);
}

int main(int argc,char *argv[])
{
	if(argv==0){
		printf("port?");
	}
struct sockaddr_in si_other;
long int j;
int fd, slen=sizeof(si_other);
char buf[BUFLEN];

int i;
{
if ((fd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
diep("socket");

memset((char *) &si_other, 0, sizeof(si_other));
si_other.sin_family = AF_INET;
si_other.sin_port = htons(PORT);

if (inet_aton(SRV_IP, &si_other.sin_addr)==0)
{
fprintf(stderr, "inet_aton() failedn");
exit(1);
}
}

for (j=0; j<NPACK * 1000 * 1000; j++) 
{
sprintf(buf, "512 length string you can type here anything this is so and so packetn");
if (sendto(fd, buf, BUFLEN, 0, &si_other, slen)==-1)
diep("sendto()");
}
close(fd);
return 0;
}