#include "geometry.h"
using namespace glm;

bool operator<(const Vertex &lhs, const Vertex &rhs) {
	return lhs.position.x < rhs.position.x ||
		(lhs.position.x == rhs.position.x && lhs.position.y < rhs.position.y) ||
		(lhs.position.x == rhs.position.x && lhs.position.y == rhs.position.y && lhs.position.z < rhs.position.z);
}
bool operator==(const Vertex &lhs, const Vertex &rhs) {
	return lhs.position == rhs.position;
}

bool operator<(const Edge &lhs, const Edge &rhs) {
	return *(lhs.head) < *(rhs.head) ||
		(*(lhs.head) == *(rhs.head) && *(lhs.tail) < *(rhs.tail));
}
bool operator==(const Edge &lhs, const Edge &rhs) {
	return *(lhs.head) == (*rhs.head) && *(lhs.tail) == (*rhs.tail) ;
}

bool operator<(const Face &lhs, const Face &rhs) {
	return *(lhs.first) < *(rhs.first) ||
		(*(lhs.first) == *(rhs.first) && *(lhs.first->next) < *(rhs.first->next));
}
bool operator==(const Face &lhs, const Face &rhs) {
	return *(lhs.first) == (*rhs.first) &&
		*(lhs.first->next) == (*rhs.first->next);
}

bool vertexcmp(Vertex* lhs, Vertex* rhs) {	return *lhs < *rhs; }
bool edgecmp(Edge* lhs, Edge* rhs) { return *lhs < *rhs; }
bool facecmp(Face* lhs, Face* rhs) { return *lhs < *rhs; }

Vertex::Vertex(vec3 position) {
	this->position = position;
}
Vertex::Vertex(vec3 position, GLuint index) {
	this->position = position;
	this->index = index;
}

Edge::Edge(Vertex* tail, Vertex* head) {
	this->tail = tail;
	this->head = head;
}

Face::Face(Edge* first) {
	this->first = first;
}

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
	GLuint i = this->vertices.size();
	pair<set<Vertex*>::iterator, bool> ret = this->vertices.insert(new Vertex(position, i));
	return *(ret.first); //pointer to vertex, or equivalent vertex if it already exists
}

//Add edge to mesh using set insertion, to prevent duplicates
Edge* Mesh::AddEdge(vec3 a, vec3 b) {
	Vertex *vert_a = this->AddVertex(a);
	Vertex *vert_b = this->AddVertex(b);
	pair<set<Edge*>::iterator, bool> ret = this->edges.insert(new Edge(vert_a, vert_b));
	return *(ret.first); //pointer to edge, or equivalent edge if it already exists
}

Face* Mesh::AddFace(vec3 a, vec3 b, vec3 c) {
	Edge *edge1 = this->AddEdge(a, b);
	Edge *edge2 = this->AddEdge(b, c);
	Edge *edge3 = this->AddEdge(c, a);
	edge1->next = edge2;
	edge2->next = edge3;
	edge3->next = edge1;
	pair<set<Face*>::iterator, bool> ret = this->faces.insert(new Face(edge1));
	return *(ret.first); //pointer to face, or equivalent face if it already exists
}