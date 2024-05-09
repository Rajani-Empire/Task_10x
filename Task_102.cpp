#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#define SCREEN_WIDTH 300
#define SCREEN_HEIGHT 300
#define INITIAL_RADIUS 100
#define RADIUS_INCREMENT 1
#define MAX_RADIUS 150

bool initializeSDL(SDL_Window** window, SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return false;
    }

    *window = SDL_CreateWindow("Circle Drawing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (*window == NULL) {
        printf("Window creation failed: %s\n", SDL_GetError());
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (*renderer == NULL) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void drawSolidCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    for (int x = -radius; x <= radius; x++) {
        for (int y = -radius; y <= radius; y++) {
            if (x*x + y*y <= radius*radius) {
                SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if (!initializeSDL(&window, &renderer)) {
        return 1;
    }

    SDL_Event event;
    bool running = true;
    int currentRadius = INITIAL_RADIUS;
    int direction = 1;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 100, 100, 200, 255); // Change background color (RGB: 100, 100, 200)
        SDL_RenderClear(renderer);

        // Increase or decrease the radius
        currentRadius += direction * RADIUS_INCREMENT;

        // Check if the circle collides with window boundaries
        if (SCREEN_WIDTH / 2 + currentRadius >= SCREEN_WIDTH || SCREEN_HEIGHT / 2 + currentRadius >= SCREEN_HEIGHT || SCREEN_WIDTH / 2 - currentRadius <= 0 || SCREEN_HEIGHT / 2 - currentRadius <= 0) {
            // Reset radius to initial value
            currentRadius = INITIAL_RADIUS;
        }

        SDL_SetRenderDrawColor(renderer, 50, 200, 100, 255); // Change circle color (RGB: 50, 200, 100)
        drawSolidCircle(renderer, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, currentRadius);

        SDL_RenderPresent(renderer);

        // Cap the frame rate
        SDL_Delay(10);

        // Change direction if maximum radius is reached
        if (currentRadius >= MAX_RADIUS || currentRadius <= 0) {
            direction *= -1;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

