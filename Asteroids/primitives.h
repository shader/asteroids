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

class Ship : public Mesh {
public:
	Ship();
};

class AlienShip : public Ship {
public:
	AlienShip();
};