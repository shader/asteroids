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
#include "primitives.h"

using namespace glm;
using namespace std;

struct Material {
	vec4 color, ambient, diffuse, specular;
	float shininess;
};

class Model
{
public:
	Model(ShaderType shader = basic);
	Model(Mesh &mesh, ShaderType shader = basic);
	~Model();

	vec3 position, size;
	quat orientation;
	float radius;
	Material material;
	ShaderType shader_type;
	
	void Init();
	void LoadShader(GLenum type, string path);
	void LoadShaders(string vertex, string fragment);
	void GenBuffers(Shader& shader);
	void DeleteBuffers();

	void Bind();
	void Bind(GLenum mode);
	void Bind(Shader& shader, GLenum mode);

	void Draw(mat4 view, mat4 projection);
	void Draw(Shader& shader, mat4 view, mat4 projection);
	void Draw(Shader& shader, mat4 view, mat4 projection, GLenum mode);

	Mesh mesh;

private:
	GLuint vertex_array, verticesID, texcoordsID, indicesID, normalsID, materialID;
	GLenum draw_mode;
	vector<GLuint> indices;
};

struct ModelType {
	enum Type {
		sphere,
		rock,
	};
};