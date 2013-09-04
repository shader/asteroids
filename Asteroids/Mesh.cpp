#include "geometry.h"
using namespace glm;

bool vertexcmp(shared_ptr<Vertex> lhs, shared_ptr<Vertex> rhs) { return *lhs < *rhs; }
bool edgecmp(shared_ptr<Edge> lhs, shared_ptr<Edge> rhs) { return *lhs < *rhs; }
bool facecmp(shared_ptr<Face> lhs, shared_ptr<Face> rhs) { return *lhs < *rhs; }

Mesh::Mesh() {
	bool(*vert_pt)(shared_ptr<Vertex>, shared_ptr<Vertex>) = vertexcmp;
	bool(*edge_pt)(shared_ptr<Edge>, shared_ptr<Edge>) = edgecmp;
	bool(*face_pt)(shared_ptr<Face>, shared_ptr<Face>) = facecmp;
	vertices = new set<shared_ptr<Vertex>, bool(*)(shared_ptr<Vertex>, shared_ptr<Vertex>)>(vert_pt);
	edges = new set<shared_ptr<Edge>, bool(*)(shared_ptr<Edge>, shared_ptr<Edge>)>(edge_pt);
	faces = new set<shared_ptr<Face>, bool(*)(shared_ptr<Face>, shared_ptr<Face>)>(face_pt);
}

Mesh::~Mesh() {
	delete vertices;
	delete edges;
	delete faces;
}

//Add vertex using set insertion, so that vertices are not created twice
shared_ptr<Vertex> Mesh::AddVertex(vec3 position) {
	GLuint i = vertices->size();
	shared_ptr<Vertex> vert(new Vertex(position, i));
	pair<set<shared_ptr<Vertex>>::iterator, bool> ret = vertices->insert(vert);
	return *(ret.first); //pointer to vertex, or equivalent vertex if it already exists
}

//Add edge to mesh using set insertion, to prevent duplicates
shared_ptr<Edge> Mesh::AddEdge(vec3 a, vec3 b) {
	shared_ptr<Vertex> vert_a = this->AddVertex(a);
	shared_ptr<Vertex> vert_b = this->AddVertex(b);
	shared_ptr<Edge> edge(new Edge(vert_a, vert_b));
	return this->add(edge);
}

shared_ptr<Face> Mesh::AddFace(vec3 a, vec3 b, vec3 c) {
	shared_ptr<Face> face(new Face(a, b, c));
	return this->add(face);
}

shared_ptr<Vertex> Mesh::add(shared_ptr<Vertex> vertex) {
	pair<set<shared_ptr<Vertex>>::iterator, bool> ret = vertices->insert(vertex);
	if (ret.second) {
		vertex->index = vertices->size()-1; //new vertex gets next index
	}
	return *(ret.first); //pointer to vertex, or equivalent vertex if it already exists
}

shared_ptr<Edge> Mesh::add(shared_ptr<Edge> edge) {
	edge->head = this->add(edge->head);
	edge->tail = this->add(edge->tail);
	pair<set<shared_ptr<Edge>>::iterator, bool> ret = edges->insert(edge);
	if (ret.second) {
		//new edge
		shared_ptr<Edge> twin(new Edge(edge->head, edge->tail));
		tie(edge, twin); //ensure there's a twin, but not in the set
	} else {
		//duplicate or twin edge
		shared_ptr<Edge> other = *(ret.first);
		if (*other != *edge) { //not identical edges, must be twin
			shared_ptr<Edge> twin = other->twin.lock();
			if (!twin) { //new twin
				tie(other, edge);
				return edge; //return new edge, even though it wasn't saved in the edges set
			} else {
				return twin;
			}
		}
	}
	return *(ret.first); //pointer to edge, or equivalent edge if it already exists
}

shared_ptr<Face> Mesh::add(shared_ptr<Face> face) {
	shared_ptr<Edge> next = face->first->next.lock();
	shared_ptr<Edge> e = face->first = this->add(face->first);
	do {
		e->next = this->add(next);
		e = e->next.lock();
		next = next->next.lock();
	} while (e != face->first);
	pair<set<shared_ptr<Face>>::iterator, bool> ret = faces->insert(face);
	return *(ret.first);
}

void Mesh::erase(shared_ptr<Vertex> vertex) {
	vertices->erase(vertex);
}

void Mesh::erase(shared_ptr<Edge> edge) {
	edges->erase(edge);
}

void Mesh::erase(shared_ptr<Face> face) {
	faces->erase(face);
	shared_ptr<Edge> e = face->first;
	do {
		edges->erase(e);
		e = e->next.lock();
	} while (e != face->first);
}

shared_ptr<Mesh> split(shared_ptr<Mesh> mesh) {
	shared_ptr<Mesh>new_mesh(new Mesh());
	for (set<shared_ptr<Face>>::iterator f = mesh->faces->begin(); f!=mesh->faces->end(); f++) {
		vector<shared_ptr<Face>> faces = split(*f);
		for (int i=0; i<4; i++) {
			new_mesh->add(faces[i]);
		}
	}
	return new_mesh;
}

void Mesh::subdivide(int times) {
}