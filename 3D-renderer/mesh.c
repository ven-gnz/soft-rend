#pragma once
#include "mesh.h"
#include <stdio.h>
#include <string.h>

mesh_t mesh = {
    .vertices = NULL,
    .faces = NULL,
    .rotation = {0,0,0}
};

vec3_t cube_vertices[N_CUBE_VERTICES] = {
    {.x = -1, .y = -1, .z = -1}, // 1
    {.x = -1, .y = 1, .z = -1}, // 2
    {.x = 1,  .y = 1, .z = -1}, // 3
    {.x = 1, .y = -1, .z = -1}, // 4
    {.x = 1, .y = 1, .z = 1}, // 5
    {.x = 1, .y = -1, .z = 1}, // 6
    {.x = -1, .y = 1, .z = 1}, // 7
    {.x = -1, .y = -1, .z = 1}, // 8
};

face_t cube_faces[N_CUBE_FACES] = {
    // front
    {.a = 1, .b = 2, .c = 3, .color = 0xFFFF0000 },
    {.a = 1, .b = 3, .c = 4, .color = 0xFFFF0000 },
    // right
    {.a = 4, .b = 3, .c = 5, .color = 0xFF00FF00 },
    {.a = 4, .b = 5, .c = 6, .color = 0xFF00FF00 },
    // back
    {.a = 7, .b = 5, .c = 6, .color = 0xFF0000FF },
    {.a = 8, .b = 7, .c = 6, .color = 0xFF0000FF },
    // left
    {.a = 2, .b = 7, .c = 8, .color = 0xFFFFFF00 },
    {.a = 1, .b = 2, .c = 8, .color = 0xFFFFFF00 },
    // top
    {.a = 5, .b = 7, .c = 2, .color = 0xFFFF00FF },
    {.a = 3, .b = 5, .c = 2, .color = 0xFFFF00FF },
    // bottom
    {.a = 1, .b = 8, .c = 6, .color = 0xFF0000FF },
    {.a = 4, .b = 1, .c = 6, .color = 0xFF0000FF }
};

void load_cube_mesh(void) {
    for (int i = 0; i < N_CUBE_VERTICES; i++) {
        vec3_t cube_vertex = cube_vertices[i];
        array_push(mesh.vertices, cube_vertex);
    }
    for (int j = 0; j < N_CUBE_FACES; j++) {
        face_t f = cube_faces[j];
        array_push(mesh.faces, f);
    }

}

void load_obj_mesh(char* filename) {

    FILE* file_ptr;

    file_ptr = fopen( filename, "r");
    char line[512];
    if (file_ptr != NULL) {

        while (fgets(line, 512, file_ptr)) {

            if (strncmp(line, "v ", 2) == 0) {
                vec3_t vertex;
                sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
                array_push(mesh.vertices, vertex);
            }

            if (strncmp(line, "f ", 2) == 0) {
                
                int vertex_indices[3];
                int texture_indices[3];
                int normal_indices[3];
                int matches = sscanf(line,
                    "f  %d/%d/%d %d/%d/%d %d/%d/%d",
                    &vertex_indices[0], &texture_indices[0], &normal_indices[0],
                    &vertex_indices[1], &texture_indices[1], &normal_indices[1],
                    &vertex_indices[2], &texture_indices[2], &normal_indices[2]);
                if (matches == 9) {
                    face_t face = {
                    .a = vertex_indices[0],
                    .b = vertex_indices[1],
                    .c = vertex_indices[2]
                    };

                    array_push(mesh.faces, face);
                }
                
                 
            }
           

        }


    }
    else {
        printf("No file!");
    }


}