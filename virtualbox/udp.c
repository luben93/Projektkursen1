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
#include <sys/uio.h>


#define SERVERPORT argv[3]
#define MYPORT portnr	// the port users will be connecting to

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

int lyssna(char *portnr)
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


	//måste särlägga bind / recv

	addr_len = sizeof their_addr;
	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
		(struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		return 2;
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

	//close(sockfd);
	//shutdown(sockfd,2); 

	return bufint;
}

int beejsconnect(char *portnr)
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
return sockfd;
}


int beejssend(int sockfd){
	//måste särlägga bind / recv
		int numbytes;
	struct sockaddr_storage their_addr;
	char buf[MAXBUFLEN];
	socklen_t addr_len;
	char s[INET6_ADDRSTRLEN];

	addr_len = sizeof their_addr;
	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
		(struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		return 2;
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

	//close(sockfd);
	//shutdown(sockfd,2); 

	return bufint;
}


int create( const char* portname){
	
const char* hostname=0; /* wildcard */ //bestämm vilken ip !!!!! :@
//const char* portname="daytime";
struct addrinfo hints;
memset(&hints,0,sizeof(hints));
hints.ai_family=AF_UNSPEC;
hints.ai_socktype=SOCK_DGRAM;
hints.ai_protocol=0;
hints.ai_flags=AI_PASSIVE|AI_ADDRCONFIG;
struct addrinfo* res=0;
int err=getaddrinfo(hostname,portname,&hints,&res);

if (err!=0) {
    printf("failed to resolve local socket address (err=%d)",err);
    return 2;
    //die("failed to resolve local socket address (err=%d)",err);
}

int fd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
if (fd==-1) {
    printf("%s",strerror(errno));
    return 2;
    //die("%s",strerror(errno));
}

if (bind(fd,res->ai_addr,res->ai_addrlen)==-1) {
    printf("%s",strerror(errno));
    return 2;
    //die("%s",strerror(errno));
}

freeaddrinfo(res);
return fd;
}


int hear(int fd){

char buffer[549];
struct sockaddr_storage src_addr;
socklen_t src_addr_len=sizeof(src_addr);
ssize_t count=recvfrom(fd,buffer,sizeof(buffer),0,(struct sockaddr*)&src_addr,&src_addr_len);
if (count==-1) {
    printf("%s",strerror(errno));
    return 2;
    //die("%s",strerror(errno));
} else if (count==sizeof(buffer)) {
    warn("datagram too large for buffer: truncated");
} else {
    //handle_datagram(buffer,count);
}
return atoi(buffer);
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
        return()
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
        return 2;
    }

    freeaddrinfo(servinfo);

    //printf("talker: sent %d bytes to %s\n", numbytes, argv[1]);
    close(sockfd);

    return 0;
}


