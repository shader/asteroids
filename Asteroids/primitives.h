#pragma once
#include "model.h"

class Icosahedron : public Mesh {
public:
	Icosahedron();
};

class Quad : public Mesh {
public:
	Quad();
};

class Rock : public Icosahedron {
public:
	Rock(int levels, float start, float rate);
};

class Sphere : public Icosahedron {
public:
	Sphere(int smoothness = 3);
};

class Ship : public Mesh {
public:
	Ship();
};

class AlienShip : public Ship {
public:
	AlienShip();
};

class LineBox {
public:
	LineBox();
	~LineBox();
	vec3 lower, upper;
	void Bind();
	void Draw(mat4 model, mat4 projection);
private:
	GLuint verticesID, indicesID, vertex_array;
};