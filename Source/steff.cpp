#include <iostream>
#include <ctime>
//#include <GL/glut.h>
#include <sys/time.h>
#include "Player.h"
#include <stdlib.h>
#include "Platform.cpp"
#include <math.h>
#include <SDL/SDL.h>
#include <vector>
#include "positions.h"
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

#ifdef TARGET_OS_MAC
  // Mac Includes Here
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
  #include <OpenGL/OpenGL.h>
  #include <GLUT/glut.h>
#endif

#ifdef __linux__
  #include <GL/glut.h>
//  #error Can't be compiled on Linux yet
#endif

#define PORT "3490" // the port client will be connecting to 
#define ADDRESS "localhost"
#define MAXSIZE 100 // max number of bytes we can get at once 
#define IP "130.237.84.186"

using namespace std; 

/* Globala variabler */
Player mainPlayer(-15.0f, 10.0f, 0.5f, 1.0f);
vector<Platform> basicPlatform;

// x position för kamera
double x=0.0f,y=0.0f;
unsigned int tex;
	float a;

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
	
	//printf("%s",buf);
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
	printf("Recv: %s\n", buf);
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

int udpanslut(char port[])
{
	//char port[5];
	char positions[12]="31415926535";
	int positions_from_server;
	int i;
	int buf;
	
	
	printf("portnummer: %s\n", port);
	int fd=beejsconnect(port);
	
         	//positions_from_server=lyssna(port);

/*		if (connect(fd,IP,strlen(IP))==-1) {
    		fprintf(stderr,"connect failed udp");
    		return 2;
    		//die("%s",strerror(errno));
		}
*/
        //printf("%d\n",positions_from_server);
	 		//prata_udp(positions,port);
	 	//char skicka[4][]={"",IP,"hejsan",port};
      	prat(IP,"hejsan",port);
      
      	char strs[MAXSIZE];

		buf=hear(fd,strs);
		usleep(5000);
		printf("1.%s\n", strs);
 		buf=hear(fd,strs);
		usleep(5000);
		printf("2.%s\n", strs);
		buf=hear(fd,strs);
		usleep(5000);
		printf("3.%s\n", strs);
		buf=hear(fd,strs);
		usleep(5000);
		printf("4.%s\n", strs);
	
	
} 

void changeSize(int w, int h)
{
	float ratio =  w * (1.0 / h);

	// Använd Projection och ladda om matris
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Ställer in så att spelet finns i hela fönster
	glViewport(0, 0, w, h);
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	// Tillbaka till rätt matris model
	glMatrixMode(GL_MODELVIEW);
}

void getKeys(int key, int xx, int yy) 
{
	switch (key) 
        {
		case GLUT_KEY_LEFT : mainPlayer.moveLeft(); break;
		case GLUT_KEY_RIGHT : mainPlayer.moveRight(); break;
	}
        
        int modifier = glutGetModifiers();

		if (modifier == (GLUT_ACTIVE_CTRL || GLUT_ACTIVE_ALT))
		{
            mainPlayer.setSpeed(0.3f);
        }
        else
        {
            mainPlayer.setSpeed(0.1f);
        }
}

void releaseKey(int key, int x, int y) 
{
	switch (key) 
        {
		case GLUT_KEY_LEFT : mainPlayer.stop();break;
		case GLUT_KEY_RIGHT : mainPlayer.stop();break;
	}
}

void rorelse(unsigned char key, int xx, int yy) 
{
	if (key == 65 || key == 97) 
	mainPlayer.moveLeft();
	if (key == 68 || key == 100) 
	mainPlayer.moveRight();
	
	if (key == 27) 
	exit(0);
	
	if (key == 32) 
	mainPlayer.jump();


       int modifier = glutGetModifiers();

		if (modifier == (GLUT_ACTIVE_CTRL || GLUT_ACTIVE_ALT))
		{
            mainPlayer.setSpeed(0.3f);
        }
        else
        {
            mainPlayer.setSpeed(0.1f);
        }
}

void rorelseStop(unsigned char key, int x, int y) 
{
	if (key == 65 || key == 97) 
	mainPlayer.stop();
	if (key == 68 || key == 100) 
	mainPlayer.stop();
	if (key == 32)
	{
	glutKeyboardFunc(rorelse);
		if (key == 32)
			mainPlayer.doublejump();
	}
}


void kamera(int key, int xx, int yy) 
{
int modifier = glutGetModifiers();
	switch (key) {
		case GLUT_KEY_LEFT :
			mainPlayer.moveLeft();
			if (modifier == (GLUT_ACTIVE_CTRL || GLUT_ACTIVE_ALT))
			{
				//x -= 0.6f;				
            	mainPlayer.setSpeed(0.3f);
			}
			else
			{
				//x -=0.2f;
			    mainPlayer.setSpeed(0.1f);
			}
			break;
		case GLUT_KEY_RIGHT :
			mainPlayer.moveRight();
		{
			if (modifier == (GLUT_ACTIVE_CTRL || GLUT_ACTIVE_ALT))
			{
				//x += 0.6f;				
				mainPlayer.setSpeed(0.3f);
			}
			else
			{
				//x +=0.2f;
				mainPlayer.setSpeed(0.1f);
			}
			break;	
		}
	}
}

void update()
{
        vector<Platform>::iterator it;
	 
	for ( it = basicPlatform.begin(); it != basicPlatform.end(); ++it ) 
        {
                mainPlayer.collisionWithPlatform(it->corners);
        }
        
        mainPlayer.update();
        
        x=mainPlayer.getX();
	y=mainPlayer.getY();
	//cout << x << "|" << y << endl;
}

void renderScene(void) {
 struct player_id player_id_main;
	// Clear Color and Depth Buffers



	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(	mainPlayer.getX(), 1.0f,  40.0f,
				mainPlayer.getX(), 1.0f,  10.0f,
				0.0f, 			   1.0f,  0.0f);

			glBindTexture(GL_TEXTURE_2D,tex);
	        glBegin(GL_QUADS);
	        glTexCoord2f(1.0,1.0);              //höger upp
	        glVertex3f(0.0f, -16.0f, 0.0f);
	        glTexCoord2f(0.0,1.0);
	        glVertex3f(125.0f , -16.0f, 0.0f);
	        glTexCoord2f(0.0,0.0);              //vänster ner
	        glVertex3f(125.0f , 15.0f, 0.0f);
	        glTexCoord2f(1.0,0.0);
	        glVertex3f(0.0f , 15.0f, 0.0f);
	        glEnd();

	     	update();


	        // Uppdaterar spelare
      //  mainPlayer.update();
		mainPlayer.draw();

	a = a + 0.1;
  glBegin(GL_QUADS);
	glVertex3f(1.0f+a, 0.0f+y, 0.0f);
	glVertex3f(1.0f+a, 1.0f+y, 0.0f);
	glVertex3f(0.0f+a, 1.0f+y, 0.0f);
	glVertex3f(0.0f+a, 0.0f+y, 0.0f);
  glEnd();

	   	x=mainPlayer.getX();
	   	y=mainPlayer.getY();
	   	//cout << x << "|" << y << endl;

	   	packa_ihop(x,y,&player_id_main);
        //ta emot
		
		if(str[0]==0)
		{
			glBegin(GL_QUADS);
			glVertex3f(1.0f+a, 0.0f+y, 0.0f);
			glVertex3f(1.0f+a, 1.0f+y, 0.0f);
			glVertex3f(0.0f+a, 1.0f+y, 0.0f);
			glVertex3f(0.0f+a, 0.0f+y, 0.0f);
			glEnd();
		}
        
        vector<Platform>::iterator it;
        for ( it = basicPlatform.begin(); it != basicPlatform.end(); ++it ) 
        {
            it->draw();
        }
        
        if(mainPlayer.getY() <= -3.0f)
            mainPlayer.collisionWithFloor(-3.0f);

       /* if((mainPlayer.getX() >= 40.0f) && (mainPlayer.getX() <= 40.5f) )
        	{
        		if((mainPlayer.getY() <= 2.0f) &&  (mainPlayer.getY() >= -3.0f) )
        		{
					mainPlayer.collisionWithPlatform2(mainPlayer.getY(),mainPlayer.getX());
        		}
        	}  

        if((mainPlayer.getX() >= 60.0f) && (mainPlayer.getX() <= 70.0f) )
        	{
        		if((mainPlayer.getY() <= 1.5f) &&  (mainPlayer.getY() >= 1.0f) )
        		{
           			 mainPlayer.collisionWithFloor(1.5f);
        		}
        	} */


	glutSwapBuffers();
}

void makePlatforms()
{
    Platform *p1 = new Platform(0.0f, 7.0f, 10.0f, .5f);
    basicPlatform.push_back(*p1);
    
    p1 = new Platform(10.0f, 3.0f, 10.0f, .5f);
    basicPlatform.push_back(*p1);
    
    p1 = new Platform(-10.0f, 3.0f, 10.0f, .5f);
    basicPlatform.push_back(*p1);
}

unsigned int  loadTexture(const char* filename)
{
	SDL_Surface* bild=SDL_LoadBMP(filename);
	unsigned int id;
	glGenTextures(0,&id);
	glBindTexture(GL_TEXTURE_2D,id);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,bild->w,bild->h,0,GL_RGB,GL_UNSIGNED_SHORT_5_6_5,bild->pixels);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	SDL_FreeSurface(bild);
	return id;
}



int main(int argc, char **argv) {
	
	int buf;
	char portnum_player[10];
	char playernumber;
	buf=anslut(IP);
	//printf("main buf char %d\n",buf);
	sprintf(portnum_player,"%d",buf);
	//printf("main portnum_player after sprit %s\n",portnum_player);
	//portnum_playerportnum_player[3]-48;	
	//player_id_main.spelarnummer[0]=portnum_player[3]-48;
	
	//printf("main playernumber %d\n", playernumber);
	//printf("main portnum_player %s\n",portnum_player);
	udpanslut(portnum_player);


	// Starta GLUT, skapa ett fönster
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(50,50);
	glutInitWindowSize(800,600);
	glutCreateWindow("PlatformProjekt");

	        makePlatforms();

	glEnable(GL_TEXTURE_2D);
	tex=loadTexture("cave.bmp");

	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
        
        //Tangent funktioner
		//glutIgnoreKeyRepeat(0);
       // glutSpecialFunc(getKeys);
        glutSpecialFunc(kamera);
       	glutSpecialUpFunc(releaseKey);
		glutKeyboardFunc(rorelse);
		glutKeyboardUpFunc(rorelseStop);


			// OpenGL init
	//glEnable(GL_DEPTH_TEST);


	// Gör så att GLUT loopar
	glutMainLoop();

	return 1;
}
