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

void Edge::init(Vertex *tail, Vertex *head) {
	this->tail = tail;
	this->head = head;
	tail->edges.insert(this);
	twin = next = prev = NULL;
	left = right = NULL;
}

Edge::Edge(Vertex* tail, Vertex* head) {
	init(tail, head);
}

Edge::Edge(vec3 tail, vec3 head) {
	init(new Vertex(tail), new Vertex(head));
}

Edge::~Edge() {
	tail->edges.erase(this);
}

void Edge::attach(Edge *edge) {
	this->next = edge;
	this->next->left = this->left;
	this->next->right = this->right;
	edge->prev = this;
}

vec3 Edge::midpoint() const {
	return (this->head->position + this->tail->position) * 0.5;
}

void tie(Edge* edge, Edge* twin) {
	edge->twin = twin;
	twin->twin = edge;
	twin->left = edge->right;
	twin->right = edge->left;
}

pair<Edge*, Edge*> Edge::split() {
	Vertex* mid = new Vertex(this->midpoint()); //add the midpoint
	
	Edge* new_edge = new Edge(mid, this->head);
	if (this->next != NULL)
		new_edge->attach(this->next);

	this->head = mid;
	this->attach(new_edge);

	return pair<Edge*,Edge*>(this, new_edge);
}