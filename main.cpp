#include "space.hpp"
#include "utils.hpp"
#include <iostream>
#include <cmath>

int main()
{
    Space space(0.0, -2, 0.0, 60.0, 800, 600);
    space.initSDL();
    
    addArcWall(space, 0, 0, 8, 0, 180, 720);
    space.addWall(Wall(8, 0, -8, 0));

    // 运行交互循环
    space.runInteractLoop(0.1, 3.0, 60, true, 0.2);

    return 0;
}
