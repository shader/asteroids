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
	GLuint i = this->vertices.size();
	Vertex* vert = new Vertex(position, i);
	pair<set<Vertex*>::iterator, bool> ret = this->vertices.insert(vert);
	if (!ret.second) delete vert; //delete duplicate vertex
	return *(ret.first); //pointer to vertex, or equivalent vertex if it already exists
}

//Add edge to mesh using set insertion, to prevent duplicates
Edge* Mesh::AddEdge(vec3 a, vec3 b) {
	Vertex *vert_a = this->AddVertex(a);
	Vertex *vert_b = this->AddVertex(b);
	Edge* edge = new Edge(vert_a, vert_b);
	pair<set<Edge*>::iterator, bool> ret = this->edges.insert(edge);
	if (!ret.second) {
		//duplicate or twin edge
		Edge* other = *(ret.first);
		if (*other != *edge) { //not identical edges, must be twin
			if (other->twin == NULL) { //new twin
				other->twin = edge;
				edge->twin = other;
				edge->right = other->left; // adjacent faces
				return edge; //return new edge, even though it wasn't saved in the edges set
			}
			else delete edge; //equivalent, but twin already exists
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

void Mesh::split(Edge* edge) {
	if (edge->next->twin->next->twin == edge) return; //already split this edge
	Vertex* mid = AddVertex(edge->midpoint());
	Edge* new_edge = new Edge(mid, edge->head);
	edge->head = mid;
	edge->attach(new_edge);
	new_edge->head->edges.erase(edge);

	split(edge->twin);
}