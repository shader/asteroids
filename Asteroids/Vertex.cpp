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

vector<shared_ptr<Vertex>> Vertex::neighbors() {
	vector<shared_ptr<Vertex>> n;
	shared_ptr<Edge> edge;
	for (set<weak_ptr<Edge>>::iterator e = edges.begin(); e!=edges.end(); e++) {
		edge = e->lock();
		if(edge) n.push_back(edge->head);
	}
	return n;
}