#include "triangle.h"


void int_swap(int* a, int* b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2)
{
	float inv_slope1 = (x1-x0)/(float)(y1-y0);
	float inv_slope2 = (x1-x2)/(float)(y1-yx);

	for(int y = y0, y <= my)



}

void fill_flat_top_triangle(int x1, int y1, int mx, int my, int x2, int y2)
{

}



void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	// sort vertices by y0 < y1 < y2
	if (y0 > y1) {
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

	//triangle midpoint using triangle similarity
	int my = y1;
	int mx = ((x2 - x0) * (y1 - y0)) / (float)(y2 - y0) + x0;

	fill_flat_bottom_triangle(x0, y0, x1, y1, mx, my);

	fill_flat_top_triangle(x1, y1, mx, my, x2, y2);


};