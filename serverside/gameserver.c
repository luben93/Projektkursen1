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
#define NOPLAYERS 4
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
    int active[NOPLAYERS];
	int posx[NOPLAYERS];
	int posy[NOPLAYERS];
	int action[NOPLAYERS];
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
  int sockfd;
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
                
      			//strcpy(str,"                                             ");
      			//str[0]=0;
      			//printf("Derp\n");
      		

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
	//!!!!!behöver timeout !!!!!! kanske även en ticker som dödar efter visst antal sek
	//cli->sockfd=beejsconnect(portstr);
	cli->sockfd=create(portstr);
	printf("%d\n",cli->sockfd);

	for(;;){
            	
       	//buf=hear(cli->sockfd,bufstr);//!!!!!behöver timeout !!!!!!! kanske även en ticker som dödar efter visst antal sek!!
        if (!beejsrecv(cli->sockfd,bufstr)){
        	printf("timeout!\n");

        }
        printf("bufstr: %s\n",bufstr);
        if(pthread_mutex_trylock(&mtest[cli->player])){// to try or to not try that is the question?
   			//sprintf(mystruct.test[cli->player], "%d", buf);
   			strcpy(mystruct.test[cli->player],bufstr);
   			printf("%s",mystruct.test[cli->player]);
         	pthread_mutex_unlock(&mtest[cli->player]);
        }
        //printf("global struct contains:%s\n",mystruct.test[cli->player]);
		for(j=1;j<NOPLAYERS+1;j++){//sends every players buf
			char *skicka[]={"",cli->ip,mystruct.test[j], portstr};
			printf("sent:%d %s \n",j,mystruct.test[j]);
         	prat(skicka);
         			
        }
   		usleep(50);//ska vara usleep(500);
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
  //noPhils=atoi(argv[1]);
  //lock=atoi(argv[2]);
  	int round = 0;

  	pthread_t thread_id[NOPLAYERS];
  	pthread_t nyaid;
  	struct cli_param thread_args[NOPLAYERS];
  	//struct serverthread_params posthreadargs;
	//  pthread_create (&nyaid,NULL,&spelsync,&posthreadargs);

  	/*
	openlog(DAEMON_NAME,LOG_PID,LOG_LOCAL5);	//daemonize
    syslog(LOG_INFO, "starting");
    printf("daemonize server\n");
    daemonize("/tmp/" DAEMON_NAME);
    dup2(accept_server,STDOUT_FILENO);
    dup2(accept_server,STDERR_FILENO);
	//*/

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
	
	for (i=1;i<NOPLAYERS;i++){
		strcpy(thread_args[i].ip,"                  ");
		thread_args[i].fd=0;
		thread_args[i].player=0;
		thread_args[i].port=0;
		thread_args[i].sockfd=0;
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
		printf("server: got connection from player %d on %s \n", n,s);

		//!!!!!!!!!!!
		//!!!kolla om en ny spelare har samma ip som en gammal och lägg in den nya på den gammla tråden
		//!!!!!!!!!!!
		int j,ipprev=0;
		//funkar typ inte
		for(j=1;j<=NOPLAYERS;j++){//kollar om ip addressen redan anslutit
			//printf("nya:%s\ngammla:%s\n",s,thread_args[j].ip);
			if(!strcmp(thread_args[j].ip,s)){
				printf("byter %s\n", s);
				ipprev=j;
				break;
			}
		}

		//if (ipprev == 0){
			thread_args[n].fd=new_fd;
			thread_args[n].player=n;
			strcpy(thread_args[n].ip,s);
			
			pthread_create (&thread_id[n], NULL, &clithread, &thread_args[n]);
			
			n++;
			//printf("FÖRSTA RAD IF\n");
		/*}else{//ska återanvanda gammla trådar för gammla ipaddresser, fast tror inte de går
			thread_args[ipprev].fd=new_fd;
			pthread_create (&thread_id[ipprev], NULL, &clithread, &thread_args[ipprev]);// så kanske funkar iaf 
			ipprev=0;
			//printf("ANDRA RAD IF\n");
		}*/
		
		}

	return 0;
}


