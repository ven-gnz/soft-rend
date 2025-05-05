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


cull_method = CULL_BACKFACE;
render_method = RENDER_WIRE;
bool is_running = false;
int previous_frame_time = 0;
float fov_factor = 400;


triangle_t* triangles_to_render = NULL;

vec3_t camera_position = { .x = 0,.y = 0, .z = 0 };

const float Z_COORD_OFFSET_DEFAULT = -5;



void setup(void) {

    render_method = RENDER_WIRE;
    cull_method = CULL_BACKFACE;
    load_cube_mesh();

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
        if (event.key.keysym.sym == SDLK_ESCAPE) {
            is_running = false;
            break;
        }
            
        if (event.key.keysym.sym == SDLK_c) {
            cull_method = CULL_BACKFACE;
            printf("Culling on\n");
            break;
            }
        if (event.key.keysym.sym == SDLK_d) {
            cull_method = CULL_NONE;
            printf("Culling off\n");
            break;
        }
        if (event.key.keysym.sym == SDLK_1) {
            render_method = RENDER_WIRE_VERTEX;
            break;
        }
        if (event.key.keysym.sym == SDLK_2) {
            render_method = RENDER_WIRE;
            break;
        }
        if (event.key.keysym.sym == SDLK_3) {
            render_method = RENDER_FILL_TRIANGLE;
            break;
        }
        if (event.key.keysym.sym == SDLK_4) {
            render_method = RENDER_FILL_TRIANGLE_WIRE;
            break;
        }            

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
    mesh.rotation.y += 0.01;
    mesh.rotation.z += 0.01;
    mesh.rotation.x += 0.01;

    for (int i = 0; i < array_length(mesh.faces); i++) {

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

        if (cull_method == CULL_BACKFACE) {
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

            if (dot_cam <= 0) {
                continue;
            }
        }


            vec2_t projected_points[3];
            for (int j = 0; j < 3; j++) {
                projected_points[j] = project(transformed_vertices[j]);
                projected_points[j].x += (window_width / 2);
                projected_points[j].y += (window_height / 2);
            }

            triangle_t projected_triangle = {
                .points = {
                    { projected_points[0].x, projected_points[0].y},
                    { projected_points[1].x, projected_points[1].y},
                    { projected_points[2].x, projected_points[2].y},
            },
                .color = mesh_face.color
            };

            array_push(triangles_to_render, projected_triangle);
        }

        
    }







    









void render(void) {
    
    draw_bg_grid();

    for (int i = 0; i < array_length(triangles_to_render); i++) {
        triangle_t triangle = triangles_to_render[i];

        switch (render_method)
        {
        case RENDER_WIRE:
            draw_triangle(
                triangle.points[0].x,
                triangle.points[0].y,
                triangle.points[1].x,
                triangle.points[1].y,
                triangle.points[2].x,
                triangle.points[2].y,
                triangle.color
            );
            break;
        case RENDER_WIRE_VERTEX:
            draw_triangle(
                triangle.points[0].x,
                triangle.points[0].y,
                triangle.points[1].x,
                triangle.points[1].y,
                triangle.points[2].x,
                triangle.points[2].y,
                triangle.color
            );

            draw_rect(triangle.points[0].x, triangle.points[0].y, 4, 4, 0xFFFF0000);
            draw_rect(triangle.points[1].x, triangle.points[1].y, 4, 4, 0xFFFF0000);
            draw_rect(triangle.points[2].x, triangle.points[2].y, 4, 4, 0xFFFF0000);
            break;
        case RENDER_FILL_TRIANGLE:
            draw_filled_triangle(
                triangle.points[0].x,
                triangle.points[0].y,
                triangle.points[1].x,
                triangle.points[1].y,
                triangle.points[2].x,
                triangle.points[2].y,
                triangle.color
            );
            break;

        case RENDER_FILL_TRIANGLE_WIRE:
            draw_filled_triangle(
                triangle.points[0].x,
                triangle.points[0].y,
                triangle.points[1].x,
                triangle.points[1].y,
                triangle.points[2].x,
                triangle.points[2].y,
                triangle.color
            );
            draw_triangle(
                triangle.points[0].x,
                triangle.points[0].y,
                triangle.points[1].x,
                triangle.points[1].y,
                triangle.points[2].x,
                triangle.points[2].y,
                0x00000000
            );
            break;

        }

        // render loop end
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
