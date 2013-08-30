#include <glm/glm.hpp>
#include "utils.h"
using namespace glm;

vec3 perturb(vec3 point, float max_radius) {
	float r = rand(0, max_radius);
	vec3 dir = rand_vec3();

	return point + r*dir;
}

void destructive_perturb(vec3* out_point, float max_radius) {
	*out_point = perturb(*out_point, max_radius);
}