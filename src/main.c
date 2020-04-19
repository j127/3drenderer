#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "display.h"
#include "vector.h"

bool is_running = false;
int previous_frame_time = 0;  // This will be milliseconds.

// Declare an array of vectors/points
#define N_POINTS (9 * 9 * 9)  // 9x9x9 cube (729 vectors)
vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];

vec3_t camera_position = {.x = 0, .y = 0, .z = -5};
vec3_t cube_rotation = {.x = 0, .y = 0, .z = 0};

float fov_factor = 640;  // Field of view factor

void setup(void) {
    // Allocate the required memory in bytes to hold the color buffer
    color_buffer =
        (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);

    // Create an SDL texture to display the color
    color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             window_width, window_height);

    int point_count = 0;

    // Start loading array of vectors
    // From -1 to 1 (in the 9x9x9 cube)
    for (float x = -1; x <= 1; x += 0.25) {
        for (float y = -1; y <= 1; y += 0.25) {
            for (float z = -1; z <= 1; z += 0.25) {
                // The dot syntax assigns to the struct elements.
                vec3_t new_point = {.x = x, .y = y, .z = z};
                cube_points[point_count++] = new_point;
            }
        }
    }
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

/**
 * Project 3D info on a screen (2D).
 *
 * Take a 3D vector and return a 2D vector.
 */
vec2_t project(vec3_t point) {
    vec2_t projected_point = {.x = (fov_factor * point.x) / point.z,
                              .y = (fov_factor * point.y) / point.z};
    return projected_point;
}

void update(void) {
    // This locks the execution to match the desired FPS.
    int time_to_wait =
        FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);

    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }

    previous_frame_time = SDL_GetTicks();

    cube_rotation.x += 0.002;
    cube_rotation.y += 0.01;
    cube_rotation.z += 0.004;

    for (int i = 0; i < N_POINTS; i++) {
        vec3_t point = cube_points[i];

        vec3_t transformed_point = vec3_rotate_x(point, cube_rotation.x);
        transformed_point = vec3_rotate_y(transformed_point, cube_rotation.y);
        transformed_point = vec3_rotate_z(transformed_point, cube_rotation.z);

        // Translate the points away from the camera
        transformed_point.z -= camera_position.z;

        // Project the current point
        vec2_t projected_point = project(transformed_point);

        // Save the projected 2D vector
        projected_points[i] = projected_point;
    }
}

void render(void) {
    draw_grid(10);

    for (int i = 0; i < N_POINTS; i++) {
        vec2_t projected_point = projected_points[i];
        draw_rect(projected_point.x + window_width / 2,
                  projected_point.y + window_height / 2, 4, 4, 0xFFFFFF00);
    }

    // draw_rect(200, 200, 100, 100, 0xFF00FF33);  /* Apple IIc green */
    // draw_rect(600, 400, 150, 150, 0xFFF1C232);  /* Pikuma yellow */
    // draw_rect(900, 100, 60, 150, 0xFF000F89);   /* phthalo blue */
    // draw_rect(1200, 600, 150, 100, 0xFFE32636); /* alizarin crimson */
    // for (int i = 0; i < 600; i++) {
    //     draw_pixel(i + 10, i + 30, 0xFFFFFF00);
    // }

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
