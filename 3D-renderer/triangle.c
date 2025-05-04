#include "triangle.h"
#include "display.h"


void int_swap(int* a, int* b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	float inv_slope1 = (float) (x1-x0) / (y1-y0);
	float inv_slope2 = (float) (x2-x0)/ (y2-y0);

	float x_start = x0;
	float x_end = x0;
	// Scanline loop from bottom to top
	for (int y = y0; y <= y2; y++)
	{
		draw_line(x_start, y, x_end, y, color);
		x_start += inv_slope1;
		x_end += inv_slope2;
	}
}

void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	float inv_slope1 = (float) (x2-x0) / (y2-y0);
	float inv_slope2 = (float) (x2-x1) / (y2-y1);
	
	float x_start = x2;
	float x_end = x2;
	for (int y = y2; y >= y0; y--) {
		draw_line(x_start, y, x_end, y, color);
		x_start -= inv_slope1;
		x_end -= inv_slope2;
	}
}



void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	// sort vertices by y0 < y1 < y2
	if (y0 > y1) 
	{
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
	}
	if (y1 > y2)
	{
		int_swap(&y1, &y2);
		int_swap(&x1, &x2);
	}
	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
	}
	// Special case : if the original triangle has a flat bottom
	if (y1 == y2) {
		fill_flat_bottom_triangle(x0, y0, x1, y1, x2, y2, color);
	}
	// Special case 2 : if the original triangle has a flat top
	else if (y0 == y1) 
	{
		fill_flat_bottom_triangle(x0, y0, x1, y1, x2, y2, color);
	}
	//Default case : divide triangle to flat top & flat bottom
	// triangle midpoint x computed using triangle similarity
	else
	{
		int my = y1;
		int mx = ((x2 - x0) * (y1 - y0)) / (y2 - y0) + x0;
		fill_flat_bottom_triangle(x0, y0, x1, y1, mx, my, color);
		fill_flat_top_triangle(x1, y1, mx, my, x2, y2, color);

	}
	



};