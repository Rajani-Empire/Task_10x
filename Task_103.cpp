#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int CIRCLE_RADIUS = 20;
const int CIRCLE_SPEED = 2;

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;

int circle1X = SCREEN_WIDTH / 2;
int circle1Y = SCREEN_HEIGHT / 2;
int circle2X = SCREEN_WIDTH / 2;
int circle2Y = 0;

bool initSDL();
void closeSDL();
void moveCircles(SDL_Event& e);
bool checkCollision();
void drawCircles();

int main(int argc, char* args[]) {
    if (!initSDL()) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            moveCircles(e);
        }

        if (checkCollision()) {
            // Visual effect of collision
            SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255); // Set color to red
        } else {
            SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255); // Set color to black
        }

        SDL_RenderClear(gRenderer);
        drawCircles();
        SDL_RenderPresent(gRenderer);
    }

    closeSDL();
    return 0;
}

bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    gWindow = SDL_CreateWindow("Collision Detection", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void closeSDL() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;
    gRenderer = nullptr;

    SDL_Quit();
}

void moveCircles(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_UP:
                if (circle2Y - CIRCLE_SPEED >= 0)
                    circle2Y -= CIRCLE_SPEED;
                break;
            case SDLK_DOWN:
                if (circle2Y + CIRCLE_SPEED <= SCREEN_HEIGHT)
                    circle2Y += CIRCLE_SPEED;
                break;
            case SDLK_LEFT:
                if (circle2X - CIRCLE_SPEED >= 0)
                    circle2X -= CIRCLE_SPEED;
                break;
            case SDLK_RIGHT:
                if (circle2X + CIRCLE_SPEED <= SCREEN_WIDTH)
                    circle2X += CIRCLE_SPEED;
                break;
            default:
                break;
        }
    }

    // Move circle1 from left to right continuously
    circle1X += CIRCLE_SPEED;
    if (circle1X > SCREEN_WIDTH + CIRCLE_RADIUS) {
        circle1X = -CIRCLE_RADIUS; // Reset to the left side when it reaches the right edge
    }
}

bool checkCollision() {
    int deltaX = circle1X - circle2X;
    int deltaY = circle1Y - circle2Y;
    int distanceSquared = deltaX * deltaX + deltaY * deltaY;
    int radiiSquared = (CIRCLE_RADIUS + CIRCLE_RADIUS) * (CIRCLE_RADIUS + CIRCLE_RADIUS);
    return distanceSquared <= radiiSquared;
}

void drawCircles() {
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255); // Set color to blue for circle1
    SDL_Rect fillRect1 = { circle1X - CIRCLE_RADIUS, circle1Y - CIRCLE_RADIUS, CIRCLE_RADIUS * 2, CIRCLE_RADIUS * 2 };
    SDL_RenderFillRect(gRenderer, &fillRect1);

    SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255); // Set color to yellow for circle2
    SDL_Rect fillRect2 = { circle2X - CIRCLE_RADIUS, circle2Y - CIRCLE_RADIUS, CIRCLE_RADIUS * 2, CIRCLE_RADIUS * 2 };
    SDL_RenderFillRect(gRenderer, &fillRect2);
}
