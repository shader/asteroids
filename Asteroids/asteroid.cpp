#include "asteroid.h"

Asteroid::Asteroid(int levels, float start, float rate) : Icosahedron() {
	float max_radius = start;
	for (int i=0; i<levels; i++) {
		if (i%2 == 0) {
			Perturb(max_radius);
			max_radius /= rate;
		} else {
			Subdivide();
		}
	}
}