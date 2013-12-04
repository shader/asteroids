#pragma once
#include <glm/glm.hpp>

using namespace glm;

struct System {
	vec4 start_color;
	vec4 end_color;
	float age;
	float duration;
};

struct Particle {
	vec3 position, velocity;
	float age;
};