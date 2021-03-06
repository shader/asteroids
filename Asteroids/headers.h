//Master include file to collect all headers for main/others


#ifdef _M_IX86
#define NOMINMAX
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
#include <list>
#include <algorithm>
#include <time.h>

// OpenGL
#include <GL/glew.h>

//GLUT
#include <GL/freeglut.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//local
#include "content.h"
#include "utils.h"
#include "timer.h"
#include "input.h"
#include "shader.h"
#include "scene.h"
#include "model.h"
#include "geometry.h"
#include "primitives.h"