#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
using namespace glm;

typedef struct {
	GLfloat x, y, z;
} Vector3;

typedef Vector3 Color;

typedef struct {
	GLuint a, b, c;
} Triangle;