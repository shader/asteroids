#include "rock.h"

Rock::Rock(int levels, float start, float rate) : Model() {
	delete mesh;
	mesh = new Icosahedron();

	float max_radius = start;
	for (int i=0; i<levels; i++) {
		if (i%2 == 0) {
			Perturb(max_radius);
			max_radius /= rate;
		} else {
			Subdivide();
		}
	}
	mesh->Normalize();
}