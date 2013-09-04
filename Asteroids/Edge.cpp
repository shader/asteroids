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

void Edge::init(shared_ptr<Vertex> tail, shared_ptr<Vertex> head) {
	this->tail = tail;
	this->head = head;
}

Edge::Edge(shared_ptr<Vertex> tail, shared_ptr<Vertex> head) {
	init(tail, head);
}

Edge::Edge(vec3 tail, vec3 head) {
	shared_ptr<Vertex> a(new Vertex(tail));
	shared_ptr<Vertex> b(new Vertex(head));
	init(a, b);
}

Edge::~Edge() {
}

void attach(shared_ptr<Edge> a, shared_ptr<Edge> b) {
	a->next = b;
	b->left = a->left;
	b->right = a->right;
	b->prev = a;
}

vec3 Edge::midpoint() const {
	return (this->head->position + this->tail->position) * 0.5;
}

void tie(shared_ptr<Edge> edge, shared_ptr<Edge> twin) {
	edge->twin = twin;
	twin->twin = edge;
	twin->left = edge->right;
	twin->right = edge->left;
}

shared_ptr<Edge> twin(shared_ptr<Edge> edge) {
	shared_ptr<Edge> t = edge->twin.lock();
	if (t) return t;
	else {
		t = shared_ptr<Edge>(new Edge(edge->head, edge->tail));
		tie(edge, t);
		return t;
	}
}

pair<shared_ptr<Edge>, shared_ptr<Edge>> split(shared_ptr<Edge> edge) {
	shared_ptr<Vertex> mid(new Vertex(edge->midpoint())); //add the midpoint
	
	shared_ptr<Edge> a(new Edge(edge->tail, mid));
	shared_ptr<Edge> b(new Edge(mid, edge->head));
	attach(a, b);

	return pair<shared_ptr<Edge>,shared_ptr<Edge>>(a, b);
}