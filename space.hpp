#ifndef SPACE_HPP
#define SPACE_HPP

#include "viewpoint.hpp"
#include "wall.hpp"
#include <SDL2/SDL.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <memory>

class Space
{
public:
    struct RayResult
    {
        double distance;
        bool hit;
        const Wall* hitWall;
        RayResult() : distance(1e9), hit(false), hitWall(nullptr) {}
    };

    Space(double posX = 0.0, double posY = 0.0, double towards = 0.0, double viewAngle = 60.0, int width = 800, int height = 600)
        : viewpoint(posX, posY, towards, viewAngle, width, height),
          window(nullptr), renderer(nullptr), texture(nullptr)
    {
        pixelBuffer.resize(width * height, 0);
    }

    ~Space() 
    {
        cleanup();
    }

    bool initSDL()
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            return false;
        }

        int width = viewpoint.getScreenWidth();
        int height = viewpoint.getScreenHeight();

        window = SDL_CreateWindow(
            "Easy 3D Engine",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width, height,
            SDL_WINDOW_SHOWN
        );

        if (!window) {
            return false;
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            return false;
        }

        texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            width, height
        );

        if (!texture) {
            return false;
        }

        return true;
    }

    void cleanup()
    {
        if (texture) SDL_DestroyTexture(texture);
        if (renderer) SDL_DestroyRenderer(renderer);
        if (window) SDL_DestroyWindow(window);
        SDL_Quit();
    }

    Viewpoint* getViewpoint() { return &viewpoint; }

    void addWall(const Wall& wall) 
    {
        walls.push_back(wall);
    }

    RayResult castRay(double rayAngle) const
    {
        RayResult result;
        
        double rayOriginX = viewpoint.getPosX();
        double rayOriginY = viewpoint.getPosY();
        
        double angleRad = rayAngle * M_PI / 180.0;
        double rayDirX = std::cos(angleRad);
        double rayDirY = std::sin(angleRad);
        
        double closestDistance = 1e9;
        
        for (const auto& wall : walls) {
            double wallX1 = wall.getX1();
            double wallY1 = wall.getY1();
            double wallX2 = wall.getX2();
            double wallY2 = wall.getY2();
            
            double wallDirX = wallX2 - wallX1;
            double wallDirY = wallY2 - wallY1;
            
            double denominator = rayDirX * wallDirY - rayDirY * wallDirX;
            
            if (std::abs(denominator) < 1e-10) {
                continue;
            }
            
            double dx = wallX1 - rayOriginX;
            double dy = wallY1 - rayOriginY;
            
            double t = (dx * wallDirY - dy * wallDirX) / denominator;
            double s = (dx * rayDirY - dy * rayDirX) / denominator;
            
            if (t >= 0 && s >= 0 && s <= 1) {
                if (t < closestDistance) {
                    closestDistance = t;
                    result.hit = true;
                    result.distance = t;
                    result.hitWall = &wall;
                }
            }
        }
        
        return result;
    }

    // naive collision detection
    // TODO: Try other methods to optimize
    bool checkCollision(double newX, double newY, double radius = 0.1) const
    {
        for (const auto& wall : walls) {
            double x1 = wall.getX1();
            double y1 = wall.getY1();
            double x2 = wall.getX2();
            double y2 = wall.getY2();
            
            double dx = x2 - x1;
            double dy = y2 - y1;
            double lengthSq = dx * dx + dy * dy;
            
            // TODO: magic number
            if (lengthSq < 1e-10) {
                double dist = std::sqrt((newX - x1) * (newX - x1) + (newY - y1) * (newY - y1));
                if (dist < radius) return true;
                continue;
            }
            
            double t = std::max(0.0, std::min(1.0, 
                ((newX - x1) * dx + (newY - y1) * dy) / lengthSq));
            
            double closestX = x1 + t * dx;
            double closestY = y1 + t * dy;
            
            double dist = std::sqrt((newX - closestX) * (newX - closestX) + 
                                   (newY - closestY) * (newY - closestY));
            
            if (dist < radius) {
                return true;
            }
        }
        
        return false;
    }

    bool tryMove(double distance)
    {
        double rad = viewpoint.getTowards() * M_PI / 180.0;
        double newX = viewpoint.getPosX() + distance * std::cos(rad);
        double newY = viewpoint.getPosY() + distance * std::sin(rad);
        
        if (!checkCollision(newX, newY)) {
            viewpoint.tryMove(distance, newX, newY);
            return true;
        }
        return false;
    }

    bool tryStrafe(double distance)
    {
        double rad = (viewpoint.getTowards() + 90.0) * M_PI / 180.0;
        double newX = viewpoint.getPosX() + distance * std::cos(rad);
        double newY = viewpoint.getPosY() + distance * std::sin(rad);
        
        if (!checkCollision(newX, newY)) {
            viewpoint.tryMove(distance, newX, newY);
            return true;
        }
        return false;
    }

    void render()
    {
        const int width = viewpoint.getScreenWidth();
        const int height = viewpoint.getScreenHeight();
        const double halfFov = viewpoint.getViewAngle() / 2.0;
        const int halfHeight = height / 2;
        const double degToRad = M_PI / 180.0;

        // render ceiling and floor
        // TODO: magic number
        for (int y = 0; y < height; ++y) {
            uint32_t color = (y < halfHeight) ? 0xFF1A1A2E : 0xFF3A3A3A;
            for (int x = 0; x < width; ++x) {
                pixelBuffer[y * width + x] = color;
            }
        }

        const double angleStep = viewpoint.getViewAngle() / width;
        // render walls
        for (int x = 0; x < width; ++x) {
            double relativeAngle = -halfFov + angleStep * x;
            double absoluteAngle = viewpoint.getTowards() + relativeAngle;
            
            RayResult result = castRay(absoluteAngle);
            
            if (!result.hit) continue;
            
            double correctedDistance = result.distance * std::cos(relativeAngle * degToRad);
            
            // TODO: magic number
            double wallHeight = (5.0 / correctedDistance) * height * 0.5;
            int wallTop = static_cast<int>((height - wallHeight) / 2.0);
            int wallBottom = static_cast<int>((height + wallHeight) / 2.0);
            
            // calculate brightness from wall angle and distance
            double dx = result.hitWall->getX2() - result.hitWall->getX1();
            double dy = result.hitWall->getY2() - result.hitWall->getY1();
            double wallAngle = std::atan2(dy, dx);
            double brightness = std::abs(std::cos(wallAngle));
            // TODO: magic number
            double distanceFade = std::max(0.0, 1.0 - correctedDistance / 30.0);
            brightness *= distanceFade;
            
            // TODO: magic number
            uint8_t r = static_cast<uint8_t>(brightness * 200);
            uint8_t g = static_cast<uint8_t>(brightness * 150);
            uint8_t b = static_cast<uint8_t>(brightness * 100);
            uint32_t wallColor = 0xFF000000 | (r << 16) | (g << 8) | b;
            
            for (int y = std::max(0, wallTop); y < std::min(height, wallBottom); ++y) {
                pixelBuffer[y * width + x] = wallColor;
            }
        }

        SDL_UpdateTexture(texture, nullptr, pixelBuffer.data(), width * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    bool isRunning() const
    {
        return window != nullptr;
    }

    void runInteractLoop(double moveSpeed = 0.1, double rotateSpeed = 3.0, 
                         int maxFPS = 60, bool enableCollision = true,
                         double mouseSensitivity = 0.1)
    {
        bool running = true;
        SDL_Event event;
        bool mouseCaptured = true;
        
        int frameDelay = (maxFPS > 0) ? (1000 / maxFPS) : 0;
        
        SDL_SetRelativeMouseMode(SDL_TRUE);
        SDL_ShowCursor(SDL_DISABLE);
        
        while (running && isRunning()) {
            
            const Uint8* keyState = SDL_GetKeyboardState(NULL);
            bool altPressed = keyState[SDL_SCANCODE_LALT] || keyState[SDL_SCANCODE_RALT];
            
            // Press Alt to release mouse
            if (altPressed && mouseCaptured) {
                SDL_SetRelativeMouseMode(SDL_FALSE);
                SDL_ShowCursor(SDL_ENABLE);
                mouseCaptured = false;
            } else if (!altPressed && !mouseCaptured) {
                SDL_SetRelativeMouseMode(SDL_TRUE);
                SDL_ShowCursor(SDL_DISABLE);
                mouseCaptured = true;
            }
            
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                }
                
                // mouse motion
                if (event.type == SDL_MOUSEMOTION && mouseCaptured) {
                    int mouseX = event.motion.xrel;
                    viewpoint.rotate(- mouseX * mouseSensitivity);
                }
                
                // ESC
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
            }
            
            // nomalized movement speed when moving diagonally
            bool moveForward = keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP];
            bool moveBackward = keyState[SDL_SCANCODE_S] || keyState[SDL_SCANCODE_DOWN];
            bool moveLeft = keyState[SDL_SCANCODE_A];
            bool moveRight = keyState[SDL_SCANCODE_D];
            
            int moveDirections = (moveForward || moveBackward ? 1 : 0) + 
                                 (moveLeft || moveRight ? 1 : 0);
            
            double actualSpeed = (moveDirections == 2) ? moveSpeed * 0.70710678118 : moveSpeed;
            
            // WSAD
            if (moveForward) {
                if (enableCollision) {
                    tryMove(actualSpeed);
                } else {
                    viewpoint.move(actualSpeed);
                }
            }
            if (moveBackward) {
                if (enableCollision) {
                    tryMove(-actualSpeed);
                } else {
                    viewpoint.move(-actualSpeed);
                }
            }
            if (moveLeft) {
                if (enableCollision) {
                    tryStrafe(-actualSpeed);
                } else {
                    viewpoint.strafe(-actualSpeed);
                }
            }
            if (moveRight) {
                if (enableCollision) {
                    tryStrafe(actualSpeed);
                } else {
                    viewpoint.strafe(actualSpeed);
                }
            }
            
            // <- and ->
            if (keyState[SDL_SCANCODE_LEFT]) {
                viewpoint.rotate(-rotateSpeed);
            }
            if (keyState[SDL_SCANCODE_RIGHT]) {
                viewpoint.rotate(rotateSpeed);
            }
            
            render();

        // control frame rate
        SDL_Delay(frameDelay);
        }
        
        // release mouse
        SDL_SetRelativeMouseMode(SDL_FALSE);
        SDL_ShowCursor(SDL_ENABLE);
    }

private:
    Viewpoint viewpoint;
    std::vector<Wall> walls;
    std::vector<uint32_t> pixelBuffer;
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
};

#endif
