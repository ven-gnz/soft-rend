#pragma once

typedef struct {
	float x;
	float y;
} vec2_t;

typedef struct {
	float x;
	float y;
	float z;
} vec3_t;

vec3_t vec3_rotate_x(vec3_t v, float angle);
vec3_t vec3_rotate_y(vec3_t v, float angle);
vec3_t vec3_rotate_z(vec3_t v, float angle);

float vec2_length(vec2_t v);
float vec3_length(vec3_t v);
vec2_t vec2_add(vec2_t a, vec2_t b);
vec2_t vec2_subtract(vec2_t a, vec2_t b);
vec3_t vec3_add(vec3_t a, vec3_t b);
vec3_t vec3_subtract(vec3_t a, vec3_t b);
vec2_t vec2_scalar(vec2_t, float s);
vec3_t vec3_scalar(vec3_t, float s);
vec2_t vec2_div(vec2_t v, float f);
vec3_t vec3_div(vec3_t v, float f)