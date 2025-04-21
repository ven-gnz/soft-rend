#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "display.h"
#include "vector.h"
#define N_POINTS (9*9*9)



bool is_running = false;

vec3_t cube_points[N_POINTS];

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
                vec3_t new_point = { x,y,z };
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

void update(void) {
   
}


void render(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    clear_color_buffer(0x00000000);

    draw_bg_grid();
    draw_rect(300, 200, 300, 100, 0xFFFF0000);

    draw_pixel(0, 0, 0xFFFFFFFF);
    render_color_buffer();

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
