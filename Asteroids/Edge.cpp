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
	tail->edges.insert(this);
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

void twin(Edge* edge, Edge* twin) {
	edge->twin = twin;
	twin->twin = edge;
	twin->left = edge->right;
	twin->right = edge->left;
}

pair<Edge*, Edge*> split(Edge* edge) {
	Vertex* mid = new Vertex(edge->midpoint()); //add the midpoint
	
	Edge* new_edge = new Edge(mid, edge->head);
	twin(new_edge, edge->twin);
	new_edge->twin->head = mid; //the twin of the original edge stays tied to the same point

	edge->head = mid;
	Edge* new_twin = new Edge(mid, edge->tail); //make a new twin for the original edge
	twin(edge, new_twin);

	edge->attach(new_edge);
	new_twin->attach(edge->twin);

	return pair<Edge*,Edge*>(edge, new_edge);
}