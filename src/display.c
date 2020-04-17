#include "display.h"

// global vars
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;
int window_width = 800;
int window_height = 600;

// The void parameter prevents passing in other args.
bool initialize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }

    // Set to max width/height (full screen)
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    window_width = display_mode.w;
    window_height = display_mode.h;

    // Create SDL window
    window =
        SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         window_width, window_height, SDL_WINDOW_BORDERLESS);
    if (!window) {
        fprintf(stderr, "Error creating SDL window.\n");
        return false;
    }

    // Create SDL renderer
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL renderer.\n");
        return false;
    }

    return true;
}

void render_color_buffer(void) {
    // https://wiki.libsdl.org/SDL_UpdateTexture
    SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer,
                      (int)(window_width * sizeof(uint32_t)));

    // https://wiki.libsdl.org/SDL_RenderCopy
    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

/**
 * Draw a grid of dots every `spacing` pixels by changing the color_buffer
 *
 * spacing: how many pixels between each grid line
 */
void draw_grid(int spacing) {
    for (int y = 0; y < window_height; y += spacing) {
        for (int x = 0; x < window_width; x += spacing) {
            color_buffer[(window_width * y) + x] = 0xFF333333;
        }
    }
}

/**
 * Draw a pixel
 */
void draw_pixel(int x, int y, uint32_t color) {
    if (x < window_width && y < window_height) {
        color_buffer[(window_width * y) + x] = color;
    }
}

/**
 * Draw a rectangle on the screen.
 */
void draw_rect(int x, int y, int width, int height, uint32_t color) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int cur_x = x + i;
            int cur_y = y + j;
            color_buffer[(window_width * cur_y) + cur_x] = color;
        }
    }
}

/**
 * Set all the pixels in the window to the given color.
 */
void clear_color_buffer(uint32_t color) {
    for (int y = 0; y < window_height; y++) {
        for (int x = 0; x < window_width; x++) {
            // The pixels are in a single, linear array.
            color_buffer[(window_width * y) + x] = color;
        }
    }
}

void destroy_window(void) {
    // Clean up the things that were created above.
    free(color_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
