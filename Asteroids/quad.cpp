#include "primitives.h"

Quad::Quad() : Mesh() {
	vec3 points[] = {
		vec3(0.5, -0.5, 0), vec3(0.5, 0.5, 0),
		vec3(-0.5, -0.5, 0), vec3(0.5, -0.5, 0)
	};
 
	GLuint triangles[2][3] = {
		{0, 2, 1}, {1, 2, 3},
	};

	for (int i = 0; i<2; i++)
		this->AddFace(
			points[triangles[i][0]],
			points[triangles[i][1]],
			points[triangles[i][2]]);
}