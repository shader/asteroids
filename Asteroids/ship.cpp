#include "ship.h"

Ship::Ship() : Model() {
	vec3 points[] = {
		vec3(0.5f, 0, 0), vec3(0,1,0), vec3(0, -0.25, 0.25),
		vec3(0, -0.25, -0.25), vec3(-0.5, 0, 0)
	};

	GLuint triangles[6][3] = {
		{0,1,2}, {0,2,3}, {0,3,1}, {2,1,4}, {3,2,4}, {3,4,1}
	};
	
	for (int i = 0; i<6; i++)
		this->mesh->AddFace(
			points[triangles[i][0]],
			points[triangles[i][1]],
			points[triangles[i][2]]);
}