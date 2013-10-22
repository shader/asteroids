#include "geometry.h"
#include "utils.h"
using namespace glm;

bool operator<(const Edge &lhs, const Edge &rhs) {
	return lhs.midpoint() < rhs.midpoint() || (lhs.midpoint() < rhs.midpoint() &&
		(lhs.head() < rhs.head() &&
		 lhs.tail() < rhs.tail() &&
		 lhs.tail().position != rhs.head().position));
}
bool operator==(const Edge &lhs, const Edge &rhs) {
	return lhs.head() == rhs.head() && lhs.tail() == rhs.tail();
}
bool operator!=(const Edge &lhs, const Edge &rhs) {
	return !(lhs == rhs);
}

Vertex &Edge::head() const { return mesh->vertices[head_vertex]; }
Vertex &Edge::tail() const { return mesh->vertices[tail_vertex]; }
Edge &Edge::next() const { return mesh->edges[next_edge]; }
Edge &Edge::prev() const { return mesh->edges[prev_edge]; }
Edge &Edge::twin() const { return mesh->edges[twin_edge]; }
Face &Edge::left() const { return mesh->faces[left_face]; }
Face &Edge::right() const { return mesh->faces[right_face]; }

Edge::Edge(int tail, int head, Mesh* mesh) {
	this->tail_vertex = tail;
	this->head_vertex = head;
	this->index = mesh->edges.size();
	this->mesh = mesh;
	twin_edge = -1;
	this->tail().edges.insert(index);
}

Edge::~Edge() {
	tail().edges.erase(index);
}

void Edge::attach(int edge) {
	next_edge = edge;
	next().left_face = left_face;
	next().right_face = right_face;
	next().prev_edge = index;
	if (next().twin_edge != -1)
		next().twin().right_face = left_face;
}

void Edge::attach(Edge &edge) {
	next_edge = edge.index;
	next().left_face = left_face;
	next().right_face = right_face;
	edge.prev_edge = index;	
	if (next().twin_edge != -1)
		next().twin().right_face = left_face;
}

vec3 Edge::midpoint() const {
	return (head().position + tail().position) * 0.5;
}

void split_edge(Edge &edge, int mid, Mesh *mesh) {	
	mesh->edges.emplace_back(Edge(mid, edge.head_vertex, mesh));
	Edge &new_edge = mesh->edges.back();
	new_edge.next_edge = edge.next_edge;
	edge.next().prev_edge = new_edge.index;
	edge.head_vertex = mid;
	edge.attach(new_edge);
}

void Edge::split() {
	if (next().twin().next().twin().index == index) return;
	int mid = mesh->vertices.size();
	mesh->vertices.emplace_back(Vertex(midpoint(), mesh)); //add the midpoint
	
	split_edge(*this, mid, mesh);
	split_edge(twin(), mid, mesh);

	tie(twin(), next());
	tie(mesh->edges[index], twin().next());
}

void tie(Edge &edge, Edge &twin) {
	edge.twin_edge = twin.index;
	twin.twin_edge = edge.index;
	edge.right_face = twin.left_face;
	twin.right_face = edge.left_face;
}

Edge& create_twin(Edge &edge) {
	if (edge.twin_edge != -1) return edge.twin();
	else {
		edge.twin_edge = edge.mesh->edges.size();
		edge.mesh->edges.emplace_back(Edge(edge.head_vertex, edge.tail_vertex, edge.mesh));
		tie(edge, edge.mesh->edges.back());
		return edge.mesh->edges.back();
	}
}