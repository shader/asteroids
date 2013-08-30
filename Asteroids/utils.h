#pragma once
#include "headers.h"

using namespace glm;

mat4 translation(vec3 position);
mat4 translation(vec4 position);
float rand(float min, float max);
vec3 rand_vec3();

vec3 perturb(vec3 point, float max_radius);
void destructive_perturb(vec3* out_point, float max_radius);