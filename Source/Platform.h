class Platform
{
  private:
    float x, y, width, height;
    
  public:
      Platform(float, float, float, float);
      
    float getX(void);
    float getY(void);
    float getWidth(void);
    float getHeight(void);
    
    void draw(void);
};