#pragma once
#include "geometry.h"
#include "icosahedron.h"

class Rock : public Model {
public:
	Rock(int levels, float start, float rate);
};