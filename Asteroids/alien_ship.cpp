#include "model.h"

AlienShip::AlienShip() : Ship() {
	Subdivide();
	Subdivide();
	Normalize();
	BoundingBox();
}