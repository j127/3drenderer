#include "vector.h"
#include <math.h>

//////////////////////
// 2D Vector Functions
//////////////////////

/**
 * Get the length of a vector using the Pythagorean theorem
 *
 *     (x,y)
 *      /|
 *   c / | b
 *    /  |
 *   /___|
 *     a
 */
float vec2_length(vec2_t v) { return sqrt(v.x * v.x + v.y * v.y); }

/**
 * Add two 2D vectors
 */
vec2_t vec2_add(vec2_t a, vec2_t b) {
    vec2_t result = {.x = a.x + b.x, .y = a.y + b.y};
    return result;
}

/**
 * Subtract two 2D vectors
 */
vec2_t vec2_sub(vec2_t a, vec2_t b) {
    vec2_t result = {.x = a.x - b.x, .y = a.y - b.y};
    return result;
}

/**
 * Multiple a 2D vector by a scalar
 */
vec2_t vec2_mul(vec2_t v, float factor) {
    vec2_t result = {.x = v.x * factor, .y = v.y * factor};
    return result;
}

/**
 * Divide a 2D vector by a scalar
 */
vec2_t vec2_div(vec2_t v, float factor) {
    vec2_t result = {.x = v.x / factor, .y = v.y / factor};
    return result;
}

/**
 * 2D dot product
 */
float vec2_dot(vec2_t a, vec2_t b) { return (a.x * b.x) + (a.y * b.y); }

//////////////////////
// 3D Vector Functions
//////////////////////

float vec3_length(vec3_t v) { return sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }

/**
 * Add two 3D vectors
 */
vec3_t vec3_add(vec3_t a, vec3_t b) {
    vec3_t result = {.x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z};
    return result;
}

/**
 * Subtract two 3D vectors
 */
vec3_t vec3_sub(vec3_t a, vec3_t b) {
    vec3_t result = {.x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z};
    return result;
}

/**
 * Multiple a 3D vector by a scalar
 */
vec3_t vec3_mul(vec3_t v, float factor) {
    vec3_t result = {.x = v.x * factor, .y = v.y * factor, .z = v.z * factor};
    return result;
}

/**
 * Divide a 3D vector by a scalar
 */
vec3_t vec3_div(vec3_t v, float factor) {
    vec3_t result = {.x = v.x / factor, .y = v.y / factor, .z = v.z};
    return result;
}

/**
 * Vector cross product
 */
vec3_t vec3_cross(vec3_t a, vec3_t b) {
    vec3_t result = {.x = a.y * b.z - a.z * b.y,
                     .y = a.z * b.x - a.x * b.z,
                     .z = a.x * b.y - a.y * b.x};
    return result;
}

/**
 * 3D dot product
 */
float vec3_dot(vec3_t a, vec3_t b) {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

vec3_t vec3_rotate_x(vec3_t v, float angle) {
    vec3_t rotated_vector = {.x = v.x,
                             .y = v.y * cos(angle) - v.z * sin(angle),
                             .z = v.y * sin(angle) + v.z * cos(angle)};
    return rotated_vector;
};

vec3_t vec3_rotate_y(vec3_t v, float angle) {
    vec3_t rotated_vector = {.x = v.x * cos(angle) - v.z * sin(angle),
                             .y = v.y,
                             .z = v.x * sin(angle) + v.z * cos(angle)};
    return rotated_vector;
};

vec3_t vec3_rotate_z(vec3_t v, float angle) {
    vec3_t rotated_vector = {.x = v.x * cos(angle) - v.y * sin(angle),
                             .y = v.x * sin(angle) + v.y * cos(angle),
                             .z = v.z};
    return rotated_vector;
};
