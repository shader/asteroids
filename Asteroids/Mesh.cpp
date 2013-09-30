#include "geometry.h"
#include "utils.h"
using namespace glm;

bool edgecmp(Edge* lhs, Edge* rhs) { return *lhs < *rhs; }

Mesh::~Mesh() {
	Edge* first = first_face->first;
	Edge *e, *next_edge;

	//linearize all edges and delete faces
	Face *f, *next_face;
	e = first->prev;
	for (f = first_face->next; f!=nullptr; f=next_face) {
		next_face = f->next;
		e->next = f->first;
		e = f->first->prev;
		delete f;
	}

	//delete edges
	for (e = first; e!=nullptr; e=next_edge) {
		next_edge = e->next;
		delete e;
	}

	//delete vertices
	Vertex *v, *next_vertex;
	for (v = first_vertex; v!=nullptr; v=next_vertex) {
		next_vertex = v->next;
		delete v;
	}
}

Mesh* split(Mesh *mesh) {
	for (auto f = mesh->first_face; f!=nullptr; f=f->next) {
		split(f);
	}	
	mesh->vertex_count *= 2;
	mesh->face_count *= 4;
	return mesh;
}

Mesh* average(Mesh *mesh) {
	for (auto v = mesh->first_vertex; v!=nullptr; v=v->next) {
		v->new_position = average(*v).position;
	}
	for (auto v = mesh->first_vertex; v!=nullptr; v=v->next) {
		v->position = v->new_position;
	}
}

Mesh* subdivide(Mesh *mesh) {
	return split(average(mesh));
}

void Mesh::Perturb(float max_radius) {	
	for (auto v = first_vertex; v!=nullptr; v=v->next) {
		v->position = perturb(v->position, max_radius);
	}
}

void Mesh::Normalize() {
	vec3 min_corner, max_corner, center, max_deviation;
	min_corner = max_corner = vec3(0,0,0);

	for (auto v = first_vertex; v!=nullptr; v=v->next) {
		min_corner = min(min_corner, v->position);
		max_corner = max(max_corner, v->position);
	}
	center = (min_corner + max_corner) / 2.0f;
	max_deviation = max(abs(min_corner - center), abs(max_corner - center));
	float scale = glm::max(max_deviation.x, glm::max(max_deviation.y, max_deviation.z));
	for (auto v = first_vertex; v!=nullptr; v=v->next) {
		v->position = (v->position - center) / scale;
	}
}

Edge* add(Edge* edge, set<Edge*,bool(*)(Edge*,Edge*)> &edges) {
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

void Mesh::LoadTriangles(vector<Vertex*> &vertices, vector<int> &triangles) {
    bool(*edge_pt)(Edge*,Edge*) = edgecmp;
    auto edges = new set<Edge*,bool(*)(Edge*,Edge*)>(edge_pt);
	
	Vertex* a,b,c;
	Edge *e1, *e2, *e3;
	
	//link vertices
	first_vertex = vertices[0];
	vertex_count = 1;
	for (auto v = vertices.begin()++; v!=vertices.end(); v++) {
		(*v)->insert_after(*(v-1));
		vertex_count++;
	}

	for (auto t = triangles.begin(); t!=triangles.end(); t++) {
		Face* f = new Face();
	}
}