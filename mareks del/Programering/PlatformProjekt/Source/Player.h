class Player
{
  private:
    float x, y, speed, gravity;
    bool movingLeft, movingRight, jumping, canJump, standing;
    
  public:
    float getX(void);
    float getY(void);
    
    void moveLeft(void);
    void moveRight(void);
    void stop(void);
    void collisionWithFloor(int);
    void jump(void);
    void setSpeed(float);
    
    void update();
    void draw();
};