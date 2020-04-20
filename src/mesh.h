#ifndef MESH_H
#define MESH_H

#include "triangle.h"
#include "vector.h"

#define N_CUBE_VERTICES 8
extern vec3_t cube_vertices[N_CUBE_VERTICES];

#define N_CUBE_FACES (6 * 2)  // 6 faces with 2 triangles each
extern face_t cube_faces[N_CUBE_FACES];

// A struct for dynamic sized meshes
typedef struct {
    vec3_t* vertices;  // dynamic array of vertices
    face_t* faces;     // dynamic array of faces
    vec3_t rotation;   // rotation with x, y, and z values
} mesh_t;

extern mesh_t mesh;

void load_cube_mesh_data(void);

#endif
