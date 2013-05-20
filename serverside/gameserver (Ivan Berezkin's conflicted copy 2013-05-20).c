#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/un.h>
#include <assert.h>
#include <fcntl.h>
#include <syslog.h>
#include <pwd.h>
#include <pthread.h>
#include "udp.h"

//gcc -DNDEBUGG server.c -o serverfinal

#define PORT "3490"  // the port users will be connecting to
#define MAXSIZE 100
#define BACKLOG 10	 // how many pending connections queue will hold
#define NOPLAYERS 30
//#define SERVERPORT "4950"
//#define MYPORT "4950"	
#define MAXBUFLEN 100


#define DAEMON_NAME "gameserver"
#define RUN_AS_USER "me"
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define READ  0
#define WRITE 1	
#define ERROR 2

pthread_mutex_t mtest[NOPLAYERS] = PTHREAD_MUTEX_INITIALIZER;

typedef struct 
 {
 	int actplayer[NOPLAYERS+1];
	char test[NOPLAYERS][MAXSIZE];
 } TESTSTRUCT;

/* Define globally accessible variables and a mutex */


   TESTSTRUCT mystruct; 
   pthread_t callThd[NOPLAYERS];//behövs denna?
   pthread_mutex_t mutexsum;


struct cli_param
{
  int fd;
  int player;
  int udpfd;
  int port;
  //char ip[INET6_ADDRSTRLEN];
  char ip[20];

};


void sigchld_handler(int s)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}

void* clithread (void* grej){
	int n,test;
	char str[MAXSIZE],portstr[MAXSIZE];
	struct cli_param* cli = (struct cli_param*) grej;

	printf("new_fd: %d\n",cli->fd);

	cli->port=cli->player+3800;
  	sprintf(portstr, "%d", cli->port);
    send(cli->fd,portstr,MAXSIZE,0);//skickar portnr baserat på spelarid
    printf("sent:%s\n",portstr);
                
    n = recv(cli->fd, str, MAXSIZE, 0);  //tar imot skit
    if (n <= 0) {
        if (n < 0) perror("recv");
          	
    }
    		//str[n]=0;
    printf("received:%s!\n",str);
      			//strcmp(str,1)
    printf("%s har anslutit tcp\n",cli->ip);

		
	int buf,j;
	char bufstr[MAXSIZE];
	cli->udpfd=beejsconnect(portstr);
	//cli->sockfd=create(portstr);
	printf("%d\n",cli->udpfd);

	for(;;){
            	
       	//buf=hear(cli->udpfd,bufstr);
       	if (!beejsrecv(cli->udpfd,bufstr)){
        	printf("timeout!\n");
        	mystruct.actplayer[cli->player]=0;
        	strcpy(mystruct.test[cli->player],"0|gone");
        	close(cli->udpfd);
        	close(cli->fd);
        	pthread_exit(NULL);
        }

        printf("bufstr: %s\n",bufstr);

        if (!strcmp(bufstr,"godnatt")){
        	printf("bufstr: %s\n",bufstr);
        	mystruct.actplayer[cli->player]=0;
			strcpy(mystruct.test[cli->player],"0|gone");
        	pthread_exit(NULL);
        }
        printf("mutex trylock:%d\n",bufstr[0]-48);
        if(pthread_mutex_trylock(&mtest[bufstr[0]-48])){// to try or to not try that is the question?
   			//sprintf(mystruct.test[cli->player], "%d", buf);
   			
			strcpy(mystruct.test[bufstr[0]-48],bufstr);
   			//strcpy(mystruct.test[cli->player],bufstr);//fungerade förr 
   			
   			//printf("%s\n",mystruct.test[cli->player]);
   			
        	pthread_mutex_unlock(&mtest[bufstr[0]-48]);
        	strcpy(bufstr,"0");
        }else{
        	printf("mutex fail\n");
        }
        //printf("global struct contains:%s\n",mystruct.test[cli->player]);
		for(j=1;j<NOPLAYERS+1;j++){//sends every players buf
			char *skicka[]={"",cli->ip,mystruct.test[j], portstr};
			printf("sent: %s \n",mystruct.test[j]);
         	prat(skicka);
         			
        }
   		usleep(40);
	}

}


static void child_handler(int signum)
{
    switch(signum)
    {
    case SIGALRM:
        exit(EXIT_FAILURE);
        break;
    case SIGUSR1:
        exit(EXIT_SUCCESS);
        break;
    case SIGCHLD:
        exit(EXIT_FAILURE);
        break;
    }
}


int main(void)
{   
	int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int rv;
	int t, len,test,accept_server=0,pid,done;
  	struct sockaddr_un local, remote;
  	char str[100];
  	int i,n=1;
  	int round = 0;
  	//pthread_t thread_id[NOPLAYERS];
  	struct cli_param thread_args[NOPLAYERS];


	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}
		break;
	}

	if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		return 2;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	printf("server: waiting for connections...\n");
	
	for (i=0;i<NOPLAYERS;i++){
		strcpy(thread_args[i].ip,"                  ");
		thread_args[i].fd=0;
		thread_args[i].player=0;
		thread_args[i].port=0;
		thread_args[i].udpfd=0;
		mystruct.actplayer[i]=0;
	}

	while(1) {  // main accept() loop
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}

		inet_ntop(their_addr.ss_family,
			get_in_addr((struct sockaddr *)&their_addr),s, sizeof s);
		printf("new connection %s\n",s);
//------------------------------------------------------------------------
		//while(1){
		for (n=1;n<NOPLAYERS;n++){
			
			if(mystruct.actplayer[n]!=1){

				pthread_t nyaid[n];

				thread_args[n].fd=new_fd;
				thread_args[n].player=n;
				strcpy(thread_args[n].ip,s);
				//printf("%d\n",n);
				//sleep(2);
				mystruct.actplayer[n]=1;
				pthread_create (&nyaid[n], NULL, &clithread, &thread_args[n]);
				printf("server: got connection from player %d on %s \n", n,s);
				n=NOPLAYERS+1;
				break;
			}
			printf("nobreak on %d\n",n);
			//sleep(2);
		}

		if(n==NOPLAYERS){
				send(new_fd,"inga slots lediga",MAXSIZE,0);
		}
		
	}
	//}

	return 0;
}