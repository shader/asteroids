//Master include file to collect all headers for main/others

#ifdef _M_IX86
#include <windows.h>
#else
#include <stream.h>
#endif

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>
#include <map>
#include <vector>

// OpenGL
#include <GL/glew.h>

//GLUT
#include <GL/freeglut.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//local
#include "utils.h"
#include "shader.h"
#include "model.h"
#include "menu.h"
#include "geometry.h"
#include "icosahedron.h"