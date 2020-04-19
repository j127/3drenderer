#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

// This could be 30, 60, 200 FPS or whatever you want.
#define FPS 200
#define FRAME_TARGET_TIME (1000 / FPS)

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern uint32_t* color_buffer;
extern SDL_Texture* color_buffer_texture;
extern int window_width;
extern int window_height;

bool initialize_window(void);
void clear_color_buffer(uint32_t color);
void destroy_window(void);
void draw_grid(int spacing);
void draw_pixel(int x, int y, uint32_t color);
void draw_rect(int x, int y, int width, int height, uint32_t color);
void render_color_buffer(void);

#endif
