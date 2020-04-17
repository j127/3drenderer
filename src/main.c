#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "display.h"

// global vars
bool is_running = false;

void setup(void) {
    // Allocate the required memory in bytes to hold the color buffer
    color_buffer =
        (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);

    // Create an SDL texture to display the color
    color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             window_width, window_height);
}

void process_input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            is_running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) is_running = false;
            break;
    }
}

void update(void) {
    // TODO:
}

void render(void) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);

    draw_grid(100);
    draw_rect(200, 200, 100, 100, 0xFF00FF33);  /* Apple IIc green */
    draw_rect(600, 400, 150, 150, 0xFFF1C232);  /* Pikuma yellow */
    draw_rect(900, 100, 60, 150, 0xFF000F89);   /* phthalo blue */
    draw_rect(1200, 600, 150, 100, 0xFFE32636); /* alizarin crimson */
    for (int i = 0; i < 600; i++) {
        draw_pixel(i + 10, i + 30, 0xFFFFFF00);
    }

    render_color_buffer();
    clear_color_buffer(0xFF000000);

    SDL_RenderPresent(renderer);
}

int main(void) {
    /* Create an SDL window */
    is_running = initialize_window();

    setup();

    while (is_running) {
        process_input();
        update();
        render();
    }

    destroy_window();

    return 0;
}
