#include "triangle.h"
#include "display.h"
#include "swap.h"



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


vec3_t barycentric_weights(vec2_t a, vec2_t b, vec2_t c, vec2_t p)
{
	vec2_t ac = vec2_sub(c, a);
	vec2_t ab = vec2_sub(b, a);
	vec2_t ap = vec2_sub(p, a);
	vec2_t pc = vec2_sub(c, p);
	vec2_t pb = vec2_sub(b, p);


	float area_parallelogram = (ac.x * ab.y - ac.y * ab.x); // AC x AB
	float alpha = (pc.x * pb.y - pc.y * pb.x) / area_parallelogram;
	float beta = (ac.x * ap.y - ac.y * ap.x) / area_parallelogram;
	float gamma = 1 - alpha - beta;

	vec3_t weights = { alpha, beta, gamma };
	return weights;

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


void draw_texel(
	int x, int y, uint32_t* texture,
	vec4_t point_a, vec4_t point_b, vec4_t point_c,
	tex2_t a_uv, tex2_t b_uv, tex2_t c_uv
) {
	vec2_t p = { x, y };
	vec2_t a = vec2_from_vec4(point_a);
	vec2_t b = vec2_from_vec4(point_b);
	vec2_t c = vec2_from_vec4(point_c);

	// Calculate the barycentric coordinates of our point 'p' inside the triangle
	vec3_t weights = barycentric_weights(a, b, c, p);

	float alpha = weights.x;
	float beta = weights.y;
	float gamma = weights.z;

	// Variables to store the interpolated values of U, V, and also 1/w for the current pixel
	float interpolated_u;
	float interpolated_v;
	float interpolated_reciprocal_w;

	// Perform the interpolation of all U/w and V/w values using barycentric weights and a factor of 1/w
	interpolated_u = (a_uv.u / point_a.w) * alpha + (b_uv.u / point_b.w) * beta + (c_uv.u / point_c.w) * gamma;
	interpolated_v = (a_uv.v / point_a.w) * alpha + (b_uv.v / point_b.w) * beta + (c_uv.v / point_c.w) * gamma;

	// Also interpolate the value of 1/w for the current pixel
	interpolated_reciprocal_w = (1 / point_a.w) * alpha + (1 / point_b.w) * beta + (1 / point_c.w) * gamma;

	// divide back both interpolated values by 1/w
	interpolated_u /= interpolated_reciprocal_w;
	interpolated_v /= interpolated_reciprocal_w;

	// Map the UV coordinate to the full texture width and height
	int tex_x = abs((int)(interpolated_u * texture_width));
	int tex_y = abs((int)(interpolated_v * texture_height));
	int tex_index = ((texture_width * tex_y) + tex_x) % 4096;
	draw_pixel(x, y, texture[tex_index]);
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
	// Guards against division by zero on the slopes

	// Edge case 1 : if the original triangle has a flat bottom, draw only flat bottom triangle
	if (y1 == y2) {
		fill_flat_bottom_triangle(x0, y0, x1, y1, x2, y2, color);
	}
	// Edge case 2 : if the original triangle has a flat top, draw only flat top triangle
	else if (y0 == y1)
	{
		fill_flat_top_triangle(x0, y0, x1, y1, x2, y2, color);
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

}

	void draw_textured_triangle(int x0, int y0, float z0, float w0, float u0, float v0,
								int x1, int y1, float z1, float w1, float u1, float v1,
								int x2, int y2, float z2, float w2, float u2, float v2,
								uint32_t* texture)
	{

		// sort vertices : (y0 < y1 < y2)

		if (y0 > y1)
		{
			int_swap(&y0, &y1);
			int_swap(&x0, &x1);
			float_swap(&z0, &z1);
			float_swap(&w0, &w1);
			float_swap(&u0, &u1);
			float_swap(&v0, &v1);
		}
		if (y1 > y2)
		{
			int_swap(&y1, &y2);
			int_swap(&x1, &x2);
			float_swap(&z1, &z2);
			float_swap(&w1, &w2);
			float_swap(&u1, &u2);
			float_swap(&v1, &v2);
		}
		if (y0 > y1) {
			int_swap(&y0, &y1);
			int_swap(&x0, &x1);
			float_swap(&z0, &z1);
			float_swap(&w0, &w1);
			float_swap(&u0, &u1);
			float_swap(&v0, &v1);
		}

		vec4_t point_a = { x0, y0, z0, w0};
		vec4_t point_b = { x1, y1, z1, w1};
		vec4_t point_c = { x2, y2, z2, w2};

		tex2_t a_uv = { u0,v0};
		tex2_t b_uv = { u1,v1 };
		tex2_t c_uv = { u2,v2 };


		// flat bottom triangle
		float inverse_slope1 = 0;
		float inverse_slope2 = 0;

		if(y1-y0 != 0) inverse_slope1 = (float)(x1 - x0) / abs(y1 - y0);
		if(y2-y0 != 0) inverse_slope2 = (float)(x2 - x0) / abs(y2 - y0);

		if (y1 - y0 != 0) 
		{
			for (int y = y0; y <= y1; y++)
			{
				int x_start = x1 + (y - y1) * inverse_slope1;
				int x_end = x0 + (y - y0) * inverse_slope2;
				if (x_end < x_start) {
					int_swap(&x_start, &x_end);
				}
				for (int x = x_start; x < x_end; x++)
				{
					draw_texel(x, y, texture, point_a, point_b, point_c, a_uv, b_uv, c_uv);
				}

			}
		}

		// flat top triangle
		inverse_slope1 = 0;
		inverse_slope2 = 0;

		if (y2 - y1 != 0) inverse_slope1 = (float)(x2 - x1) / abs(y2 - y1);
		if (y2 - y0 != 0) inverse_slope2 = (float)(x2 - x0) / abs(y2 - y0);

		if (y2 - y1 != 0) {
			for (int y = y1; y <= y2; y++) {
				int x_start = x1 + (y - y1) * inverse_slope1;
				int x_end = x0 + (y - y0) * inverse_slope2;

				if (x_end < x_start) {
					int_swap(&x_start, &x_end); // swap if x_start is to the right of x_end
				}

				for (int x = x_start; x < x_end; x++) {
					// Draw our pixel with the color that comes from the texture
					draw_texel(x, y, texture, point_a, point_b, point_c, a_uv, b_uv, c_uv);
				}
			}
		}
	}
	

	


	