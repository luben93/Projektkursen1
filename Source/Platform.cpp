#include "Platform.h"
#include <GL/glut.h>

Platform::Platform(float x, float y, float width, float height)
{
    this->width = width;
    this->height = height;
    this->x = x;
    this->y = y;
}

void Platform::draw()
{
    glBegin(GL_QUADS);
	glVertex3f(0.0f+x, 0.0f+y, 0.0f);
	glVertex3f(width+x, 0.0f+y, 0.0f);
	glVertex3f(width+x, height+y, 0.0f);
	glVertex3f(0.0f+x, height+y, 0.0f);
    glEnd();
}