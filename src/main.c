#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "display.h"
#include "mesh.h"
#include "vector.h"

///////////////////////
// Palette Reference //
///////////////////////
// 0xFF00FF33  Apple IIc green
// 0xFF33FF33  Apple II green
// 0xFFFFB000  amber monitor
// 0xFFF1C232  Pikuma yellow
// 0xFF000F89  phthalo blue
// 0xFFE32636  alizarin crimson

bool is_running = false;
int previous_frame_time = 0;  // This will be milliseconds.

triangle_t triangles_to_render[N_MESH_FACES];

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

    // Loop over all the triangle faces of the mesh
    for (int i = 0; i < N_MESH_FACES; i++) {
        face_t mesh_face = mesh_faces[i];

        vec3_t face_vertices[3];
        face_vertices[0] = mesh_vertices[mesh_face.a - 1];
        face_vertices[1] = mesh_vertices[mesh_face.b - 1];
        face_vertices[2] = mesh_vertices[mesh_face.c - 1];

        triangle_t projected_triangle;

        // Loop over the vertices and apply transformations
        for (int j = 0; j < 3; j++) {
            vec3_t transformed_vertex = face_vertices[j];

            transformed_vertex =
                vec3_rotate_x(transformed_vertex, cube_rotation.x);
            transformed_vertex =
                vec3_rotate_y(transformed_vertex, cube_rotation.y);
            transformed_vertex =
                vec3_rotate_z(transformed_vertex, cube_rotation.z);

            // Translate the vertex away from the camera
            transformed_vertex.z -= camera_position.z;

            // Project the current vertex
            vec2_t projected_point = project(transformed_vertex);

            // Scale and translate the projected points to the middle of the
            // screen
            projected_point.x += (window_width / 2);
            projected_point.y += (window_height / 2);

            projected_triangle.points[j] = projected_point;
        }

        // Save the projected triangle in the array of triangles to render
        triangles_to_render[i] = projected_triangle;
    }
}

void render(void) {
    draw_grid(10);

    uint32_t line_color = 0xFF33FF33;   // green
    uint32_t point_color = 0xFFFFB000;  // amber

    for (int i = 0; i < N_MESH_FACES; i++) {
        triangle_t triangle = triangles_to_render[i];

        draw_triangle(triangle.points[0].x, triangle.points[0].y,
                      triangle.points[1].x, triangle.points[1].y,
                      triangle.points[2].x, triangle.points[2].y, line_color);

        draw_rect(triangle.points[0].x, triangle.points[0].y, 3, 3,
                  point_color);
        draw_rect(triangle.points[1].x, triangle.points[1].y, 3, 3,
                  point_color);
        draw_rect(triangle.points[2].x, triangle.points[2].y, 3, 3,
                  point_color);
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
