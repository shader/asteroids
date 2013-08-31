#pragma once
#include "shader.h"

using namespace std;

Shader::Shader(void)
{
	GLuint _shaders[] = {0, 0, 0};
	_attributes.clear();
	_uniforms.clear();
}

Shader::~Shader(void)
{
	_attributes.clear();	
	_uniforms.clear();
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
	ifstream infile(filename);
	if(infile) {
		string data((istreambuf_iterator<char>(infile)), istreambuf_iterator<char>());
		infile.close();
		LoadString(shader, data);
	} else {
		cerr << "Error loading shader: " << filename.c_str() << endl;
	}
}

void Shader::Create() {
	_program = glCreateProgram ();
	ShaderMap::iterator cur;

	//attach all loaded shaders
	for (cur = _shaders.begin(); cur != _shaders.end(); ++cur) {
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
	for (cur = _shaders.begin(); cur != _shaders.end(); ++cur) {
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
	GLuint value = _attributes[attribute];
	if (value == 0) {
		value = _attributes[attribute] = glGetAttribLocation(_program, attribute.c_str());
	}
	return value;
}

GLuint Shader::operator()(const string uniform){
	GLuint value = _uniforms[uniform];
	if (value == 0) {
		return _uniforms[uniform] = glGetUniformLocation(_program, uniform.c_str());
	} else {
		return value;
	}
}