#include "primitives.h"

Quad::Quad() : Mesh() {
	vertices.push_back(Vertex(vec3(-1, -1, 0), vec2(0,0), this));
	vertices.push_back(Vertex(vec3(-1, 1, 0), vec2(0,1), this));
	vertices.push_back(Vertex(vec3(1, 1, 0), vec2(1,1), this));
	vertices.push_back(Vertex(vec3(1, -1, 0), vec2(1,0), this));
 
	uint indices[2][3] = {
		{0, 2, 1}, {0, 3, 2},
	};
	
	LoadTriangles(&indices[0][0], 2);
}