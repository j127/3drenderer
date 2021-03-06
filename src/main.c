#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "array.h"
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

// An array of triangles that should be rendered frame by frame.
triangle_t* triangles_to_render = NULL;

vec3_t camera_position = {.x = 0, .y = 0, .z = 0};

float fov_factor = 640;  // Field of view factor

void setup(void) {
    render_method = RENDER_WIRE;
    cull_method = CULL_BACKFACE;

    // Allocate the required memory in bytes to hold the color buffer
    color_buffer =
        (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);

    // Create an SDL texture to display the color
    color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             window_width, window_height);

    load_cube_mesh_data();
    // load_obj_file_data("./assets/f22.obj");
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
            // Also quit on q
            if (event.key.keysym.sym == SDLK_q) is_running = false;
            if (event.key.keysym.sym == SDLK_1)
                render_method = RENDER_WIRE_VERTEX;
            if (event.key.keysym.sym == SDLK_2) render_method = RENDER_WIRE;
            if (event.key.keysym.sym == SDLK_3)
                render_method = RENDER_FILL_TRIANGLE;
            if (event.key.keysym.sym == SDLK_4)
                render_method = RENDER_FILL_TRIANGLE_WIRE;
            if (event.key.keysym.sym == SDLK_c) cull_method = CULL_BACKFACE;
            if (event.key.keysym.sym == SDLK_d) cull_method = CULL_NONE;
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

    // Initialize the array of triangles to render
    triangles_to_render = NULL;  // reset

    mesh.rotation.x += 0.01;
    mesh.rotation.y += 0.005;
    mesh.rotation.z += 0.0001;

    // Loop over all the triangle faces of the mesh
    int num_faces = array_length(mesh.faces);
    for (int i = 0; i < num_faces; i++) {
        face_t mesh_face = mesh.faces[i];

        vec3_t face_vertices[3];
        face_vertices[0] = mesh.vertices[mesh_face.a - 1];
        face_vertices[1] = mesh.vertices[mesh_face.b - 1];
        face_vertices[2] = mesh.vertices[mesh_face.c - 1];

        vec3_t transformed_vertices[3];

        // Loop over the vertices and apply transformations
        for (int j = 0; j < 3; j++) {
            vec3_t transformed_vertex = face_vertices[j];

            transformed_vertex =
                vec3_rotate_x(transformed_vertex, mesh.rotation.x);
            transformed_vertex =
                vec3_rotate_y(transformed_vertex, mesh.rotation.y);
            transformed_vertex =
                vec3_rotate_z(transformed_vertex, mesh.rotation.z);

            // Translate the vertex away from the camera
            transformed_vertex.z += 5;

            // Save the transformed_vertex in the array of transformed_vertices
            transformed_vertices[j] = transformed_vertex;
        }

        // Backface culling
        if (cull_method == CULL_BACKFACE) {
            vec3_t vector_a = transformed_vertices[0]; /*   A   */
            vec3_t vector_b = transformed_vertices[1]; /*  / \  */
            vec3_t vector_c = transformed_vertices[2]; /* B---C */

            // Get the vector subtraction of A-B and A-C, then normalize them.
            vec3_t vector_ab = vec3_sub(vector_a, vector_b);
            vec3_t vector_ac = vec3_sub(vector_a, vector_c);
            vec3_normalize(&vector_ab);
            vec3_normalize(&vector_ac);

            // Compute the face normal using the cross product to find the
            // perpendicular vector. Then normalize it.
            vec3_t normal = vec3_cross(vector_ab, vector_ac);
            vec3_normalize(&normal);

            // Find the vector between point a and the camera position
            vec3_t camera_ray = vec3_sub(camera_position, vector_a);

            // Calculate alignment between camera ray and face normal using dot
            // product
            float dot_normal_camera = vec3_dot(normal, camera_ray);

            // Don't render the face if the dot product is less than zero.
            if (dot_normal_camera < 0) {
                continue;
            }
        }

        vec2_t projected_points[3];

        // Loop over the three vertices to perform the projection
        for (int j = 0; j < 3; j++) {
            // Project the current vertex
            projected_points[j] = project(transformed_vertices[j]);

            // Scale and translate the projected points to the middle of the
            // screen
            projected_points[j].x += (window_width / 2);
            projected_points[j].y += (window_height / 2);
        }

        triangle_t projected_triangle = {
            .points = {{projected_points[0].x, projected_points[0].y},
                       {projected_points[1].x, projected_points[1].y},
                       {projected_points[2].x, projected_points[2].y}},
            .color = mesh_face.color};

        // Save the projected triangle in the array of triangles to render
        array_push(triangles_to_render, projected_triangle);
    }
}

void render(void) {
    draw_grid(10);

    // draw_filled_triangle(300, 100, 50, 400, 500, 700, 0xFF00FF00);

    int num_triangles = array_length(triangles_to_render);
    for (int i = 0; i < num_triangles; i++) {
        triangle_t triangle = triangles_to_render[i];

        if (render_method == RENDER_FILL_TRIANGLE ||
            render_method == RENDER_FILL_TRIANGLE_WIRE) {
            draw_filled_triangle(triangle.points[0].x, triangle.points[0].y,
                                 triangle.points[1].x, triangle.points[1].y,
                                 triangle.points[2].x, triangle.points[2].y,
                                 triangle.color);
        }

        if (render_method == RENDER_WIRE ||
            render_method == RENDER_WIRE_VERTEX ||
            render_method == RENDER_FILL_TRIANGLE_WIRE) {
            draw_triangle(triangle.points[0].x, triangle.points[0].y,
                          triangle.points[1].x, triangle.points[1].y,
                          triangle.points[2].x, triangle.points[2].y,
                          0xFFFFFFFF);
        }

        uint32_t point_color = 0xFFFFB000;  // amber
        if (render_method == RENDER_WIRE_VERTEX) {
            draw_rect(triangle.points[0].x - 3, triangle.points[0].y - 3, 6, 6,
                      point_color);
            draw_rect(triangle.points[1].x - 3, triangle.points[1].y - 3, 6, 6,
                      point_color);
            draw_rect(triangle.points[2].x - 3, triangle.points[2].y - 3, 6, 6,
                      point_color);
        }
    }

    // Clear the array of triangles to render every frame loop
    array_free(triangles_to_render);

    render_color_buffer();
    clear_color_buffer(0xFF000000);

    SDL_RenderPresent(renderer);
}

// Free the memory
void free_resources(void) {
    free(color_buffer);
    array_free(mesh.faces);
    array_free(mesh.vertices);
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
    free_resources();

    return 0;
}
