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
	Vertex(float x, float y, float z, class Mesh* mesh);
	Vertex(vec3 position, class Mesh* mesh);
	vec3 position, new_position;
	vec2 texture_coord;
	float temperature;
	set<int> edges;
	Mesh* mesh;

	vector<int> neighbors() const;
	vec3 normal() const;
	friend Vertex average(const Vertex &vertex);
	friend Vertex perturb(Vertex vertex, float max_radius);

	friend bool operator<(const Vertex &lhs, const Vertex &rhs);
	friend bool operator==(const Vertex &lhs, const Vertex &rhs);
	friend bool operator!=(const Vertex &lhs, const Vertex &rhs);
};

class Edge {
public:
	Edge(int tail, int head, Mesh* mesh);
	int index, head_vertex, tail_vertex;
	int next_edge, prev_edge, twin_edge;
	int left_face, right_face;
	Mesh* mesh;
	
	friend bool operator<(const Edge &lhs, const Edge &rhs);
	friend bool operator==(const Edge &lhs, const Edge &rhs);
	friend bool operator!=(const Edge &lhs, const Edge &rhs);

	void attach(Edge &edge);
	void attach(int edge);
	void split();
	Vertex& head() const, &tail() const;
	Edge &next() const, &prev() const, &twin() const;
	class Face &left() const, &right() const;

	friend void tie(Edge &edge, Edge &twin);
	friend Edge& create_twin(Edge &edge);

	vec3 midpoint() const;
	void init();
};

class Face {
public:
	Face();
	Face(int first, Mesh* mesh);
	Face(int a, int b, int c, Mesh* mesh);

	int first_edge, index;
	class Mesh* mesh;

	vector<int> neighbors();
	Edge &first() const;

	friend bool operator<(const Face &lhs, const Face &rhs);
	friend bool operator==(const Face &lhs, const Face &rhs);
	friend bool operator!=(const Face &lhs, const Face &rhs);

	vec3 midpoint() const;
	vec3 normal() const;
	void split();
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