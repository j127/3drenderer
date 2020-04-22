#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdint.h>
#include "vector.h"

// Stores the vertex indices
typedef struct {
    int a;
    int b;
    int c;
    uint32_t color;
} face_t;

// Stores the vec2 points of the triangle on the screen
typedef struct {
    vec2_t points[3];
    uint32_t color;
} triangle_t;

void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2,
                          uint32_t color);

#endif
