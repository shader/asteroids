#include "utils.h"
#include "kd-tree.h"
#include "geometry.h"
using namespace glm;

Mesh::Mesh() {
	vertices = vector<Vertex>();
	edges = vector<Edge>();
	faces = vector<Face>();
	radius = 0;
};

//copy constructor - update mesh pointer of elements
Mesh::Mesh(const Mesh &mesh) {	
	vertices = mesh.vertices;
	edges = mesh.edges;
	faces = mesh.faces;
	box = mesh.box;
	radius = mesh.radius;

	for (auto v=vertices.begin(); v!=vertices.end(); v++) {
		v->mesh = this;
	}
	for (auto e=edges.begin(); e!=edges.end(); e++) {
		e->mesh = this;
	}
	for (auto f=faces.begin(); f!=faces.end(); f++) {
		f->mesh = this;
	}
}

Mesh& Mesh::operator=(const Mesh &mesh) {
	if (this != &mesh) {
		vertices = mesh.vertices;
		edges = mesh.edges;
		faces = mesh.faces;
		radius = mesh.radius;

		for (auto v=vertices.begin(); v!=vertices.end(); v++) {
			v->mesh = this;
		}
		for (auto e=edges.begin(); e!=edges.end(); e++) {
			e->mesh = this;
		}
		for (auto f=faces.begin(); f!=faces.end(); f++) {
			f->mesh = this;
		}
	}
    return *this;
}

void Mesh::Split() {
	vertices.reserve(vertices.size() + edges.size()); // 
	faces.reserve(faces.size() * 4); // 4 new faces for every face
	edges.reserve(faces.size() * 4 * 3);
	int end = edges.size();
	for (int e = 0; e<end; e++) {
		edges[e].split();
	}
	end = faces.size();
	for (int f = 0; f<end; f++) {
		faces[f].split();
	}
}

void Mesh::Average(bool spike) {
	for (auto v = vertices.begin(); v!=vertices.end(); v++) {
		v->new_position = v->average(spike);
	}
	for (auto v = vertices.begin(); v!=vertices.end(); v++) {
		v->position = v->new_position;
	}
}

void Mesh::Subdivide(int times, bool spike) {
	for (int i=0; i<times; i++) {
		Split();
		Average(spike);
	}
}

void Mesh::Perturb(float max_radius) {
	for (auto v = vertices.begin(); v!=vertices.end(); v++) {
		v->position = ::perturb(v->position, max_radius);
	}
}

void Mesh::Scale(vec3 scale) {	
	for (auto v = vertices.begin(); v!=vertices.end(); v++) {
		v->position *= scale;
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
		radius = glm::max(length(v->position), radius);
	}
}

Box Mesh::BoundingBox() {
	for (auto v = vertices.begin(); v!=vertices.end(); v++) {
		box.lower = min(box.lower, v->position);
		box.upper = max(box.upper, v->position);
	}
	return box;
}

void Mesh::KdTree() {
	kd_tree.resize(vertices.size());
	for (unsigned int i=0; i<vertices.size(); i++) {
		kd_tree[i]=i;
	}
	make_kdtree<int>(kd_tree.begin(), kd_tree.end(), [this](int depth)->function<bool(const int&, const int&)>{
		auto mesh = this;
		return [=](const int& a, const int& b) {
			return mesh->vertices[a].position[depth] < mesh->vertices[b].position[depth];
		};
	}, 0);
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