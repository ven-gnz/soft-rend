#pragma once
#include "vector.h"
#include <stdint.h>

typedef struct {
	vec3_t direction;
} light_t;

extern light_t light;

uint32_t shade_func(uint32_t original_color, float percentage_factor);