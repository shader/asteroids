#include "geometry.h"
#include "utils.h"
using namespace glm;

bool operator<(const Vertex &lhs, const Vertex &rhs) {
	return lhs.position < rhs.position;
}

bool operator==(const Vertex &lhs, const Vertex &rhs) {
	return lhs.position == rhs.position;
}

bool operator!=(const Vertex &lhs, const Vertex &rhs) {
	return !(lhs == rhs);
}

Vertex::Vertex(vec3 position) {
	this->position = position;
}

Vertex::Vertex(vec3 position, GLuint index) {
	this->position = position;
	this->index = index;
}