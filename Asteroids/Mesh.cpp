#include "geometry.h"
#include "utils.h"
using namespace glm;

bool edgecmp(Edge* lhs, Edge* rhs) { return *lhs < *rhs; }

Mesh::~Mesh() {
}

Mesh* split(Mesh *mesh) {
	mesh->vertices.reserve(mesh->vertices.size() * 2);
	mesh->edges.reserve(mesh->edges.size() * 2);
	mesh->faces.reserve(mesh->faces.size() * 2);
	for (auto f = mesh->faces.begin(); f!=mesh->faces.end(); f++) {
		f->split(mesh->faces, mesh->edges, mesh->vertices);
	}
	return mesh;
}

Mesh* average(Mesh *mesh) {
	for (auto v = mesh->vertices.begin(); v!=mesh->vertices.end(); v++) {
		v->new_position = average(*v).position;
	}
	for (auto v = mesh->vertices.begin(); v!=mesh->vertices.end(); v++) {
		v->position = v->new_position;
	}
	return mesh;
}

Mesh* subdivide(Mesh *mesh) {
	return split(average(mesh));
}

void Mesh::Perturb(float max_radius) {	
	for (auto v = vertices.begin(); v!=vertices.end(); v++) {
		v->position = perturb(v->position, max_radius);
	}
}

void Mesh::Normalize() {
	vec3 min_corner, max_corner, center, max_deviation;
	min_corner = max_corner = vec3(0,0,0);

	for (auto v = vertices.begin(); v!=vertices.end(); v++) {
		min_corner = min(min_corner, v->position);
		max_corner = max(max_corner, v->position);
	}
	center = (min_corner + max_corner) / 2.0f;
	max_deviation = max(abs(min_corner - center), abs(max_corner - center));
	float scale = glm::max(max_deviation.x, glm::max(max_deviation.y, max_deviation.z));
	for (auto v = vertices.begin(); v!=vertices.end(); v++) {
		v->position = (v->position - center) / scale;
	}
}

Edge* get_match(Edge *edge, set<Edge*,bool(*)(Edge*,Edge*)> &edges) {
	auto ret = edges.insert(edge);
	if (!ret.second) {
		//duplicate or twin edge
		Edge* other = *(ret.first);
		if (*other != *edge) { //not identical edges, must be twin
			if (other->twin == NULL) { //new twin
				tie(other, edge);
				return edge; //return new edge, even though it wasn't saved in the edges set
			} else if (other->twin != edge) {
				//same edge, different instance
				edge->prev->next = other->twin; //patch hole in loop
				edge->next->prev = other->twin;
				delete edge; //equivalent, but twin already exists
				return other->twin;
			} else { //identical, return twin
				return edge;
			}
		} else if (other != edge) {
			//same edge, different pointers
			edge->prev->next = other;
			edge->next->prev = other;
			delete edge;
		}
	}
	return *(ret.first); //pointer to edge, or equivalent edge if it already exists
}

Edge* add(Vertex *a, Vertex *b, Mesh* mesh) {
	int edge, twin;
	for (auto e=mesh->edges.begin(); e!=mesh->edges.end(); e++) {
		if (e->tail == a && e->head == b) {
			edge = e-edges.begin();
		}
		else if (e->head == a && e->tail == b) {
			twin = e-edges.begin();
		}
	}

	if (!edge) {
		mesh->edges.push_back(Edge(a,b));
		edge = edges.back()-edges.begin();
		edge->init();
	}
	if (twin) {
		tie(edge, twin);
	}
	return edge;
}

void Mesh::LoadTriangles(uint* triangles, int triangle_count) {
    bool(*edge_pt)(Edge*,Edge*) = edgecmp;
    auto edges = set<Edge*,bool(*)(Edge*,Edge*)>(edge_pt);
	faces.reserve(faces.size() + triangle_count);
	edges.reserve(edges.size() + triangle_count * 3);
	
	Vertex *a, *b, *c;
	Edge *e1, *e2, *e3;
	
	for (auto t = triangles; t< triangles + triangle_count*3; t++) {
		a = &vertices[t[0]]; b = &vertices[t[1]]; c = &vertices[t[2]];
		e1 = add(a, b, this);
		e2 = add(b, c, this);
		e3 = add(c, a, this);
		faces.emplace_back(Face(e1, e2, e3));
	}
}