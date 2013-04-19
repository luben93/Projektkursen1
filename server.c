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

//gcc -DNDEBUGG server.c -o serverfinal

#define PORT "3490"  // the port users will be connecting to
#define MAXSIZE 100
#define BACKLOG 10	 // how many pending connections queue will hold
#define NOPLAYERS 4
#define MAINTREADS 2 
#define DAEMON_NAME "gameserver"
#define RUN_AS_USER "me"
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define READ  0
#define WRITE 1	
#define ERROR 2

//pthread_mutex_t chopstick[20] = PTHREAD_MUTEX_INITIALIZER;
//int lock;

struct cli_param
{
  int fd;
  int player;
  int sockfd;
};


void sigchld_handler(int s)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void* steffe (void* grej){
	int n,test;
	char str[MAXSIZE],str2[MAXSIZE];
	struct cli_param* cli = (struct cli_param*) grej;

	//test = close(cli->sockfd); assert(test == 0);// child doesn't need the listener
			printf("new_fd: %d\n",cli->fd);

			///här händer de grejer
			int done = 0;
			while(1){   //infinent loop
                str[0]=0;
      			strcpy(str,"                                             ");
      			//printf("Derp\n");
                n = recv(cli->fd, str, MAXSIZE, 0);  //tar imot skit
      			if (n <= 0) {
          			if (n < 0) perror("recv");
          		    done = 1;
      			}
      			printf("received:%s\n",str);
                

                sprintf(str2, "player:%d", cli->player);
      			send(cli->fd,str2,MAXSIZE,0);
                printf("sent:%s\n",str2);
                
                sleep(1);
    		} 
			 //printf("slut");
            
			//exit(0);
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

static void daemonize( const char *lockfile )
{
    pid_t pid, sid, parent;
    int lfp = -1;

    /* already a daemon */
    if ( getppid() == 1 ) return;

    /* Create the lock file as the current user */
    if ( lockfile && lockfile[0] )
    {
        lfp = open(lockfile,O_RDWR|O_CREAT,0640);
        if ( lfp < 0 )
        {
            syslog( LOG_ERR, "unable to create lock file %s, code=%d (%s)",
                    lockfile, errno, strerror(errno) );
            exit(EXIT_FAILURE);
        }
    }

    /* Drop user if there is one, and we were run as root */
    if ( getuid() == 0 || geteuid() == 0 )
    {
        struct passwd *pw = getpwnam(RUN_AS_USER);
        if ( pw )
        {
            syslog( LOG_NOTICE, "setting user to " RUN_AS_USER );
            setuid( pw->pw_uid );
        }
    }

    /* Trap signals that we expect to recieve */
    signal(SIGCHLD,child_handler);
    signal(SIGUSR1,child_handler);
    signal(SIGALRM,child_handler);

    /* Fork off the parent process */
    pid = fork();
    if (pid < 0)
    {
        syslog( LOG_ERR, "unable to fork daemon, code=%d (%s)",
                errno, strerror(errno) );
        exit(EXIT_FAILURE);
    }
    /* If we got a good PID, then we can exit the parent process. */
    if (pid > 0)
    {

        /* Wait for confirmation from the child via SIGTERM or SIGCHLD, or
           for two seconds to elapse (SIGALRM).  pause() should not return. */
        alarm(2);
        pause();

        exit(EXIT_FAILURE);
    }

    /* At this point we are executing as the child process */
    parent = getppid();

    /* Cancel certain signals */
    signal(SIGCHLD,SIG_DFL); /* A child process dies */
    signal(SIGTSTP,SIG_IGN); /* Various TTY signals */
    signal(SIGTTOU,SIG_IGN);
    signal(SIGTTIN,SIG_IGN);
    signal(SIGHUP, SIG_IGN); /* Ignore hangup signal */
    signal(SIGTERM,SIG_DFL); /* Die on SIGTERM */

    /* Change the file mode mask */
    umask(0);

    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0)
    {
        syslog( LOG_ERR, "unable to create a new session, code %d (%s)",
                errno, strerror(errno) );
        exit(EXIT_FAILURE);
    }

    /* Change the current working directory.  This prevents the current
       directory from being locked; hence not being able to remove it. */
    if ((chdir("/")) < 0)
    {
        syslog( LOG_ERR, "unable to change directory to %s, code %d (%s)",
                "/", errno, strerror(errno) );
        exit(EXIT_FAILURE);
    }

    /* Redirect standard files to /dev/null */
    freopen( "/dev/null", "r", stdin);
    //freopen( "", "w", stdout);
    freopen( "/dev/null", "w", stderr);

    /* Tell the parent process that we are A-okay */
    kill( parent, SIGUSR1 );
}



void* tcpaccept(void* strukt)
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
  struct cli_param thread_args[NOPLAYERS];



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


		thread_args[i].fd=new_fd;
        thread_args[i].player=i;
		pthread_create (&thread_id[i], NULL, &steffe, &thread_args[i]);
		i++;
		
		//test=close(new_fd);assert(test==0);
        //printf("close returns: %d\n",test);  // parent doesn't need this
	}

	return 0;
}

void main(void){
    pthread_t thread_id[MAINTREADS];
    pthread_create (&thread_id[0], NULL, &tcpaccept, NULL);

    //updattering av allas position 

}
