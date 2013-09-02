#include "geometry.h"
using namespace glm;

bool vertexcmp(Vertex* lhs, Vertex* rhs) {	return *lhs < *rhs; }
bool edgecmp(Edge* lhs, Edge* rhs) { return *lhs < *rhs; }
bool facecmp(Face* lhs, Face* rhs) { return *lhs < *rhs; }

Mesh::Mesh() {
	bool(*vert_pt)(Vertex*,Vertex*) = vertexcmp;
	bool(*edge_pt)(Edge*,Edge*) = edgecmp;
	bool(*face_pt)(Face*,Face*) = facecmp;
	vertices = set<Vertex*,bool(*)(Vertex*,Vertex*)>(vert_pt);
	edges = set<Edge*,bool(*)(Edge*,Edge*)>(edge_pt);
	faces = set<Face*,bool(*)(Face*,Face*)>(face_pt);
}

//Add vertex using set insertion, so that vertices are not created twice
Vertex* Mesh::AddVertex(vec3 position) {
	GLuint i = vertices.size();
	Vertex* vert = new Vertex(position, i);
	pair<set<Vertex*>::iterator, bool> ret = vertices.insert(vert);
	if (!ret.second) delete vert; //delete duplicate vertex
	return *(ret.first); //pointer to vertex, or equivalent vertex if it already exists
}

//Add edge to mesh using set insertion, to prevent duplicates
Edge* Mesh::AddEdge(vec3 a, vec3 b) {
	Vertex *vert_a = this->AddVertex(a);
	Vertex *vert_b = this->AddVertex(b);
	Edge* edge = new Edge(vert_a, vert_b);
	pair<set<Edge*>::iterator, bool> ret = this->edges.insert(edge);
	if (ret.second) {
		//new edge
		tie(edge, new Edge(vert_b, vert_a)); //ensure there's a twin, but not in the set
	} else {
		//duplicate or twin edge
		Edge* other = *(ret.first);
		if (*other != *edge) { //not identical edges, must be twin
			if (other->twin == NULL) { //new twin
				tie(other, edge);
				return edge; //return new edge, even though it wasn't saved in the edges set
			} else {
				delete edge; //equivalent, but twin already exists
				return other->twin;
			}
		}
	}
	return *(ret.first); //pointer to edge, or equivalent edge if it already exists
}

Face* Mesh::AddFace(vec3 a, vec3 b, vec3 c) {
	Edge *edge1 = this->AddEdge(a, b);
	Edge *edge2 = this->AddEdge(b, c);
	Edge *edge3 = this->AddEdge(c, a);
	edge1->attach(edge2);
	edge2->attach(edge3);
	edge3->attach(edge1);
	pair<set<Face*>::iterator, bool> ret = this->faces.insert(new Face(edge1));
	return *(ret.first); //pointer to face, or equivalent face if it already exists
}

Vertex* Mesh::add(Vertex *vertex) {
	pair<set<Vertex*>::iterator, bool> ret = vertices.insert(vertex);
	if (!ret.second) {
		if (*(ret.first) != vertex)
			delete vertex; //delete duplicate vertex
	} else {
		vertex->index = vertices.size()-1; //new vertex gets next index
	}
	return *(ret.first); //pointer to vertex, or equivalent vertex if it already exists
}

Edge* Mesh::add(Edge* edge) {
	edge->head = this->add(edge->head);
	edge->tail = this->add(edge->head);
	pair<set<Edge*>::iterator, bool> ret = edges.insert(edge);
	return *(ret.first);
}

Face* Mesh::add(Face* face) {
	face->first = this->add(face->first);
	Edge *e = face->first, *next = e->next;
	do {
		e->next = this->add(next);
		e = e->next;
		next = next->next;
	} while (e != face->first);
	pair<set<Face*>::iterator, bool> ret = faces.insert(face);
	return *(ret.first);
}

void Mesh::erase(Vertex *vertex) {
	vertices.erase(vertex);
}

void Mesh::erase(Edge *edge) {
	edges.erase(edge);
}

void Mesh::erase(Face *face) {
	faces.erase(face);
	Edge *e = face->first;
	do {
		edges.erase(e);
		e = e->next;
	} while (e != face->first);
}

void Mesh::split() {
	vector<Face*> new_faces(faces.size());
	copy(faces.begin(), faces.end(), new_faces.begin());
	for (vector<Face*>::iterator f = new_faces.begin(); f!=new_faces.end(); f++) {
		this->erase(*f);
		(*f)->split();
		this->add(*f);
	}
}

void Mesh::subdivide(int times) {
}