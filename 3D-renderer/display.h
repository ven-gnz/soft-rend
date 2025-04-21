#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include <stdio.h>

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* color_buffer_texture;


extern uint32_t* color_buffer;
extern bool is_fullscreen;
extern int window_width;
extern int window_height;


bool initialize_window(void);
void destroy_window(void);

void render_color_buffer(void);
void clear_color_buffer(uint32_t color);

void draw_pixel(int x, int y, uint32_t color);
void draw_bg_grid();
void draw_rect(int top_left_x, int top_y, int width, int height, uint32_t color);


