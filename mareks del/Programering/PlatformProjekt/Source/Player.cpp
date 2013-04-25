#include "Player.h"
#include <GL/gl.h>

float Player::getX(void)
{
  return x;
}

float Player::getY(void)
{
  return y;
}

void Player::stop(void)
{
    movingLeft = false;
    movingRight = false;
}

void Player::moveLeft(void)
{
  movingLeft = true;
}

void Player::moveRight(void)
{
  movingRight = true;
}

void Player::setSpeed(float speed)
{
  this->speed = speed;
}

void Player::jump(void)
{
    if(canJump && standing)
    {
        jumping = true;
        standing = false;
    }
}

void Player::collisionWithFloor(int collision_y)
{
    canJump = true;
    standing = true;
    
    y = collision_y+0.01f;
}

void Player::update()
{
  if(movingLeft)
    x -= speed;
  
  if(movingRight)
    x += speed;
  
  y += gravity/100.0f;
  
  if(standing)
    gravity = 0.0f;
  
  if(jumping)
  {
    gravity = 20.0f;
    jumping = false;
  }
  
  if(gravity > -20.0f && !standing)
    gravity -= 0.3f;
}

void Player::draw()
{
  glBegin(GL_QUADS);
	glVertex3f(0.0f+x, 0.0f+y, 0.0f);
	glVertex3f(1.0f+x, 0.0f+y, 0.0f);
	glVertex3f(1.0f+x, 1.0f+y, 0.0f);
	glVertex3f(0.0f+x, 1.0f+y, 0.0f);
  glEnd();
}