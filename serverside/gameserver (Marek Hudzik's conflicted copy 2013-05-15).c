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

        //printf("bufstr: %s\n",bufstr);

        if (!strcmp(bufstr,"godnatt")){
        	printf("bufstr: %s\n",bufstr);
        	mystruct.actplayer[cli->player]=0;
			strcpy(mystruct.test[cli->player],"0|gone");
        	pthread_exit(NULL);
        }
        if(pthread_mutex_trylock(&mtest[cli->player])){// to try or to not try that is the question?
   				//sprintf(mystruct.test[cli->player], "%d", buf);
   			
				strcpy(mystruct.test[cli->player],bufstr);
   				//strcpy(mystruct.test[cli->player],bufstr);//fungerade förr 
   			
   				printf("wrote:%s on %d\n",mystruct.test[cli->player],cli->player);
   			
    	    	pthread_mutex_unlock(&mtest[cli->player]);
    	    	strcpy(mystruct.test,"0");
         	}else{
        		printf("mutex fail\n");
        	}

        /*// non working piece of shit code
        //printf("mutex trylock:%d\n",bufstr[0]-48);
        if (bufstr[3]-48<=4){
        	if(pthread_mutex_trylock(&mtest[cli->player])){// to try or to not try that is the question?
   				//sprintf(mystruct.test[cli->player], "%d", buf);
   			
				strcpy(mystruct.test[cli->player],bufstr);
   			//strcpy(mystruct.test[cli->player],bufstr);//fungerade förr 
   			
   				printf("wrote:%s on %d\n",mystruct.test[cli->player],cli->player);
   			
    	    	pthread_mutex_unlock(&mtest[cli->player]);
        		
    	    }else if(pthread_mutex_trylock(&mtest[bufstr[3]-48])){
				strcpy(mystruct.test[bufstr[3]-48],bufstr);
   			//strcpy(mystruct.test[cli->player],bufstr);//fungerade förr 
   			
   				printf("wrote:%s on %d\n",mystruct.test[bufstr[3]-48],bufstr[3]-48);
   			
        		pthread_mutex_unlock(&mtest[bufstr[3]-48]);
       	 	}else{
 