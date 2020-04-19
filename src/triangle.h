#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"

// Stores the vertex indices
typedef struct {
    int a;
    int b;
    int c;
} face_t;

// Stores the vec2 points of the triangle on the screen
typedef struct { vec2_t points[3]; } triangle_t;

#endif
