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

vec3 Vertex::average() {
	vec3 average(0);
	vector<int> ns = neighbors();
	int n = ns.size();
	Vertex new_vertex = Vertex(position * alpha(n), mesh);
	for (int i = 0; i < n; i++) {
		average += mesh->vertices[ns[i]].position;
	}
	average /= (alpha(n) + n);
	return average;
}

vec3 Vertex::normal() const {
	vec3 normal(0);
	for (auto e = edges.begin(); e!=edges.end(); e++) {
		normal += mesh->faces[mesh->edges[*e].left_face].normal();
	}
	return normal / (float)edges.size();
}