/*
 * Main.cpp
 *
 *  Created on: 13 apr 2013
 *      Author: marek
 */

#include <iostream>
#include <ctime>
#include <GL/glut.h>
#include <time.h>
#include "Player.h"
#include "Platform.cpp"
#include <stdlib.h>

#define GLUT_KEY_SPACEBAR 32
#define GLUT_KEY_a 97

/* Globala variabler */
Player mainPlayer;
Platform basicPlatform(0.0f, -3.0f, 10.0f, 0.5f);



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
                case GLUT_KEY_UP : mainPlayer.jump(); break;
	}
        
        int modifier = glutGetModifiers();

        if (modifier == 1) //ALT=4  SHIFT=1  CTRL=2
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
	if (key == 'a' || key == 'A')
        {
            std::cout << "BAJS!" << std::endl;
            mainPlayer.moveLeft();
                std::cout << "BAJS2" << std::endl;
        }
	if (key == 68 || key == 100) 
	mainPlayer.moveRight();
	
	if (key == 27) 
	exit(0);
	
	if (key == 32) 
	{
	mainPlayer.jump();
		//if (key == 32) 
		//mainPlayer.jump();
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

	//Rengör buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
        
	// Ställer in kamera
	gluLookAt(0.0f, 0.0f, 30.0f,
                  0.0f, 0.0f,  10.0f,
                  0.0f, 1.0f, 0.0f);
        
        // Uppdaterar spelare
        mainPlayer.update();
	mainPlayer.draw();
        basicPlatform.draw();
        
        if(mainPlayer.getY() <= -3.0f)
            mainPlayer.collisionWithFloor(-3.0f);

        // Ändrar buffer
	glutSwapBuffers();
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
        glutSpecialFunc(getKeys);
        glutSpecialUpFunc(releaseKey);
        

	// Gör så att GLUT loopar
	glutMainLoop();

	return 1;
}