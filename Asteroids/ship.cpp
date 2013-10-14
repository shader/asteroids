#include "ship.h"

Ship::Ship() : Model() {
	mesh->vertices.emplace_back(Vertex(0.5f, 0, 0));
	mesh->vertices.emplace_back(Vertex(0,1,0));
	mesh->vertices.emplace_back(Vertex(0, -0.25, 0.25));
	mesh->vertices.emplace_back(Vertex(0, -0.25, -0.25));
	mesh->vertices.emplace_back(Vertex(-0.5, 0, 0));

	uint indices[6][3] = {
		{0,1,2}, {0,2,3}, {0,3,1}, {2,1,4}, {3,2,4}, {3,4,1}
	};
	
	mesh->LoadTriangles(&indices[0][0], 6);
}