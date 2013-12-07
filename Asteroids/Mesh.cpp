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

void add_face(Mesh &mesh, int edge, Vertex *vertex) {
	vertex->position += mesh.edges[edge].tail().position;
	mesh.edges[edge].left_face = mesh.faces.size();
	mesh.faces.push_back(Face());
	mesh.faces.back().mesh = &mesh;
	mesh.faces.back().first_edge = edge;
	auto e1 = mesh.edges.size();
	mesh.edges.push_back(Edge(mesh.edges[edge].head_vertex, mesh.vertices.size()-1, &mesh));
	for (auto i=vertex->edges.begin(); i!=vertex->edges.end(); i++) {
		if (mesh.edges[*i].head_vertex == mesh.edges[edge].head_vertex) {
			tie(mesh.edges[*i], mesh.edges[e1]);
			break;
		}
	}
	
	auto e2 = mesh.edges.size();
	mesh.edges.push_back(Edge(mesh.vertices.size()-1, mesh.edges[edge].tail_vertex, &mesh));
	auto v = &mesh.vertices[mesh.edges[edge].tail_vertex];
	for (auto i=v->edges.begin(); i!=v->edges.end(); i++) {
		if (mesh.edges[*i].head_vertex == mesh.vertices.size()-1) {
			tie(mesh.edges[*i], mesh.edges[e2]);
			break;
		}
	}
	
	mesh.edges[edge].attach(e1);
	mesh.edges[e1].attach(e2);
	mesh.edges[e2].attach(edge);
}

void Mesh::Slice(Plane plane, Mesh &a, Mesh &b) {
	Vertex* cur = &vertices[0];
	
	vector<int> a_vertices(vertices.size(), -1), a_edges(edges.size(), -1), a_faces(faces.size(), -1);
	vector<int> b_vertices(vertices.size(), -1), b_edges(edges.size(), -1), b_faces(faces.size(), -1);

	//separate vertices
	for (int i=0; i<vertices.size(); i++) {
		vertices[i].edges.clear();
		if (dot(vertices[i].position, plane) > 0) {
			b_vertices[i] = b.vertices.size();
			b.vertices.push_back(vertices[i]);
		} else {
			a_vertices[i] = a.vertices.size();
			a.vertices.push_back(vertices[i]);
		}
	}

	for (int i=0; i<edges.size(); i++) {
		if (intersect(edges[i], plane)) {

		} else if (a_vertices[edges[i].head_vertex] != -1 && a_vertices[edges[i].tail_vertex] != -1) {
			a_edges[i] = a.edges.size();
			a.edges.push_back(edges[i]);
			auto e = &a.edges.back();
			e->mesh = &a;
			e->head_vertex = a_vertices[e->head_vertex];
			e->tail_vertex = a_vertices[e->tail_vertex];
			e->tail().edges.insert(a.edges.size()-1);
		} else if (b_vertices[edges[i].head_vertex] != -1 && b_vertices[edges[i].tail_vertex] != -1) {
			b_edges[i] = b.edges.size();
			b.edges.push_back(edges[i]);
			auto e = &b.edges.back();
			e->mesh = &b;
			e->head_vertex = b_vertices[e->head_vertex];
			e->tail_vertex = b_vertices[e->tail_vertex];
			e->tail().edges.insert(b.edges.size()-1);
		}
	}
	
	for (int i=0; i<faces.size(); i++) {
		if (intersect(faces[i], plane)) {
		} else if (a_edges[faces[i].first_edge] != -1 && a_edges[faces[i].first().next_edge] != -1 && a_edges[faces[i].first().next().next_edge] != -1) {
			a_faces[i] = a.faces.size();
			a.faces.push_back(faces[i]);
			auto f = &a.faces.back();
			f->mesh = &a;
			f->first_edge = a_edges[f->first_edge];
		} else {
			b_faces[i] = b.faces.size();
			b.faces.push_back(faces[i]);
			auto f = &b.faces.back();
			f->mesh = &b;
			f->first_edge = b_edges[f->first_edge];
		}
	}

	Vertex *a_center, *b_center;
	a.vertices.push_back(Vertex(0,0,0, this));
	b.vertices.push_back(Vertex(0,0,0, this));
	a_center = &a.vertices.back();
	b_center = &b.vertices.back();
	int a_count=0, b_count=0;

	auto a_size = a.edges.size();
	for (int i=0; i<a_size; i++) {
		a.edges[i].next_edge = a_edges[a.edges[i].next_edge];
		a.edges[i].twin_edge = a_edges[a.edges[i].twin_edge];
		a.edges[i].left_face = a_faces[a.edges[i].left_face];
				
		if (a.edges[i].left_face == -1) {
			add_face(a, i, a_center);
			a_count++;
		}
	}
	auto b_size = b.edges.size();
	for (int i=0; i<b_size; i++) {
		b.edges[i].next_edge = b_edges[b.edges[i].next_edge];
		b.edges[i].twin_edge = b_edges[b.edges[i].twin_edge];
		b.edges[i].left_face = b_faces[b.edges[i].left_face];

		if (b.edges[i].left_face == -1) {
			add_face(b, i, b_center);
			b_count++;
		}
	}

	a_center->position /= (float)a_count;
	b_center->position /= (float)b_count;
}