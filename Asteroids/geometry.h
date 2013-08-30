#include <glm/glm.hpp>
using namespace glm;

vec3 perturb(vec3 point, float max_radius);
void destructive_perturb(vec3* out_point, float max_radius);