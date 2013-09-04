#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <set>
#include <vector>
#include <list>
#include <memory>
using namespace glm;
using namespace std;

typedef vec3 Color;

class Vertex {
public:
	Vertex(vec3 position);
	Vertex(vec3 position, GLuint index);
	vec3 position;
	GLuint index;
	set<weak_ptr<class Edge>> edges;

	vector<shared_ptr<Vertex>> neighbors();

	friend bool operator<(const Vertex &lhs, const Vertex &rhs);
	friend bool operator==(const Vertex &lhs, const Vertex &rhs);
	friend bool operator!=(const Vertex &lhs, const Vertex &rhs);
};

class Edge {
public:
	Edge(shared_ptr<Vertex> tail, shared_ptr<Vertex> head);
	Edge(vec3 tail, vec3 head);
	~Edge();
	shared_ptr<class Vertex> head, tail;
	weak_ptr<class Edge> next, prev, twin;
	weak_ptr<class Face> left, right;
	
	friend bool operator<(const Edge &lhs, const Edge &rhs);
	friend bool operator==(const Edge &lhs, const Edge &rhs);
	friend bool operator!=(const Edge &lhs, const Edge &rhs);

	friend void attach(shared_ptr<Edge> a, shared_ptr<Edge> b);
	friend pair<shared_ptr<Edge>, shared_ptr<Edge>> split(shared_ptr<Edge> edge);
	friend void tie(shared_ptr<Edge> edge, shared_ptr<Edge> twin);
	friend shared_ptr<Edge> twin(shared_ptr<Edge> edge);

	vec3 midpoint() const;
private:
	void init(shared_ptr<Vertex> tail, shared_ptr<Vertex> head);
};

class Face {
public:
	Face(shared_ptr<Edge> first);
	Face(shared_ptr<Edge> a, shared_ptr<Edge> b, shared_ptr<Edge> c);
	Face(vec3 a, vec3 b, vec3 c);
	shared_ptr<class Edge> first;

	vector<shared_ptr<Face>> neighbors();

	friend bool operator<(const Face &lhs, const Face &rhs);
	friend bool operator==(const Face &lhs, const Face &rhs);
	friend bool operator!=(const Face &lhs, const Face &rhs);

	vec3 midpoint() const;
	friend vector<shared_ptr<Edge>> split_edges(shared_ptr<Face> face);
	friend vector<shared_ptr<Face>> split(shared_ptr<Face> face);
};

class Mesh {
public:
	Mesh();
	~Mesh();
	set<shared_ptr<Vertex>, bool(*)(shared_ptr<Vertex>, shared_ptr<Vertex>)> *vertices;
	set<shared_ptr<Edge>, bool(*)(shared_ptr<Edge>, shared_ptr<Edge>)> *edges;
	set<shared_ptr<Face>, bool(*)(shared_ptr<Face>, shared_ptr<Face>)> *faces;

	shared_ptr<Vertex> AddVertex(vec3 position);
	shared_ptr<Edge> AddEdge(vec3 a, vec3 b);
	shared_ptr<Face> AddFace(vec3 a, vec3 b, vec3 c);

	shared_ptr<Vertex> add(shared_ptr<Vertex> vertex);
	shared_ptr<Edge> add(shared_ptr<Edge> edge);
	shared_ptr<Face> add(shared_ptr<Face> face);

	void erase(shared_ptr<Vertex> vert);
	void erase(shared_ptr<Edge> edge);
	void erase(shared_ptr<Face> face);

	friend shared_ptr<Mesh> split(shared_ptr<Mesh> mesh);

	void subdivide(int times);
};