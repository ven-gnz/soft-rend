#pragma once
#include "vector.h"
#include "triangle.h"
#include "array.h"
#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6*2)

extern vec3_t cube_vertices[N_CUBE_VERTICES];
extern face_t cube_faces[N_CUBE_FACES];

void load_cube_mesh(void);

typedef struct {
	vec3_t* vertices;
	face_t* faces;
	vec3_t rotation;
} mesh_t;


extern mesh_t mesh; 

