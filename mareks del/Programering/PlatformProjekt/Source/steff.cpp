#include <iostream>
#include <ctime>
#include <GL/glut.h>
#include <sys/time.h>
#include "Player.h"
#include <stdlib.h>
#include "Platform.cpp"
#include <math.h>

/* Globala variabler */
Player mainPlayer;
Platform basicPlatform(0.0f, -3.0f, 10.0f, 0.5f);

// x position för kamera
float x=0.0f;

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
}

void renderScene(void) {

	// Clear Color and Depth Buffers

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(	x, 1.0f, 40.0f,
			x, 1.0f,  10.0f,
			0.0f, 1.0f,  0.0f);

	        // Uppdaterar spelare
        mainPlayer.update();
		mainPlayer.draw();
	    basicPlatform.draw();
        
        if(mainPlayer.getY() <= -3.0f)
            mainPlayer.collisionWithFloor(-3.0f);

	glutSwapBuffers();
}


void kamera(int key, int xx, int yy) 
{
int modifier = glutGetModifiers();
	switch (key) {
		case GLUT_KEY_LEFT :
			mainPlayer.moveLeft();
			if (modifier == (GLUT_ACTIVE_CTRL || GLUT_ACTIVE_ALT))
			{
            	mainPlayer.setSpeed(0.3f);
				x -= 0.3f;
			}
			else
			{
				x -=0.1f;
			    mainPlayer.setSpeed(0.1f);
			}
			break;
		case GLUT_KEY_RIGHT :
			mainPlayer.moveRight();
		{
			if (modifier == (GLUT_ACTIVE_CTRL || GLUT_ACTIVE_ALT))
			{
				mainPlayer.setSpeed(0.3f);
				x += 0.3f;
			}
			else
			{
				x +=0.1f;
				mainPlayer.setSpeed(0.1f);
			}
			break;	
		}
	}
}

int main(int argc, char **argv) {

	// Starta GLUT, skapa ett fönster
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(50,50);
	glutInitWindowSize(800,600);
	glutCreateWindow("PlatformProjekt");

	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
        
        // Tangent funktioner
	//glutIgnoreKeyRepeat(1);
       // glutSpecialFunc(getKeys);
                glutSpecialFunc(kamera);
       glutSpecialUpFunc(releaseKey);
		glutKeyboardFunc(rorelse);
		glutKeyboardUpFunc(rorelseStop);

			// OpenGL init
	glEnable(GL_DEPTH_TEST);
		

	// Gör så att GLUT loopar
	glutMainLoop();

	return 1;
}