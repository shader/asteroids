#include "geometry.h"
#include "utils.h"
using namespace glm;

bool operator<(const Vertex &lhs, const Vertex &rhs) {
	return lhs.position < rhs.position;
}

bool operator==(const Vertex &lhs, const Vertex &rhs) {
	return lhs.position == rhs.position;
}

bool operator!=(const Vertex &lhs, const Vertex &rhs) {
	return !(lhs == rhs);
}

Vertex::Vertex(float x, float y, float z) {
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
}

Vertex::Vertex(vec3 position) {
	this->position = position;
}

Vertex::Vertex(vec3 position, GLuint index) {
	this->position = position;
	this->index = index;
}

void Vertex::insert_after(Vertex* prev) {
	//insert this vertex in the linked list of vertices after prev
	this->prev = prev;
	next = prev->next;
	prev->next = this;
	next->prev = this;
}

vector<Vertex*> Vertex::neighbors() const {
	vector<Vertex*> n;
	for (auto e = edges.begin(); e!=edges.end(); e++) {
		n.push_back((*e)->head);
	}
	return n;
}

Vertex average(const Vertex &vertex) {
	vector<Vertex*> neighbors = vertex.neighbors();
	int n = neighbors.size();
	Vertex new_vertex = Vertex(vertex.position * alpha(n));
	for (int i = 0; i < n; i++) {
		new_vertex.position += neighbors[i]->position;
	}
	new_vertex.position /= (alpha(n) + n);
	return new_vertex;
}

Vertex perturb(Vertex vertex, float max_radius) {
	vec3 pos = perturb(vertex.position, max_radius);
	return Vertex(pos);
}

vec3 Vertex::normal() const {
	vec3 normal(0);
	for (auto e = edges.begin(); e!=edges.end(); e++) {
		normal += (*e)->left->normal();
	}
	return normal / (float)edges.size();
}