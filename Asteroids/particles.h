#pragma once
#include <glm/glm.hpp>

using namespace glm;

struct Particle {
	vec3 position, velocity;
	float age;
};