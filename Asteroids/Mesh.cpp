#include "geometry.h"
#include "utils.h"
using namespace glm;

Mesh::~Mesh() {
}

Mesh* split(Mesh *mesh) {
	mesh->vertices.reserve(mesh->vertices.size() + mesh->edges.size()); // 
	mesh->faces.reserve(mesh->faces.size() * 4); // 4 new faces for every face
	mesh->edges.reserve(mesh->faces.size() * 4 * 3);
	int end = mesh->edges.size();
	for (int e = 0; e<end; e++) {
		mesh->edges[e].split();
	}
	end = mesh->faces.size();
	for (int f = 0; f<end; f++) {
		mesh->faces[f].split();
	}
	return mesh;
}

Mesh* average(Mesh *mesh) {
	for (auto v = mesh->vertices.begin(); v!=mesh->vertices.end(); v++) {
		v->new_position = average(*v).position;
	}
	for (auto v = mesh->vertices.begin(); v!=mesh->vertices.end(); v++) {
		v->position = v->new_position;
	}
	return mesh;
}

Mesh* subdivide(Mesh *mesh) {
	return split(average(mesh));
}

void Mesh::Perturb(float max_radius) {
	for (auto v = vertices.begin(); v!=vertices.end(); v++) {
		v->position = perturb(v->position, max_radius);
	}
}

void Mesh::Normalize() {
	vec3 min_corner, max_corner, center;
	min_corner = max_corner = vec3(0,0,0);
	float max_length = 0;

	//find center
	for (auto v = vertices.begin(); v!=vertices.end(); v++) {
		min_corner = min(min_corner, v->position);
		max_corner = max(max_corner, v->position);
	}
	center = (min_corner + max_corner) / 2.0f;

	//find max length
	for (auto v = vertices.begin(); v!=vertices.end(); v++) {
		max_length = glm::max(max_length, length(v->position - center));
	}

	//normalize
	for (auto v = vertices.begin(); v!=vertices.end(); v++) {
		v->position = (v->position - center) / max_length;
	}
}

Box Mesh::BoundingBox() {
	for (auto v = vertices.begin(); v!=vertices.end(); v++) {
		box.lower = min(box.lower, v->position);
		box.upper = max(box.upper, v->position);
	}
	return box;
}

int add_edge(int a, int b, Mesh* mesh) {
	int edge, twin = -1;
	for (auto e=mesh->edges.begin(); e!=mesh->edges.end(); e++) {
		if (e->head_vertex == a && e->tail_vertex == b) {
			twin = e - mesh->edges.begin();
			break;
		}
	}
	
	edge = mesh->edges.size();
	mesh->edges.push_back(Edge(a,b,mesh));

	if (twin != -1) {
		tie(mesh->edges[edge], mesh->edges[twin]);
	}
	return edge;
}

void Mesh::LoadTriangles(uint* triangles, int triangle_count) {
	faces.reserve(faces.size() + triangle_count);
	edges.reserve(edges.size() + triangle_count * 3);
	
	int a, b, c;
	int e1, e2, e3;
	
	for (auto t = triangles; t< triangles + triangle_count*3; t+=3) {
		a = t[0]; b = t[1]; c = t[2];
		e1 = add_edge(a, b, this);
		e2 = add_edge(b, c, this);
		e3 = add_edge(c, a, this);
		faces.push_back(Face(e1, e2, e3, this));
	}
}