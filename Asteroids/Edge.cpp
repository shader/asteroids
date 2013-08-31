#include "geometry.h"
#include "utils.h"
using namespace glm;

bool operator<(const Edge &lhs, const Edge &rhs) {
	return lhs.midpoint() < rhs.midpoint() ||
		(*(lhs.head) < *(rhs.head) &&
		 *(lhs.tail) < *(rhs.tail) &&
		 lhs.tail->position != rhs.head->position);
}
bool operator==(const Edge &lhs, const Edge &rhs) {
	return *(lhs.head) == (*rhs.head) && *(lhs.tail) == (*rhs.tail);
}
bool operator!=(const Edge &lhs, const Edge &rhs) {
	return !(lhs == rhs);
}

Edge::Edge(Vertex* tail, Vertex* head) {
	this->tail = tail;
	this->head = head;
	tail->edges.push_back(this);
	twin = next = prev = NULL;
	left = right = NULL;
}

void Edge::attach(Edge *edge) {
	this->next = edge;
	edge->prev = this;
}

vec3 Edge::midpoint() const {
	return (this->head->position + this->tail->position) * 0.5;
}