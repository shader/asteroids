#pragma once
#include "headers.h"

using namespace glm;

mat4 translation(vec4 position) {
	mat4 trans = mat4(1.0);
	trans[3] = position;
	return trans;
}

mat4 translation(vec3 position) {
	return translation(vec4(position.x, position.y, position.z, 1.0));
}

float rand(float min, float max) {
	return min + (max - min)*rand() / (float)RAND_MAX;
}

const float TAU = 6.283185307179586476925286766559f;

vec3 rand_vec3() {
	float z = rand(-1.0f, 1.0f);
    float a = rand(0.0f, TAU);

    float r = sqrtf(1.0f - z*z);

    float x = r * cosf(a);
    float y = r * sinf(a);

	return vec3(x,y,z);
}