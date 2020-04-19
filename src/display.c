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
    if (x >= 0 && x < window_width && y >= 0 && y < window_height) {
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
            draw_pixel(cur_x, cur_y, color);
        }
    }
}

/**
 * Draw a triangle on the screen.
 */
void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2,
                   uint32_t color) {
    draw_line(x0, y0, x1, y1, color);
    draw_line(x1, y1, x2, y2, color);
    draw_line(x2, y2, x0, y0, color);
}

/**
 * Draw a line on the screen using the DDA algorithm.
 */
void draw_line(int x0, int y0, int x1, int y1, uint32_t color) {
    int delta_x = (x1 - x0);
    int delta_y = (y1 - y0);

    // Set the counter to the longest side, whether it's x or y
    int longest_side_length =
        (abs(delta_x) >= abs(delta_y)) ? abs(delta_x) : abs(delta_y);

    // Calculate how much to increase x and y in each step.
    // It casts one number to float, otherwise it will be integer
    // division. Division is a slow operation, so if there are
    // performance issues, we'll switch to Bresenham' algorithm.
    float x_inc = delta_x / (float)longest_side_length;
    float y_inc = delta_y / (float)longest_side_length;

    float current_x = x0;
    float current_y = y0;

    for (int i = 0; i <= longest_side_length; i++) {
        draw_pixel(round(current_x), round(current_y), color);
        // The longer side increments by 1 (the longest side divided by
        // the longest_side_length is 1), and other side increments by
        // an amount that depends on the slope of the line.
        current_x += x_inc;
        current_y += y_inc;
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
