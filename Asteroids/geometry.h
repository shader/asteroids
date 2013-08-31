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
};

class Face {
public:
	Face(Edge* first);
	class Edge *first;

	vector<Face*> neighbors();

	friend bool operator<(const Face &lhs, const Face &rhs);
	friend bool operator==(const Face &lhs, const Face &rhs);
	friend bool operator!=(const Face &lhs, const Face &rhs);
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
};