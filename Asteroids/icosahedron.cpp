#include "headers.h"

Icosahedron::Icosahedron() : Mesh() {
	static float x = .525731112119133606;
	static float z = .850650808352039932;

	Vertex* points[] = {
		new Vertex(-x, 0.0, z), new Vertex(x, 0.0, z), new Vertex(-x, 0.0, -z), new Vertex(x, 0.0, -z),
		new Vertex(0.0, z, x), new Vertex(0.0, z, -x), new Vertex(0.0, -z, x), new Vertex(0.0, -z, -x),
		new Vertex(z, x, 0.0), new Vertex(-z, x, 0.0), new Vertex(z, -x, 0.0), new Vertex(-z, -x, 0.0)
	};
	 
	int indices[20][3] = {
		{1, 4, 0}, {4, 9, 0}, {4, 5, 9}, {8, 5, 4},	{1, 8, 4}, 
		{1, 10, 8}, {10, 3, 8}, {8, 3, 5}, {3, 2, 5}, {3, 7, 2}, 
		{3, 10, 7}, {10, 6, 7}, {6, 11, 7}, {6, 0, 11},	{6, 1, 0}, 
		{10, 1, 6}, {11, 0, 9}, {2, 11, 9}, {5, 2, 9}, {11, 2, 7}, 
	};

	vector<Vertex*> vertices;
	for (int i=0; i<12; i++) vertices.push_back(points[i]);

	vector<int> triangles;
	for (int i=0; i<20; i++) {
		triangles.push_back(indices[i][0]);		
		triangles.push_back(indices[i][1]);		
		triangles.push_back(indices[i][2]);
	}

	LoadTriangles(vertices, triangles);
}