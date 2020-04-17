# 3drenderer

## Dependencies

This installs GCC and the C dependencies along with SDL2.

```text
$ sudo apt install build-essential
$ sudo apt install libsdl2-dev
```

## Structure

Sample structure:

```text
3drenderer
├── Makefile
└── src
    ├── main.c
    ├── mesh.c
    └── mesh.h
```

## Compiling

The most simple:

```text
$ gcc main.c -o renderer
$ ./renderer
```

See the `Makefile`.

## Examples

### Scalars

- temperature
- area
- length
- pressure

### Vectors

- velocity
- acceleration
- force
- lift
- drag
- displacement

A cube will be a set of vectors represented by points (originating at 0, 0, 0).

2D vector: (6.0, 3.0) -- two components
3D vector: (3.0, 2.0, 3.0) -- three components

```c
// Creating a new data type for 2D vectors
typedef struct {
    float x;
    float y;
} vec2_t; // _t here is a convention for naming the type

vec2_t p0 = { 0.0, 2.0 };
vec2_t p1 = { 3.0, 2.0 };
vec2_t p2 = { 4.5, -2.2 };

// 3D vector
typedef struct {
    float x;
    float y;
    float z;
} vec3_t;

// etc.
```

Example camera:

```c
typedef struct {
    vec3_t position;
    vec3_t rotation;
    vec3_t fov_angle;
} camera_t;

camera_t mycam = {
    { 0, 0, 0 },
    { 0.3, -2.0, 0.0 },
    0.78
};
```

## Projection

[Two main categories](https://en.wikipedia.org/wiki/3D_projection): _parallel_ and _perspective_.

Some branches of the tree:

Parallel → Orthographic → Axonometric → Isometric

Perspective → ...

