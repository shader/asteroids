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

Face::Face() {}

Face::Face(Edge* first) {
	this->first = first;
	Edge* e = first;
	do {
		e->left = this;
		if (e->twin != NULL) e->twin->right = this;
		else e->twin = twin(e);
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

vec3 Face::normal() const {
	vec3 a = first->head->position - first->tail->position;
	vec3 b = first->next->head->position - first->next->tail->position;
	vec3 normal = cross(a, b);
	return normalize(normal);
}

void Face::insert_after(Face *prev) {
	//insert this face in the linked list of faces after prev
	this->prev = prev;
	next = prev->next;
	prev->next = this;
	next->prev = this;
}

Face* new_face(Edge *e, Face* prev) {
	Face *new_face;
	e->left = new_face = new Face();
	new_face->first = e;
	new_face->insert_after(prev);
	return new_face;
}

void Face::split() {
	Edge *e = first;
	//split edges
	do {
		Edge *next = e->next;
		e->split();
		e = next;
	} while (e != first);

	//add new edges and faces
	e = first;
	Face *f = this;
	do {
		Edge *next = e->next->next;
		Edge* new_edge = new Edge(e->head, e->prev->tail);
		if (e != first) { //new face
			f = new_face(e, f);
		}
		e->attach(new_edge);
		new_edge->attach(e->prev);
		twin(new_edge);
		e = next;
	} while (e != first);

	//add new face in center
	e = first->next->twin;
	new_face(e, f);
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

Face average(const Face &face) {
	Edge *a = new Edge(average(*face.first));
	Edge *b = new Edge(average(*face.first->next));
	Edge *c = new Edge(average(*face.first->next->next));
	return Face(a, b, c);
}

Face perturb(const Face &face, float max_radius) {
	Edge *a = new Edge(perturb(*face.first, max_radius));
	Edge *b = new Edge(perturb(*face.first->next, max_radius));
	Edge *c = new Edge(perturb(*face.first->next->next, max_radius));
	return Face(a, b, c);
}