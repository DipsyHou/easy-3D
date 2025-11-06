#ifndef UTILS_HPP
#define UTILS_HPP

#include "space.hpp"

// add arc-shaped wall to space
void addArcWall(Space& space, double centerX, double centerY, double radius, double startAngle, double endAngle, int segments = 50)
{
    double angleStep = (endAngle - startAngle) / segments;
    
    for (int i = 0; i < segments; ++i) {
        double angle1 = (startAngle + i * angleStep) * M_PI / 180.0;
        double angle2 = (startAngle + (i + 1) * angleStep) * M_PI / 180.0;
        
        double x1 = centerX + radius * std::cos(angle1);
        double y1 = centerY + radius * std::sin(angle1);
        double x2 = centerX + radius * std::cos(angle2);
        double y2 = centerY + radius * std::sin(angle2);
        
        space.addWall(Wall(x1, y1, x2, y2));
    }
}


#endif