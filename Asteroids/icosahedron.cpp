#include "headers.h"

Icosahedron::Icosahedron() {
	static float x = .525731112119133606;
	static float z = .850650808352039932;

	vec3 points[] = {
		vec3(-x, 0.0, z), vec3(x, 0.0, z), vec3(-x, 0.0, -z), vec3(x, 0.0, -z),
		vec3(0.0, z, x), vec3(0.0, z, -x), vec3(0.0, -z, x), vec3(0.0, -z, -x),
		vec3(z, x, 0.0), vec3(-z, x, 0.0), vec3(z, -x, 0.0), vec3(-z, -x, 0.0)
	}; 
	vertices.assign(points, points+12);
 
	Triangle tris[] = {
		{1, 4, 0}, {4, 9, 0}, {4, 5, 9}, {8, 5, 4},	{1, 8, 4}, 
		{1, 10, 8}, {10, 3, 8}, {8, 3, 5}, {3, 2, 5}, {3, 7, 2}, 
		{3, 10, 7}, {10, 6, 7}, {6, 11, 7}, {6, 0, 11},	{6, 1, 0}, 
		{10, 1, 6}, {11, 0, 9}, {2, 11, 9}, {5, 2, 9}, {11, 2, 7}, 
	};
	triangles.assign(tris, tris+20);
}