#include "primitives.h"

Quad::Quad() : Mesh() {
	vertices.emplace_back(Vertex(0.5, -0.5, 0, this));
	vertices.emplace_back(Vertex(0.5, 0.5, 0, this));
	vertices.emplace_back(Vertex(-0.5, -0.5, 0, this));
	vertices.emplace_back(Vertex(0.5, -0.5, 0, this));
 
	uint indices[2][3] = {
		{0, 2, 1}, {1, 2, 3},
	};
	
	LoadTriangles(&indices[0][0], 2);
}