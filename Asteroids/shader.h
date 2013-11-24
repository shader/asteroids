#pragma once

#include <GL/glew.h>
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <list>

using namespace std;

class Shader
{
public:
	Shader();
	~Shader();
	void LoadString(GLenum shader, const string source);
	void Load(GLenum shader, const string filename);
	void Load(string vertex, string fragment);
	void Create();
	void Begin();
	void End();

	//An indexer that returns the location of the attribute/uniform
	GLuint operator[](const string attribute);
	GLuint operator()(const string uniform);
	
	GLuint Attribute(string attribute);
	GLuint Uniform(string uniform);
	GLuint UniformBlockIndex(string uniform);
	void BindBlock(string uniform, GLuint binding_point);

private:
	GLuint	_program;
	map<GLenum,GLuint> _shaders;
	map<string,GLuint> _attributes;
	map<string,GLuint> _uniforms;
	map<string,GLuint> _uniform_blocks;
};

enum ShaderType {
	basic,
	explosion,
	textured,
	polygon,
};