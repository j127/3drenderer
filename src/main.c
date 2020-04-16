#include <SDL2/SDL.h>
#include <stdbool.h>  // for `bool` type
#include <stdio.h>

// global vars
bool is_running = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

// The void parameter prevents passing in other args.
bool initialize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }
    // TODO: Create SDL window
    window =
        SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         800, 600, SDL_WINDOW_BORDERLESS);
    if (!window) {
        fprintf(stderr, "Error creating SDL window.\n");
        return false;
    }

    // TODO: Create SDL renderer
    renderer = SDL_CreateRenderer(Window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL renderer.\n");
        return false;
    }

    return true;
}

int main(void) {
    /* TODO: Create an SDL window */
    is_running = initialize_window();

    return 0;
}
