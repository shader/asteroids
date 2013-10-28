#pragma once
#include "shader.h"

using namespace std;

Shader::Shader() {}

Shader::~Shader()
{
	glDeleteProgram(_program);
}

void Shader::LoadString(GLenum type, const string source) {	
	GLuint shader = glCreateShader (type);

	const char * sp = source.c_str();
	glShaderSource (shader, 1, &sp, NULL);
	
	GLint status;
	glCompileShader (shader);
	glGetShaderiv (shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint length;
		glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &length);
		GLchar *log = new GLchar[length];
		glGetShaderInfoLog (shader, length, NULL, log);
		cerr << "Compile log: " << log << endl;
		delete [] log;
	}

	_shaders[type] = shader;
}

void Shader::Load(GLenum shader, const string filename){
	ifstream infile("resources/"+filename);
	if(infile) {
		string data((istreambuf_iterator<char>(infile)), istreambuf_iterator<char>());
		infile.close();
		LoadString(shader, data);
	} else {
		cerr << "Error loading shader: " << filename.c_str() << endl;
	}
}

void Shader::Create() {
	_program = glCreateProgram();

	//attach all loaded shaders
	for (auto cur = _shaders.begin(); cur != _shaders.end(); ++cur) {
		glAttachShader (_program, cur->second);
	}

	//link and check whether the program links fine
	GLint status;
	glLinkProgram (_program);
	glGetProgramiv (_program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint length;		
		glGetProgramiv (_program, GL_INFO_LOG_LENGTH, &length);
		GLchar *log= new GLchar[length];
		glGetProgramInfoLog (_program, length, NULL, log);
		cerr << "Link log: " << log << endl;
		delete [] log;
	}

	//delete shaders
	for (auto cur = _shaders.begin(); cur != _shaders.end(); ++cur) {
		glDeleteShader(cur->second);
	}
}

void Shader::Begin() {
	glUseProgram(_program);
}

void Shader::End() {
	glUseProgram(0);
}

//An indexer that returns the location of the attribute
GLuint Shader::operator [](const string attribute) {
	auto it = _attributes.find(attribute);
	if (it != _attributes.end()) {
		return it->second;
	} else {
		GLuint location = glGetAttribLocation(_program, attribute.c_str());
		_attributes[attribute] = location;
		return location;
	}
}

GLuint Shader::operator()(const string uniform){
	auto it = _uniforms.find(uniform);
	if (it != _uniforms.end()) {
		return it->second;
	} else {
		return _uniforms[uniform] = glGetUniformLocation(_program, uniform.c_str());
	}
}