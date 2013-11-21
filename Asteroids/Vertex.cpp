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

Vertex::Vertex(float x, float y, float z, float u, float v, Mesh* mesh) {
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
	this->texture_coord.x = u;
	this->texture_coord.y = v;
	this->mesh = mesh;
}

Vertex::Vertex(vec3 position, Mesh* mesh) {
	this->position = position;
	this->mesh = mesh;
}

Vertex::Vertex(vec3 position, vec2 tex, Mesh* mesh) {
	this->position = position;
	this->texture_coord = tex;
	this->mesh = mesh;
}

vector<int> Vertex::neighbors() const {
	vector<int> n;
	for (auto e = edges.begin(); e!=edges.end(); e++) {
		n.push_back(mesh->edges[*e].head_vertex);
	}
	return n;
}

vec3 Vertex::average(bool spike) {
	vector<int> ns = neighbors();
	int n = ns.size();
	vec3 average = spike ? vec3(0) : position * alpha(n);
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