#include "geometry.h"
#include "utils.h"
using namespace glm;

bool operator<(const Face &lhs, const Face &rhs) {
	shared_ptr<Edge> a1 = lhs.first, b1 = a1->next.lock(), c1 = b1->next.lock();
	shared_ptr<Edge> a2 = rhs.first, b2 = a2->next.lock(), c2 = b2->next.lock();
	return lhs.midpoint() < rhs.midpoint() ||
		(lhs.midpoint() == rhs.midpoint() && (*a1 < *a2 ||
			(*a1 == *a2 && (*b1 < *b2 ||
				(*b1 == *b2 && *c1 < *c2)))));
}
bool operator==(const Face &lhs, const Face &rhs) {
	shared_ptr<Edge> a1 = lhs.first, b1 = a1->next.lock();
	shared_ptr<Edge> a2 = rhs.first, b2 = a2->next.lock();
	return *a1 == *a2 && *b1 == *b2;
}
bool operator!=(const Face &lhs, const Face &rhs) {
	return !(lhs == rhs);
}

Face::Face(shared_ptr<Edge> first) {
	this->first = first;
}

Face::Face(shared_ptr<Edge> a, shared_ptr<Edge> b, shared_ptr<Edge> c) {
	this->first = a;
	attach(a, b);
	attach(b, c);
	attach(c, a);
}

Face::Face(vec3 a, vec3 b, vec3 c) {
	first = shared_ptr<Edge>(new Edge(a, b));
	shared_ptr<Edge> second(new Edge(b, c));	
	shared_ptr<Edge> third(new Edge(c, a));
	attach(first, second);
	attach(second, third);
	attach(third, first);
}

vec3 Face::midpoint() const {
	shared_ptr<Edge> second = first->next.lock(), third = second->next.lock();
	if (first && second && third)
		return (1.0/3) * (first->head->position + second->head->position + third->head->position);
}

vector<shared_ptr<Edge>> split_edges(shared_ptr<Face> face) {
	shared_ptr<Edge> e = face->first, next;
	vector<shared_ptr<Edge>> edges;
	edges.reserve(6);
	do {
		next = e->next.lock();
		pair<shared_ptr<Edge>,shared_ptr<Edge>>new_edges = split(e);
		edges.push_back(new_edges.first);
		edges.push_back(new_edges.second);
		e = next;
	} while (e != face->first);
	return edges;
}

vector<shared_ptr<Face>> split(shared_ptr<Face> face) {
	vector<shared_ptr<Face>> faces;
	vector<shared_ptr<Edge>> twins;
	faces.reserve(4);
	twins.reserve(3);
	vector<shared_ptr<Edge>> edges = split_edges(face);
	shared_ptr<Edge> a, b, c;
	for (int i=1; i<6; i+=2) {
		a = edges[i];
		b = edges[(i+1)%6];
		c = shared_ptr<Edge>(new Edge(b->head, a->tail));
		twins.push_back(twin(c)); //new edge for fourth triangle
		shared_ptr<Face> new_face(new Face(a, b, c));
		faces.push_back(new_face);
	}
	shared_ptr<Face> new_face(new Face(twins[0], twins[1], twins[2]));
	faces.push_back(new_face);
	return faces;
}