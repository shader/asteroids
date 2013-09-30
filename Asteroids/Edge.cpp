#include "geometry.h"
#include "utils.h"
using namespace glm;

bool operator<(const Edge &lhs, const Edge &rhs) {
	return lhs.midpoint() < rhs.midpoint() || (lhs.midpoint() < rhs.midpoint() &&
		(*(lhs.head) < *(rhs.head) &&
		 *(lhs.tail) < *(rhs.tail) &&
		 lhs.tail->position != rhs.head->position));
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

void split_edge(Edge* edge, Vertex* mid) {	
	Edge* new_edge = new Edge(mid, edge->head);
	new_edge->next = edge->next;
	edge->next->prev = new_edge;
	edge->head = mid;
	edge->attach(new_edge);
}

void Edge::split() {
	if (next->twin->next->twin == this) return;
	Vertex* mid = new Vertex(midpoint()); //add the midpoint
	mid->insert_after(tail);
	
	split_edge(this, mid);
	split_edge(twin, mid);

	tie(this->next, twin->next);
}

void tie(Edge* edge, Edge* twin) {
	edge->twin = twin;
	twin->twin = edge;
	edge->right = twin->left;
	twin->right = edge->left;
}

Edge* twin(Edge* edge) {
	if (edge->twin != NULL) return edge->twin;
	else {
		Edge* t = new Edge(edge->head, edge->tail);
		tie(edge, t);
		return t;
	}
}

pair<Edge*, Edge*> split(Edge *edge) {
	Vertex* mid = new Vertex(edge->midpoint()); //add the midpoint
	
	Edge* a = new Edge(new Vertex(edge->tail->position), mid);
	Edge* b = new Edge(mid, new Vertex(edge->head->position));
	a->attach(b);

	return pair<Edge*,Edge*>(a, b);
}

Edge average(const Edge &edge) {
	Vertex* a = new Vertex(average(*edge.tail));
	Vertex* b = new Vertex(average(*edge.head));
	return Edge(a, b);
}

Edge perturb(const Edge &edge, float max_radius) {
	Vertex* a = new Vertex(perturb(*edge.tail, max_radius));
	Vertex* b = new Vertex(perturb(*edge.head, max_radius));
	return Edge(a, b);
}