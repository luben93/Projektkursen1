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

#define PORT "3490" // the port client will be connecting to 
#define ADDRESS "localhost"
#define MAXSIZE 100 // max number of bytes we can get at once 

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
	char address[15], port[4];
	int sockfd, numbytes,test; 
	char buf[MAXSIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];
	int t, len;
	char str[MAXSIZE];

	//avkommentera för att välja ip
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

	if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {//Ersätt ADRESS med argv[1]
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
	while(printf("> "), fflush(stdout),fgets(str, 100, stdin), !feof(stdin)) { //skickar
	if (send(sockfd, str, strlen(str), 0) == -0) {
  	    	perror("send");
   	   		exit(1);
   	  	}

		//*
		//do{
			//printf("#1.\n");
			if((numbytes = recv(sockfd, buf, MAXSIZE-1, 0)) == -1) {
		 	  	perror("recv");
    			exit(1);
			}
			//printf("#2.\n");
			if(numbytes!=MAXSIZE-1){
			  buf[numbytes] = '\0';
			}

			//printf("client: received:\n");
			printf("%s",buf);
			//printf("#3.\n");	
		//}while ( strcmp(buf,"server") );
		//printf("$");

		//printf("slut\n");
		//*/
	}
	test=close(sockfd);assert(test==0);

	return 0;
}

