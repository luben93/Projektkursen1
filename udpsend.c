#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/uio.h>
#include <stdio.h>
#include <stdlib.h>

int create( const char* portname){
	
const char* hostname=0; /* wildcard */
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
    exit (2);
    //die("failed to resolve local socket address (err=%d)",err);
}

int fd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
if (fd==-1) {
    printf("%s",strerror(errno));
    exit (2);
    //die("%s",strerror(errno));
}

if (bind(fd,res->ai_addr,res->ai_addrlen)==-1) {
    printf("%s",strerror(errno));
    exit (2);
    //die("%s",strerror(errno));
}

freeaddrinfo(res);
return fd;
}


int hear(int fd){

char buffer[12]="";
struct sockaddr_storage src_addr;
socklen_t src_addr_len=sizeof(src_addr);
ssize_t count=recvfrom(fd,buffer,sizeof(buffer),0,(struct sockaddr*)&src_addr,&src_addr_len);
if (count==-1) {
    printf("%s",strerror(errno));
    exit (2);
    //die("%s",strerror(errno));
} else if (count==sizeof(buffer)) {
    warn("datagram too large for buffer: truncated");
} else {
    //handle_datagram(buffer,count);
}
return atoi(buffer);
}

///
int main (void){

char str[6];
printf("mata mig portar:");
fgets(str,5,stdin);
printf("tack\n");

int fd=create(str);
while(1){
printf("%d\n",hear(fd));
usleep(500);
}


}
//*/

