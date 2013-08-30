#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "shader.h"

using namespace glm;
using namespace std;

typedef struct {
	GLfloat x, y, z;
} Vector3;

typedef Vector3 Color;

typedef struct {
	GLuint a, b, c;
} Triangle;

class Model
{
public:
	Model();
	~Model();

	vec3 position;
	quat orientation;
	
	void Init();
	void Bind(Shader* shader);
	void Bind(Shader* shader, GLenum mode);
	void Draw(Shader* shader, mat4 ViewProjection);
	void Draw(Shader* shader, mat4 ViewProjection, GLenum mode);

	vector<vec3> vertices;
	vector<Color> colors;
	vector<Triangle> triangles;
	vector<GLuint> indices;

private:
	GLuint verticesID, indicesID, colorsID, vertex_array;
	GLenum draw_mode;
};