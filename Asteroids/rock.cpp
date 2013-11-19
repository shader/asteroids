#include "rock.h"
#include "content.h"

Rock::Rock(int levels, float start, float rate) : Model(Icosahedron(), basic) {
	float max_radius = start;
	for (int i=0; i<levels; i++) {
		if (i%2 == 0) {
			mesh.Perturb(max_radius);
			max_radius /= rate;
		} else {
			mesh.Subdivide();
		}
	}
	mesh.Normalize();
}