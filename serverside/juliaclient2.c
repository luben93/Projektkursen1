/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <assert.h>
#include <arpa/inet.h>
#include "udp.h"
#include "positions.h"

#define PORT "3490" // the port client will be connecting to 
#define ADDRESS "localhost"
#define MAXSIZE 100 // max number of bytes we can get at once 
#define IP "130.237.84.186"

// get sockaddr, IPv4 or IPv6:
/*void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}*/

//int main(int argc, char *argv[])
int anslut(char *ip)
{
	char address[15], port[4];
	int sockfd, numbytes,test; 
	char buf[MAXSIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];
	int t, len;
	char str[MAXSIZE];
	
	/*/avkommentera för att välja ip
	if (argc != 2) {
		//strcpy(address,ADDRESS);
		//strcpy(port,PORT);
		//printf("localhost används\n");
	    fprintf(stderr,"usage: client hostname\n");
	    exit(1);
	}else{
		//int i;
		//for (i=0;i<argc;i++){
		//	printf("%s",argv[i]);
		//}
		//strcpy(address,argv[1]);
		//strcpy(port,argv[3]);
	}
	//*/
	
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(ip, PORT, &hints, &servinfo)) != 0) {//Ersätt ADRESS med argv[1]
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}


	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),s, sizeof s);
	printf("client: connecting to %s\n", s);

	freeaddrinfo(servinfo); // all done with this structure
	//char str2[]="hej";
	
	//Här så skriver man ett kommando på client sidan 
	//while(printf("> "), fflush(stdout),fgets(str, 100, stdin), !feof(stdin)) { //skickar
	//Här skickas det man har skrivit på clienten till servern
	
	if((numbytes = recv(sockfd, buf, MAXSIZE-1, 0)) == -1) {
		perror("recv");

    		exit(1);

		}
	
	printf("%s",buf);
	//strcpy(str,"hej");
	if (send(sockfd, "julia", 4, 0) == -0) {
  	   	perror("send");
   	   		exit(1);
   	  	}
	/*if (send(sockfd, "mamma", strlen(str), 0) == -0) {
  	   	perror("send");
   	   		exit(1);
   	  	}
*/

	test=close(sockfd);assert(test==0);
	//printf("Luden suger");
	return atoi(buf);
}


int prata_udp(char *send_positions,char *port)
{
int    bytes_sent;
int    server_sock;
struct sockaddr_in to_addr;

to_addr.sin_family      = AF_INET;
to_addr.sin_port        = *port;
to_addr.sin_addr.s_addr = *IP;
bytes_sent = sendto(server_sock, send_positions, sizeof(send_positions), 0,
                    (struct sockaddr *)&to_addr, sizeof(to_addr));
}

int udpanslut(char portnum_player[], char str[])
{
	char port[5];
	char positions[12]="31415926535";
	int positions_from_server;
	int i;
	int buf;
	
	strcpy(port,portnum_player);
	int fd=beejsconnect(port);
		
         	//positions_from_server=lyssna(port);

/*		if (connect(fd,IP,strlen(IP))==-1) {
    		fprintf(stderr,"connect failed udp");
    		return 2;
    		//die("%s",strerror(errno));
		}
*/
        printf("%d\n",positions_from_server);
	 		//prata_udp(positions,port);
	 	char *skicka[]={"",IP,str,port};
      	prat(skicka);
/*      	if(positions_from_server<1000){
      		fprintf(stderr,"wrong buf udp");
      		return 2;
       	}*/
		buf=hear(fd);
		usleep(5000);
		printf("%d", buf);
 		usleep(5000);
		buf=hear(fd);
		usleep(5000);
		printf("%d", buf);
 		usleep(5000);
		buf=hear(fd);
		usleep(5000);
		printf("%d", buf);
 		usleep(5000);
		buf=hear(fd);
 		usleep(5000);
		printf("%d", buf);
 		usleep(5000);

	
}
/*int juliab (float *pos)
{
	int buf;
	char portnum_player[10];
	char playernumber;
	char str[12];
	float yko;
	buf=anslut(IP);
	sprintf(portnum_player,"%d",buf);
	playernumber=portnum_player[3]-48;
	/*printf(">");
	fgets(str,12,stdin);
	printf("\n");*/
	//float pos=atof(str);
	/*yko=dela_upp(player_playar,pos[0]);
	printf("yko %f\n",yko);
	udpanslut(portnum_player,str);
return 0;
}*/

