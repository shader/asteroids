#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <set>
#include <vector>
#include <list>
#define _USE_MATH_DEFINES
#include <math.h>
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
	friend Vertex* average(Vertex *vertex);
	friend Vertex *perturb(Vertex *vertex, float max_radius);

	friend bool operator<(const Vertex &lhs, const Vertex &rhs);
	friend bool operator==(const Vertex &lhs, const Vertex &rhs);
	friend bool operator!=(const Vertex &lhs, const Vertex &rhs);
};

class Edge {
public:
	Edge(Vertex *tail, Vertex *head);
	Edge(vec3 tail, vec3 head);
	~Edge();
	class Vertex *head, *tail;
	class Edge *next, *prev, *twin;
	class Face *left, *right;
	
	friend bool operator<(const Edge &lhs, const Edge &rhs);
	friend bool operator==(const Edge &lhs, const Edge &rhs);
	friend bool operator!=(const Edge &lhs, const Edge &rhs);

	void attach(Edge* edge);
	friend pair<Edge*, Edge*> split(Edge *edge);
	friend void tie(Edge* edge, Edge* twin);
	friend Edge* twin(Edge *edge);
	friend Edge* average(Edge *edge);
	friend Edge* perturb(Edge *edge, float max_radius);

	vec3 midpoint() const;
private:
	void init(Vertex *tail, Vertex *head);
};

class Face {
public:
	Face(Edge* first);
	Face(Edge *a, Edge *b, Edge *c);
	Face(vec3 a, vec3 b, vec3 c);
	class Edge *first;

	vector<Face*> neighbors();

	friend bool operator<(const Face &lhs, const Face &rhs);
	friend bool operator==(const Face &lhs, const Face &rhs);
	friend bool operator!=(const Face &lhs, const Face &rhs);

	vec3 midpoint() const;
	friend vector<Edge*> split_edges(Face *face);
	friend vector<Face*> split(Face *face);
	friend Face* average(Face *face);
	friend Face* perturb(Face *face, float max_radius);

};

class Mesh {
public:
	Mesh();
	~Mesh();
	set<Vertex*,bool(*)(Vertex*,Vertex*)> *vertices;
	set<Edge*,bool(*)(Edge*,Edge*)> *edges;
	set<Face*,bool(*)(Face*,Face*)> *faces;

	Vertex* AddVertex(vec3 position);
	Edge* AddEdge(vec3 a, vec3 b);
	Face* AddFace(vec3 a, vec3 b, vec3 c);

	Vertex* add(Vertex* vertex);
	Edge* add(Edge* edge);
	Face* add(Face* face);

	void erase(Vertex* vert);
	void erase(Edge* edge);
	void erase(Face* face);
	void Perturb(float max_radius);
	void Normalize();

	friend Mesh* split(Mesh* mesh);
	friend Mesh* average(Mesh* mesh);
	friend Mesh* perturb(Mesh* mesh, float max_radius);
	friend Mesh* subdivide(Mesh *mesh);
};

//loop averaging functions
GLfloat alpha(GLuint n);
GLfloat beta(GLuint n);