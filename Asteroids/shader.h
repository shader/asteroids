#pragma once

#include <GL/glew.h>
#include <iostream>
#include <string>
#include <fstream>
#include <map>

using namespace std;

typedef map<GLenum,GLuint> ShaderMap;

class Shader
{
public:
	Shader(void);
	~Shader(void);
	void LoadString(GLenum shader, const string source);
	void Load(GLenum shader, const string filename);
	void Create();
	void Begin();
	void End();

	//An indexer that returns the location of the attribute/uniform
	GLuint operator[](const string attribute);
	GLuint operator()(const string uniform);

private:
	GLuint	_program;
	ShaderMap _shaders;
	map<string,GLuint> _attributes;
	map<string,GLuint> _uniforms;
};	