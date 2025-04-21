#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "display.h"
#include "vector.h"
#define N_POINTS (9*9*9)


bool is_running = false;
float fov_factor = 400; //debug

vec3_t camera_position = { .x = 0,.y = 0, .z = -5 };

vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];

void setup(void) {

    color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);

    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        window_width,
        window_height
    );


    int point_count = 0;
    for (float x = -1; x <= 1; x += 0.25) {
        for (float y = -1; y <= 1; y += 0.25) {
            for (float z = -1; z <= 1; z += 0.25) {
                vec3_t new_point = { .x = x,.y = y,.z = z };
                cube_points[point_count++] = new_point;
            }
        }
    }
   
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

vec2_t ortho_project(vec3_t point) {
    vec2_t projected_point =
    {
        .x = point.x,
        .y = point.y
    };
     return projected_point;
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
    for (int i = 0; i < N_POINTS; i++) {
        vec3_t point = cube_points[i];


        point.z -= camera_position.z;

        vec2_t projected_point = project(point);
        projected_points[i] = projected_point;
    }
   
}


void render(void) {
    
    
    draw_bg_grid();

    for (int i = 0; i < N_POINTS; i++) {
        vec2_t projected_point = projected_points[i];

        draw_rect(
            projected_point.x + (window_width / 2),
            projected_point.y + (window_height / 2),
            4,
            4,
            0xFFFFFF00);
    }
    
    
    render_color_buffer();
    clear_color_buffer(0x00000000);
    SDL_RenderPresent(renderer);
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

    return 0;
}
