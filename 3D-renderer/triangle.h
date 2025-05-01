#pragma once
#include "vector.h"
#include <stdint.h>

typedef struct {
	int a;
	int b;
	int c;
} face_t;

typedef struct {
	vec2_t points[3];
} triangle_t;

void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int mx, int my);
void fill_flat_top_triangle(int x1, int y1, int mx, int my, int x2, int y2);

void int_swap(int* a, int* b);
void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);