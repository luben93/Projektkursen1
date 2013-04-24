

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <assert.h>


#define SERVERPORT "3800"
#define MYPORT "3800"	// the port users will be connecting to

#define MAXBUFLEN 100


#define PORT "3490" // the port client will be connecting to 
//#define ADDRESS "localhost"
#define MAXSIZE 100 // max number of bytes we can get at once 

/*
** listener.c -- a datagram sockets "server" demo
*/
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int lyssna(void)
{
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;
	struct sockaddr_storage their_addr;
	char buf[MAXBUFLEN];
	socklen_t addr_len;
	char s[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("listener: socket");
			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("listener: bind");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "listener: failed to bind socket\n");
		return 2;
	}

	freeaddrinfo(servinfo);

	//printf("listener: waiting to recvfrom...\n");

	addr_len = sizeof their_addr;
	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
		(struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		exit(1);
	}

	//printf("listener: got packet from %s\n",
		inet_ntop(their_addr.ss_family,
			get_in_addr((struct sockaddr *)&their_addr),
			s, sizeof s);
	//printf("listener: packet is %d bytes long\n", numbytes);
	buf[numbytes] = '\0';
	//printf("listener: packet contains \"%s\"\n", buf);

	int bufint=atoi(buf);
	//info[0]=bufint[0];
	//printf("listener: int contains \"%d\"\n", bufint);

	close(sockfd);
	return bufint;
}

/*
** talker.c -- a datagram "client" demo
*/

    // the port users will be connecting to

//int main(int argc, char *argv[])
int prat (char *argv[])
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;

  /*  if (argc != 3) {
        fprintf(stderr,"usage: talker hostname message\n");
        exit(1);
    }*/
        ////printf("helloworld\nip:%s\nmessage:%s\n",argv[1],argv[2]);
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo(argv[1], SERVERPORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "talker: failed to bind socket\n");
        return 2;
    }

    if ((numbytes = sendto(sockfd, argv[2], strlen(argv[2]), 0,
             p->ai_addr, p->ai_addrlen)) == -1) {
        perror("talker: sendto");
        exit(1);
    }

    freeaddrinfo(servinfo);

    //printf("talker: sent %d bytes to %s\n", numbytes, argv[1]);
    close(sockfd);

    return 0;
}


/*
** client.c -- a stream socket client demo
*/



/*
//int main(int argc, char *argv[])
int clilogon( char *argv[])
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
		////printf("localhost används\n");
	    fprintf(stderr,"usage: client hostname\n");
	    exit(1);
	}else{
		//int i;
		//for (i=0;i<argc;i++){
		//	//printf("%s",argv[i]);
		//}
		//strcpy(address,argv[1]);
		//strcpy(port,argv[3]);
	}
	///
	
	
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
	//printf("client: connecting to %s\n", s);

	freeaddrinfo(servinfo); // all done with this structure
	//char str2[]="hej";
	//*
	while(//printf("> "), fflush(stdout),fgets(str, 100, stdin), !feof(stdin)) { //skickar
	if (send(sockfd, str, strlen(str), 0) == -0) {
  	    	perror("send");
   	   		exit(1);
   	  	}

		//*
		//do{
			////printf("#1.\n");
			if((numbytes = recv(sockfd, buf, MAXSIZE-1, 0)) == -1) {
		 	  	perror("recv");
    			exit(1);
			}
			////printf("#2.\n");
			if(numbytes!=MAXSIZE-1){
			  buf[numbytes] = '\0';
			}

			////printf("client: received:\n");
			//printf("%s",buf);
			////printf("#3.\n");	
		//}while ( strcmp(buf,"server") );
		////printf("$");

		////printf("slut\n");
		///
	}
	test=close(sockfd);assert(test==0);
///
	return 0;
}
//*/
