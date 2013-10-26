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
	void Create();
	void Begin();
	void End();

	//An indexer that returns the location of the attribute/uniform
	GLuint operator[](const string attribute);
	GLuint operator()(const string uniform);

private:
	GLuint	_program;
	map<GLenum,GLuint> _shaders;
	map<string,GLuint> _attributes;
	map<string,GLuint> _uniforms;
};

class ShaderManager {
public:
	Shader& GetShader(string name);
	list<Shader> shaders;
};