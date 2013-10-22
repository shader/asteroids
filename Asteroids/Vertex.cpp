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

Vertex::Vertex() {
	position = vec3(0);
	temperature = 0;
}

Vertex::Vertex(float x, float y, float z, Mesh* mesh) {
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
	this->mesh = mesh;
}

Vertex::Vertex(vec3 position, Mesh* mesh) {
	this->position = position;
	this->mesh = mesh;
}

vector<int> Vertex::neighbors() const {
	vector<int> n;
	for (auto e = edges.begin(); e!=edges.end(); e++) {
		n.push_back(mesh->edges[*e].head_vertex);
	}
	return n;
}

Vertex average(const Vertex &vertex) {
	vector<int> neighbors = vertex.neighbors();
	int n = neighbors.size();
	Vertex new_vertex = Vertex(vertex.position * alpha(n), vertex.mesh);
	for (int i = 0; i < n; i++) {
		new_vertex.position += vertex.mesh->vertices[neighbors[i]].position;
	}
	new_vertex.position /= (alpha(n) + n);
	return new_vertex;
}

Vertex perturb(Vertex vertex, float max_radius) {
	vec3 pos = perturb(vertex.position, max_radius);
	return Vertex(pos, vertex.mesh);
}

vec3 Vertex::normal() const {
	vec3 normal(0);
	for (auto e = edges.begin(); e!=edges.end(); e++) {
		normal += mesh->faces[mesh->edges[*e].left_face].normal();
	}
	return normal / (float)edges.size();
}