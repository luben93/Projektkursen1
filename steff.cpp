#include <iostream>
#include <ctime>
#include <GL/glut.h>
#include <sys/time.h>
#include "Player.h"
#include <stdlib.h>
#include "Platform.cpp"

/* Globala variabler */
Player mainPlayer;

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
	gluLookAt(0.0f, 0.0f, 10.0f,
                  0.0f, 0.0f,  0.0f,
                  0.0f, 1.0f, 0.0f);
				  
	//triangel
    glBegin(GL_TRIANGLES);
		glVertex3f(-2.0f,-2.0f, 0.0f);
		glVertex3f( 2.0f, 0.0f, 0.0);
		glVertex3f( 0.0f, 2.0f, 0.0);
	glEnd();
    
        // Uppdaterar spelare
        mainPlayer.update();
	mainPlayer.draw();

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
		glutKeyboardFunc(rorelse);
		glutKeyboardUpFunc(rorelseStop);
		

	// Gör så att GLUT loopar
	glutMainLoop();

	return 1;
}