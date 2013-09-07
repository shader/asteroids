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
	
	void Init();
	void LoadShader(GLenum type, string path);
	void Bind();
	void Bind(Shader* shader);
	void Bind(Shader* shader, GLenum mode);
	void Draw(mat4 ViewProjection);
	void Draw(Shader* shader, mat4 ViewProjection);
	void Draw(Shader* shader, mat4 ViewProjection, GLenum mode);

	Mesh* mesh;
	vector<Color> colors;
	void Subdivide(int times = 1);
	void Perturb(float max_radius);

private:
	GLuint verticesID, indicesID, colorsID, vertex_array;
	GLenum draw_mode;
	Shader* shader;
	vector<GLuint> indices;
};