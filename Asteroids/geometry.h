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
	Vertex();
	Vertex(float x, float y, float z);
	Vertex(vec3 position);
	Vertex(vec3 position, GLuint index);
	vec3 position, new_position;
	vec2 texture_coord;
	float temperature;
	GLuint index;
	set<class Edge*> edges;

	vector<Vertex*> neighbors() const;
	vec3 normal() const;
	friend Vertex average(const Vertex &vertex);
	friend Vertex perturb(Vertex vertex, float max_radius);

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
	void split(vector<Edge> &edges, vector<Vertex> &vertices);
	friend pair<Edge*, Edge*> split(Edge *edge);
	friend void tie(Edge* edge, Edge* twin);
	friend Edge* twin(Edge *edge);
	friend Edge average(const Edge &edge);
	friend Edge perturb(const Edge &edge, float max_radius);

	vec3 midpoint() const;
	void init();
};

class Face {
public:
	Face();
	Face(Edge* first);
	Face(Edge *a, Edge *b, Edge *c);
	Face(vec3 a, vec3 b, vec3 c);

	class Edge *first;

	vector<Face*> neighbors();

	friend bool operator<(const Face &lhs, const Face &rhs);
	friend bool operator==(const Face &lhs, const Face &rhs);
	friend bool operator!=(const Face &lhs, const Face &rhs);

	vec3 midpoint() const;
	vec3 normal() const;
	void split(vector<Face> &faces, vector<Edge> &edges, vector<Vertex> &vertices);
	friend vector<Edge*> split_edges(Face *face);
	friend vector<Face*> split(Face *face);
	friend Face average(const Face &face);
	friend Face perturb(const Face &face, float max_radius);
};

class Mesh {
public:
	~Mesh();
	
	vector<Vertex> vertices;
	vector<Edge> edges;
	vector<Face> faces;
	
	void Perturb(float max_radius);
	void Normalize();
	void LoadTriangles(uint* triangles, int triangle_count);

	friend Mesh* split(Mesh* mesh);
	friend Mesh* average(Mesh* mesh);
	friend Mesh* perturb(Mesh* mesh, float max_radius);
	friend Mesh* subdivide(Mesh *mesh);
};

//loop averaging functions
double alpha(GLuint n);
double beta(GLuint n);