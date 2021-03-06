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
#define SERVERPORT "4950"
#define MYPORT "4950"	
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
	char test[MAXSIZE];
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
  //char ip[INET6_ADDRSTRLEN];
  char *ip;

};
//*/
struct serverthread_params{
	int active[NOPLAYERS];
	int posx[NOPLAYERS];
	int posy[NOPLAYERS];
	int action[NOPLAYERS];
	int test[NOPLAYERS];
};
//*/

void sigchld_handler(int s)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}

void* clithread (void* grej){
	int n,test;
	char str[MAXSIZE],str2[MAXSIZE];
	struct cli_param* cli = (struct cli_param*) grej;
	fflush(stdout);
	//test = close(cli->sockfd); assert(test == 0);// child doesn't need the listener
			printf("new_fd: %d\n",cli->fd);

			///här händer de grejer
			int done = 0;
			//do{   //infinent loop
                
      			strcpy(str,"                                             ");
      			str[0]=0;
      			//printf("Derp\n");
      			sprintf(str2, "%d", cli->player);
      			send(cli->fd,str2,MAXSIZE,0);
                printf("sent:%s\n",str2);
                
                n = recv(cli->fd, str, MAXSIZE, 0);  //tar imot skit
      			if (n <= 0) {
          			if (n < 0) perror("recv");
          		    done = 1;
      			}
      			str[n]=0;
      			printf("received:%s!\n",str);
      			//strcmp(str,1)

                usleep(500);
    		//}while(!done);
		//*/	
			for(;;){
				char *skicka[]={"",cli->ip,mystruct.test};
				int buf;
    	        printf("%s har anslutit tcp\n",cli->ip);

	        	//for(;;){

        		//}

            	prat(skicka);
         		buf=lyssna();
         		if(pthread_mutex_trylock(&mtest[cli->player])){
         			sprintf(mystruct.test, "%d", buf);
         			pthread_mutex_unlock(&mtest[cli->player]);
         		}
         		printf("global struct contains:%s\n",mystruct.test);
         		usleep(50);
			}

         	//chop up buf
         	//mutex_ptherad_trylock(test[cli->player]);

			//*/
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
  	int i=1;
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

	while(1) {  // main accept() loop
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}

		inet_ntop(their_addr.ss_family,
			get_in_addr((struct sockaddr *)&their_addr),s, sizeof s);
		printf("server: got connection from player %d on %s \n", i,s);

		//!!!!!!!!!!!
		//!!!kolla om en ny spelare har samma ip som en gammal och lägg in den nya på den gammla tråden
		//!!!!!!!!!!!
		int j,ipprev;
		//funkar typ inte
		for(j=1;j>=4;j++){
			if(!strcmp(thread_args[j].ip,s)){
				ipprev=j;
				break;
			}
		}

		if (!ipprev){
			thread_args[i].fd=new_fd;
			thread_args[i].player=i;
			thread_args[i].ip=s;
			pthread_create (&thread_id[i], NULL, &clithread, &thread_args[i]);
			i++;
		}else{
			thread_args[j].fd=new_fd;
			ipprev=0;
		}
		//test=close(new_fd);assert(test==0);
        //printf("close returns: %d\n",test);  // parent doesn't need this
	}

	return 0;
}


