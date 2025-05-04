#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "array.h"
#include <SDL_filesystem.h>
#define _CRT_SECURE_NO_WARNINGS 1


bool is_running = false;
int previous_frame_time = 0;
float fov_factor = 400; 


triangle_t* triangles_to_render = NULL;

vec3_t camera_position = { .x = 0,.y = 0, .z = 0 };

const float Z_COORD_OFFSET_DEFAULT = -5;



void setup(void) {

    load_obj_mesh("f22.obj");

    color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);

    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        window_width,
        window_height
    );

   
}



void process_input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
    case SDL_QUIT:
        is_running = false;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
            is_running = false;
        break;
    }
}



vec2_t project(vec3_t point) {

    vec2_t projected_point =
    {
        .x = (fov_factor * point.x) / point.z,
        .y = (fov_factor * point.y) / point.z
    };
    return projected_point;
}



void update(void) {
    
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
    
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }

   
    triangles_to_render = NULL;

    previous_frame_time = SDL_GetTicks();
    mesh.rotation.y += 0.1;
    mesh.rotation.z += 0.1;
    mesh.rotation.x += 0.1;

    for (int i = 0; i < array_length(mesh.faces); i++) {
        triangle_t projected_triangle;
        face_t mesh_face = mesh.faces[i];
        vec3_t face_vertices[3];

        
        face_vertices[0] = mesh.vertices[mesh_face.a - 1];
        face_vertices[1] = mesh.vertices[mesh_face.b - 1];
        face_vertices[2] = mesh.vertices[mesh_face.c - 1];

        vec3_t transformed_vertices[3];
        // transforming vertices
        for (int j = 0; j < 3; j++) {
            vec3_t transformed_vertex = face_vertices[j];
            transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
            transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

            transformed_vertex.z += Z_COORD_OFFSET_DEFAULT;

            transformed_vertices[j] = transformed_vertex;

        }
        //culling faces
        vec3_t vector_a = transformed_vertices[0]; /*   A    */
        vec3_t vector_b = transformed_vertices[1]; /*  / \   */
        vec3_t vector_c = transformed_vertices[2]; /* C---B  */


        vec3_t vector_ab = vec3_subtract(vector_b, vector_a);
        vec3_t vector_ac = vec3_subtract(vector_c, vector_a);
        vec3_normalize(&vector_ab);
        vec3_normalize(&vector_ac);


        vec3_t normal = cross(vector_ab, vector_ac); // LEFT HANDED COORDINATE SYSTEM
        vec3_normalize(&normal);

        vec3_t camera_ray = vec3_subtract(camera_position, vector_a);

        float dot_cam = vec3_dot(camera_ray, normal);
      
        if (dot_cam < 0) {
            continue;
        }


        //projecting faces
        for (int j = 0; j < 3; j++) {

            vec2_t projected_point = project(transformed_vertices[j]);

            // scale and translate projected point towards middle of screen
            projected_point.x += (window_width / 2);
            projected_point.y += (window_height / 2);

            projected_triangle.points[j] = projected_point;


        }
        array_push(triangles_to_render, projected_triangle);
            
        }
  }






void render(void) {
    
    draw_bg_grid();

    for (int i = 0; i < array_length(triangles_to_render); i++) {
        triangle_t triangle = triangles_to_render[i];

        draw_filled_triangle(
            triangle.points[0].x,
            triangle.points[0].y,
            triangle.points[1].x,
            triangle.points[1].y,
            triangle.points[2].x,
            triangle.points[2].y,
            0xFFFFFFFF
            );

    }


    array_free(triangles_to_render);
    
    render_color_buffer();
    clear_color_buffer(0x00000000);
    SDL_RenderPresent(renderer);
}

void free_res(void) {
    free(color_buffer);
    array_free(mesh.faces);
    array_free(mesh.vertices);
}




int main(int argc, char **argv) {
    
  
    is_running = initialize_window();
    setup();

    while (is_running) {
        process_input();
        update();
        render();
    }

    destroy_window();
    free_res();
    return 0;
}
