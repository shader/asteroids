#pragma once
#include "utils.h"
using namespace glm;

bool operator<(const vec3 &lhs, const vec3 &rhs) {
	return lhs.x < rhs.x && lhs.y < rhs.y && lhs.z < rhs.z;
}

bool operator<=(const vec3 &lhs, const vec3 &rhs) {
	return lhs < rhs || lhs == rhs;
}

bool operator>(const vec3 &lhs, const vec3 &rhs) {
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

bool operator>=(const vec3 &lhs, const vec3 &rhs) {
	return lhs > rhs || lhs == rhs;
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

quat operator*=(quat &lhs, const quat &rhs) {
	return lhs = lhs * rhs;
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

int rand(int min, int max) {
	return min + (rand() % (max-min));
}

float rand(float min, float max) {
	return min + (max - min)*rand() / (float)RAND_MAX;
}

vec3 rand_ball() { //uniform distribution within a ball
	vec3 v = vec3(0);
	do {
		v.x = rand(-1.0f, 1.0f);
		v.y = rand(-1.0f, 1.0f);
		v.z = rand(-1.0f, 1.0f);
	} while (length(v) > 1.0f); //filter by length - simpler than doing trig
	return v;
}

vec3 rand_vec3() { //random unit 3d vector
	float z = rand(-1.0f, 1.0f);
    float a = rand(0.0f, TAU);

    float r = sqrtf(1.0f - z*z);

    float x = r * cosf(a);
    float y = r * sinf(a);

	return vec3(x,y,z);
}

vec2 rand_vec2() { // random unit 2d vector
	float a = rand(0.0f, TAU);
	float x = cosf(a);
	float y = sinf(a);
	return vec2(x,y);
}

vec3 perturb(vec3 point, float max_radius) {
	float r = rand(0.0f, max_radius);
	vec3 dir = rand_vec3();

	return point + r*dir;
}

void destructive_perturb(vec3* out_point, float max_radius) {
	*out_point = perturb(*out_point, max_radius);
}

quat axis_angle(vec3 axis, float angle) {
	float s = (float)sin(angle/2);
	return quat(axis.x * s, axis.y * s, axis.z * s, cos(angle/2));
}