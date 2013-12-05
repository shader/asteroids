#include "geometry.h"
#include "utils.h"
using namespace glm;


bool intersect(Face &face, Plane &plane) {
	return intersect(face.first(), plane) ||
		intersect(face.first().next(), plane) ||
		intersect(face.first().next().next(), plane);
}

bool operator<(const Face &lhs, const Face &rhs) {
	return lhs.midpoint() < rhs.midpoint() ||
		(lhs.midpoint() == rhs.midpoint() && (lhs.first() < rhs.first() ||
			(lhs.first() == rhs.first() && (lhs.first().next()) < rhs.first().next() ||
				(lhs.first().next() == rhs.first().next() && lhs.first().next().next() < rhs.first().next().next()))));
}
bool operator==(const Face &lhs, const Face &rhs) {
	return lhs.first() == rhs.first() &&
		lhs.first().next() == rhs.first().next();
}
bool operator!=(const Face &lhs, const Face &rhs) {
	return !(lhs == rhs);
}

Face::Face() {}

Edge &Face::first() const { return mesh->edges[first_edge]; }

Face::Face(int first, Mesh *mesh) {
	first_edge = first;
	this->mesh = mesh;
	index = mesh->faces.size();
	Edge *e = &this->first();
	do {
		e->left_face = index;
		if (e->twin_edge != -1) e->twin().right_face = index;
		else e->twin_edge = create_twin(*e).index;
		e = &e->next();
	} while (e->index != first);
}

Face::Face(int a, int b, int c, Mesh* mesh) {
	first_edge = a;
	Edge &e1 = mesh->edges[a], &e2 = mesh->edges[b], &e3 = mesh->edges[c];
	this->mesh = mesh;
	first().left_face = index = mesh->faces.size();

	e1.attach(e2);
	e2.attach(e3);
	e3.attach(e1);
}

vec3 Face::midpoint() const {
	return (1.0/3) * (first().head().position + first().next().head().position + first().next().next().head().position);
}

vec3 Face::normal() const {
	vec3 a = first().head().position - first().tail().position;
	vec3 b = first().next().head().position - first().next().tail().position;
	vec3 normal = cross(a, b);
	return normalize(normal);
}

int new_face(Edge &e) {
	int new_face = e.mesh->faces.size();
	e.mesh->faces.push_back(Face(e.index, e.mesh));
	return new_face;
}

void Face::split() {
	//add new edges and faces
	Edge *e = &first(), *new_twin = nullptr, *twin = nullptr, *new_edge;
	do {
		Edge *next = &e->next().next();
		mesh->edges.push_back(Edge(e->head_vertex, e->prev().tail_vertex, mesh));
		new_edge = &mesh->edges.back();
		e->attach(*new_edge);
		new_edge->attach(e->prev());
		if (e->index != first_edge) {
			new_face(*e);
		}

		new_twin = &create_twin(*new_edge);
		if (twin) {
			twin->attach(*new_twin);
		}
		twin = new_twin;

		e = next;
	} while (e->index != first_edge);

	twin->attach(first().next().twin());

	//add new face in center
	e = &first().next().twin();
	new_face(*e);
}