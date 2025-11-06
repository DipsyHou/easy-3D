#ifndef VIEWPOINT_HPP
#define VIEWPOINT_HPP

#include <cmath>

class Viewpoint
{
private:
    double posX;
    double posY;
    double towards;
    double viewAngle;
    int screenWidth;
    int screenHeight;

public:
    Viewpoint() 
        : posX(0.0), posY(0.0), towards(0.0), 
          viewAngle(60.0), screenWidth(800), screenHeight(600) {}
    
    Viewpoint(double x, double y, double t, double angle, int width, int height)
        : posX(x), posY(y), towards(t), 
          viewAngle(angle), screenWidth(width), screenHeight(height) {}

    // Getters
    double getPosX() const { return posX; }
    double getPosY() const { return posY; }
    double getTowards() const { return towards; }
    double getViewAngle() const { return viewAngle; }
    int getScreenWidth() const { return screenWidth; }
    int getScreenHeight() const { return screenHeight; }

    // Setters
    void setPosX(double x) { posX = x; }
    void setPosY(double y) { posY = y; }
    void setTowards(double t) { towards = t; }
    void setViewAngle(double angle) { viewAngle = angle; }
    void setScreenWidth(int width) { screenWidth = width; }
    void setScreenHeight(int height) { screenHeight = height; }

    void move(double distance) 
    {
        double rad = towards * M_PI / 180.0;
        posX += distance * std::cos(rad);
        posY += distance * std::sin(rad);
    }

    void strafe(double distance)
    {
        // 横向移动：朝向方向垂直90度
        double rad = (towards + 90.0) * M_PI / 180.0;
        posX += distance * std::cos(rad);
        posY += distance * std::sin(rad);
    }

    void tryMove(double distance, double newX, double newY)
    {
        posX = newX;
        posY = newY;
    }

    void rotate(double angle) 
    {
        towards += angle;
        while (towards >= 360.0) towards -= 360.0;
        while (towards < 0.0) towards += 360.0;
    }
};

#endif
