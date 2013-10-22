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
	vec3 min_corner, max_corner, center, max_deviation;
	min_corner = max_corner = vec3(0,0,0);

	for (auto v = vertices.begin(); v!=vertices.end(); v++) {
		min_corner = min(min_corner, v->position);
		max_corner = max(max_corner, v->position);
	}
	center = (min_corner + max_corner) / 2.0f;
	max_deviation = max(abs(min_corner - center), abs(max_corner - center));
	float scale = glm::max(max_deviation.x, glm::max(max_deviation.y, max_deviation.z));
	for (auto v = vertices.begin(); v!=vertices.end(); v++) {
		v->position = (v->position - center) / scale;
	}
}

int add_edge(int a, int b, Mesh* mesh) {
	int edge, twin;
	edge = twin = -1;
	for (auto e=mesh->edges.begin(); e!=mesh->edges.end(); e++) {
		if (e->tail_vertex == a && e->head_vertex == b) {
			edge = e - mesh->edges.begin();
		}
		else if (e->head_vertex == a && e->tail_vertex == b) {
			twin = e - mesh->edges.begin();
		}
	}

	if (edge == -1) {
		edge = mesh->edges.size();
		mesh->edges.push_back(Edge(a,b,mesh));
	}
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
		faces.emplace_back(Face(e1, e2, e3, this));
	}
}