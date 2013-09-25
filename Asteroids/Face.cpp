#include "geometry.h"
#include "utils.h"
using namespace glm;

bool operator<(const Face &lhs, const Face &rhs) {
	return lhs.midpoint() < rhs.midpoint() ||
		(lhs.midpoint() == rhs.midpoint() && (*(lhs.first) < *(rhs.first) ||
			(*(lhs.first) == *(rhs.first) && (*(lhs.first->next) < *(rhs.first->next) ||
				(*(lhs.first->next) == *(rhs.first->next) && *(lhs.first->next->next) < *(rhs.first->next->next))))));
}
bool operator==(const Face &lhs, const Face &rhs) {
	return *(lhs.first) == (*rhs.first) &&
		*(lhs.first->next) == (*rhs.first->next);
}
bool operator!=(const Face &lhs, const Face &rhs) {
	return !(lhs == rhs);
}

Face::Face(Edge* first) {
	this->first = first;
	Edge* e = first;
	do {
		e->left = this;
		if (e->twin != NULL) e->twin->right = this;
		e=e->next;
	} while (e != first);
}

Face::Face(Edge *a, Edge *b, Edge *c) {
	this->first = a;
	a->left = this;
	a->attach(b);
	b->attach(c);
	c->attach(a);
}

Face::Face(vec3 a, vec3 b, vec3 c) {
	first = new Edge(a, b);
	first->left = this;
	first->attach(new Edge(b, c));
	first->next->attach(new Edge(c, a));
	first->next->next->attach(first);
}

vec3 Face::midpoint() const {
	return (1.0/3) * (first->head->position + first->next->head->position + first->next->next->head->position);
}

vector<Edge*> split_edges(Face *face) {
	Edge *e = face->first;
	vector<Edge*> edges;
	edges.reserve(6);
	do {
		Edge *next = e->next;
		pair<Edge*,Edge*>new_edges = split(e);
		edges.push_back(new_edges.first);
		edges.push_back(new_edges.second);
		e = next;
	} while (e != face->first);
	return edges;
}

vector<Face*> split(Face *face) {
	vector<Face*> faces;
	vector<Edge*> twins;
	faces.reserve(4);
	twins.reserve(3);
	vector<Edge*> edges = split_edges(face);
	for (int i=1; i<6; i+=2) {
		Edge *a = edges[i];
		Edge *b = edges[(i+1)%6];
		Edge *c = new Edge(b->head, a->tail);
		twins.push_back(twin(c)); //new edge for fourth triangle
		faces.push_back(new Face(a, b, c));
	}
	faces.push_back(new Face(twins[0], twins[1], twins[2]));
	return faces;
}

Face* average(Face *face) {
	Edge *a = average(face->first);
	Edge *b = average(face->first->next);
	Edge *c = average(face->first->next->next);
	return new Face(a, b, c);
}

Face* perturb(Face *face, float max_radius) {
	Edge *a = perturb(face->first, max_radius);
	Edge *b = perturb(face->first->next, max_radius);
	Edge *c = perturb(face->first->next->next, max_radius);
	return new Face(a, b, c);
}

vec3 Face::normal() const {
	vec3 a = first->head->position - first->tail->position;
	vec3 b = first->next->head->position - first->next->tail->position;
	vec3 normal = cross(a, b);
	return normalize(normal);
}