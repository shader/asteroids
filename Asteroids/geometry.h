#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <set>
#include <vector>
using namespace glm;
using namespace std;

typedef vec3 Color;

class Vertex {
public:
	Vertex(vec3 position);
	Vertex(vec3 position, GLuint index);
	vec3 position;
	GLuint index;
	set<class Edge*> edges;

	vector<Vertex*> neighbors();

	friend bool operator<(const Vertex &lhs, const Vertex &rhs);
	friend bool operator==(const Vertex &lhs, const Vertex &rhs);
	friend bool operator!=(const Vertex &lhs, const Vertex &rhs);
};

class Edge {
public:
	Edge(Vertex *head, Vertex *tail);
	class Vertex *head, *tail;
	class Edge *next, *prev, *twin;
	class Face *left, *right;
	
	friend bool operator<(const Edge &lhs, const Edge &rhs);
	friend bool operator==(const Edge &lhs, const Edge &rhs);
	friend bool operator!=(const Edge &lhs, const Edge &rhs);

	void attach(Edge* edge);
	pair<Edge*, Edge*> split();
	friend void twin(Edge* edge, Edge* twin);
	friend pair<Edge*, Edge*> split(Edge* edge);

	vec3 midpoint() const;
};

class Face {
public:
	Face(Edge* first);
	class Edge *first;

	vector<Face*> neighbors();

	friend bool operator<(const Face &lhs, const Face &rhs);
	friend bool operator==(const Face &lhs, const Face &rhs);
	friend bool operator!=(const Face &lhs, const Face &rhs);

	vec3 midpoint() const;
};

class Mesh {
public:
	Mesh();
	set<Vertex*,bool(*)(Vertex*,Vertex*)> vertices;
	set<Edge*,bool(*)(Edge*,Edge*)> edges;
	set<Face*,bool(*)(Face*,Face*)> faces;

	Vertex* AddVertex(vec3 position);
	Edge* AddEdge(vec3 a, vec3 b);
	Face* AddFace(vec3 a, vec3 b, vec3 c);

	Vertex* add(Vertex* vertex);
	Edge* add(Edge* edge);
	Face* add(Face* face);

	void erase(Vertex* vert);
	void erase(Edge* edge);
	void erase(Face* face);

	void split(Edge* edge);
	void split_all();
	void subdivide(int times);
};