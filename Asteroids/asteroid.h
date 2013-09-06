#include "geometry.h"
#include "icosahedron.h"

class Asteroid : public Icosahedron {
public:
	Asteroid(int levels, float start, float rate);
};