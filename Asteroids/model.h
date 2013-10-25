#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <map>
#include <string>
#include "shader.h"
#include "geometry.h"

using namespace glm;
using namespace std;

class Model
{
public:
	Model();
	~Model();

	vec3 position, size;
	quat orientation;
	float radius;
	Shader* shader;
	
	void Init();
	void LoadShader(GLenum type, string path);
	void LoadShaders(string vertex, string fragment);
	void Bind();
	void Bind(Shader* shader);
	void Bind(Shader* shader, GLenum mode);
	void Draw(mat4 view, mat4 projection);
	void Draw(Shader* shader, mat4 view, mat4 projection);
	void Draw(Shader* shader, mat4 view, mat4 projection, GLenum mode);

	Mesh* mesh;
	vector<Color> colors;
	void Subdivide(int times = 1);
	void Perturb(float max_radius);

private:
	GLuint verticesID, indicesID, colorsID, normalsID, vertex_array;
	GLenum draw_mode;
	vector<GLuint> indices;
};