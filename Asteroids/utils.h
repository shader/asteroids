#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <random>

using namespace glm;

const float TAU = 6.283185307179586476925286766559f;

bool operator<(const vec3 &lsh, const vec3 &rhs);
bool operator<=(const vec3 &lsh, const vec3 &rhs);
bool operator>(const vec3 &lsh, const vec3 &rhs);
bool operator>=(const vec3 &lsh, const vec3 &rhs);

vec3 operator*(const vec3 &lhs, const float &rhs);
vec3 operator*(const float &lhs, const vec3 &rhs);
vec3 operator*(const vec3 &lhs, const double &rhs);
vec3 operator*(const double &lhs, const vec3 &rhs);

quat operator*=(quat &lhs, const quat &rhs);

mat4 translation(vec3 position);
mat4 translation(vec4 position);
mat4 scale(vec3 size);
float rand(float min, float max);
vec3 rand_ball();
vec3 rand_vec3();
vec2 rand_vec2();

vec3 perturb(vec3 point, float max_radius);
void destructive_perturb(vec3* out_point, float max_radius);

quat axis_angle(vec3 axis, float angle);

static std::default_random_engine rand_engine;