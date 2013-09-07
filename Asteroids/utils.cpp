#pragma once
#include "utils.h"
using namespace glm;

bool operator<(const vec3 &lhs, const vec3 &rhs) {
	return lhs.x < rhs.x ||
		(lhs.x == rhs.x && (lhs.y < rhs.y ||
			(lhs.y == rhs.y && lhs.z < rhs.z)));
}

vec3 operator*(const vec3 &lhs, const float &rhs) {
	return vec3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
}
vec3 operator*(const float &lhs, const vec3 &rhs) {
	return rhs * lhs;
}
vec3 operator*(const vec3 &lhs, const double &rhs) {
	return vec3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
}
vec3 operator*(const double &lhs, const vec3 &rhs) {
	return rhs * lhs;
}

mat4 translation(vec4 position) {
	mat4 trans = mat4(1.0);
	trans[3] = position;
	return trans;
}

mat4 translation(vec3 position) {
	return translation(vec4(position.x, position.y, position.z, 1.0));
}

mat4 scale(vec3 size) {
	return glm::scale(mat4(1.0), size);

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

vec3 perturb(vec3 point, float max_radius) {
	float r = rand(0, max_radius);
	vec3 dir = rand_vec3();

	return point + r*dir;
}

void destructive_perturb(vec3* out_point, float max_radius) {
	*out_point = perturb(*out_point, max_radius);
}