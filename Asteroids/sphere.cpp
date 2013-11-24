#include "model.h"
#include "content.h"

Sphere::Sphere(int smoothness) : Icosahedron() {
	Subdivide(smoothness);
	Normalize();
}