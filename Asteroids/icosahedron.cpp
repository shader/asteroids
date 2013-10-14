#include "headers.h"

Icosahedron::Icosahedron() : Mesh() {
	static float x = .525731112119133606f;
	static float z = .850650808352039932f;

	vertices.emplace_back(Vertex(-x, 0.0f, z));
	vertices.emplace_back(Vertex(x, 0.0, z));
	vertices.emplace_back(Vertex(-x, 0.0, -z));
	vertices.emplace_back(Vertex(x, 0.0, -z));
	vertices.emplace_back(Vertex(0.0, z, x));
	vertices.emplace_back(Vertex(0.0, z, -x));
	vertices.emplace_back(Vertex(0.0, -z, x));
	vertices.emplace_back(Vertex(0.0, -z, -x));
	vertices.emplace_back(Vertex(z, x, 0.0));
	vertices.emplace_back(Vertex(-z, x, 0.0));
	vertices.emplace_back(Vertex(z, -x, 0.0));
	vertices.emplace_back(Vertex(-z, -x, 0.0));
	 
	uint indices[20][3] = {
		{1, 4, 0}, {4, 9, 0}, {4, 5, 9}, {8, 5, 4},	{1, 8, 4}, 
		{1, 10, 8}, {10, 3, 8}, {8, 3, 5}, {3, 2, 5}, {3, 7, 2}, 
		{3, 10, 7}, {10, 6, 7}, {6, 11, 7}, {6, 0, 11},	{6, 1, 0}, 
		{10, 1, 6}, {11, 0, 9}, {2, 11, 9}, {5, 2, 9}, {11, 2, 7}, 
	};
	
	LoadTriangles(&indices[0][0], 20);
}