#ifndef WALL_HPP
#define WALL_HPP

class Wall
{
private:
    double x1, y1, x2, y2;

public:
    Wall(double x1, double y1, double x2, double y2)
        : x1(x1), y1(y1), x2(x2), y2(y2) {}

    double getX1() const { return x1; }
    double getY1() const { return y1; }
    double getX2() const { return x2; }
    double getY2() const { return y2; }
};

#endif
