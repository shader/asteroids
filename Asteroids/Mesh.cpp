#include "geometry.h"
#include "utils.h"
using namespace glm;

bool vertexcmp(Vertex* lhs, Vertex* rhs) {	return *lhs < *rhs; }
bool edgecmp(Edge* lhs, Edge* rhs) { return *lhs < *rhs; }
bool facecmp(Face* lhs, Face* rhs) { return *lhs < *rhs; }

Mesh::Mesh() {
	bool(*vert_pt)(Vertex*,Vertex*) = vertexcmp;
	bool(*edge_pt)(Edge*,Edge*) = edgecmp;
	bool(*face_pt)(Face*,Face*) = facecmp;
	vertices = new set<Vertex*,bool(*)(Vertex*,Vertex*)>(vert_pt);
	edges = new set<Edge*,bool(*)(Edge*,Edge*)>(edge_pt);
	faces = new set<Face*,bool(*)(Face*,Face*)>(face_pt);
}

Mesh::~Mesh() {
	for (auto f = faces->begin(); f != faces->end(); f++) {
		delete *f;
	}
	delete faces;

	for (auto e = edges->begin(); e != edges->end(); e++) {
		delete *e;
	}
	delete edges;

	for (auto v = vertices->begin(); v != vertices->end(); v++) {
		delete *v;
	}
	delete vertices;
}

//Add vertex using set insertion, so that vertices are not created twice
Vertex* Mesh::AddVertex(vec3 position) {
	GLuint i = vertices->size();
	Vertex* vert = new Vertex(position, i);
	auto ret = vertices->insert(vert);
	if (!ret.second) delete vert; //delete duplicate vertex
	return *(ret.first); //pointer to vertex, or equivalent vertex if it already exists
}

//Add edge to mesh using set insertion, to prevent duplicates
Edge* Mesh::AddEdge(vec3 a, vec3 b) {
	Vertex *vert_a = this->AddVertex(a);
	Vertex *vert_b = this->AddVertex(b);
	Edge* edge = new Edge(vert_a, vert_b);
	return this->add(edge);
}

Face* Mesh::AddFace(vec3 a, vec3 b, vec3 c) {
	return this->add(new Face(a, b, c));
}

Vertex* Mesh::add(Vertex *vertex) {
	auto ret = vertices->insert(vertex);
	if (ret.second) {
		vertex->index = vertices->size()-1; //new vertex gets next index
	} else {
		Vertex* other = *(ret.first);
		if (*other == *vertex && other != vertex) {
			//same vertex, different pointers
			for(auto e = vertex->edges.begin(); e != vertex->edges.end(); e++) {
				(*e)->tail = other;
				other->edges.insert(*e);
				(*e)->prev->head = other;
			}
			delete vertex;
		}
	}
	return *(ret.first); //pointer to vertex, or equivalent vertex if it already exists
}

Edge* Mesh::add(Edge* edge) {
	edge->head = this->add(edge->head);
	edge->tail = this->add(edge->tail);
	auto ret = edges->insert(edge);
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

Face* Mesh::add(Face* face) {
	Edge *next = face->first->next;
	Edge *e = face->first = this->add(face->first);
	do {
		e->attach(this->add(next));
		e = e->next;
		next = next->next;
	} while (e != face->first);
	auto ret = faces->insert(face);
	return *(ret.first);
}

void Mesh::erase(Vertex *vertex) {
	vertices->erase(vertex);
}

void Mesh::erase(Edge *edge) {
	edges->erase(edge);
}

void Mesh::erase(Face *face) {
	faces->erase(face);
	Edge *e = face->first;
	do {
		edges->erase(e);
		e = e->next;
	} while (e != face->first);
}

Mesh* split(Mesh *mesh) {
	Mesh* new_mesh = new Mesh();
	for (auto f = mesh->faces->begin(); f!=mesh->faces->end(); f++) {
		vector<Face*> new_faces = split(*f);
		for (int i=0; i<4; i++) {
			new_mesh->add(new_faces[i]);
		}
	}
	return new_mesh;
}

Mesh* average(Mesh *mesh) {
	Mesh* new_mesh = new Mesh();
	for (auto f = mesh->faces->begin(); f!=mesh->faces->end(); f++) {
		Face* new_face = new Face(average(**f));
		new_mesh->add(new_face);
	}
	return new_mesh;
}

Mesh* subdivide(Mesh *mesh) {
	Mesh* tmp = split(mesh);
	Mesh* new_mesh = average(tmp);
	delete tmp;
	return new_mesh;
}

void Mesh::Perturb(float max_radius) {
	vector<Vertex*> tmp(vertices->size());
	copy(vertices->begin(), vertices->end(), tmp.begin());
	for (auto v = tmp.begin(); v!=tmp.end(); v++) {
		vertices->erase(*v);
		(*v)->position = perturb((*v)->position, max_radius);
		vertices->insert(*v);
	}
}

void Mesh::Normalize() {
	vector<Vertex*> tmp(vertices->size());
	vec3 min_corner, max_corner, center, max_deviation;
	min_corner = max_corner = vec3(0,0,0);

	copy(vertices->begin(), vertices->end(), tmp.begin());
	for (auto v = tmp.begin(); v!=tmp.end(); v++) {
		min_corner = min(min_corner, (*v)->position);
		max_corner = max(max_corner, (*v)->position);
	}
	center = (min_corner + max_corner) / 2.0f;
	max_deviation = max(abs(min_corner - center), abs(max_corner - center));
	float scale = glm::max(max_deviation.x, glm::max(max_deviation.y, max_deviation.z));
	for (auto v = tmp.begin(); v!=tmp.end(); v++) {
		vertices->erase(*v);
		(*v)->position = ((*v)->position - center) / scale;
		vertices->insert(*v);
	}
}

//Mesh* subdivide(Mesh *mesh) {
//	Mesh* new_mesh = new Mesh();
//	for (auto f = mesh->faces->begin(); f!=mesh->faces->end(); f++) {
//		vector<Face*> tmp = split(*f);
//		for (int i=0; i<4; i++) {
//			Face* new_face = average(tmp[i]);
//			new_mesh->add(new_face);
//			delete tmp[i];
//		}
//	}
//	return new_mesh;
//}