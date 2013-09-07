#include "headers.h"

Scene::Scene() {
	Projection = perspective(45.0f, (GLfloat)800/600, 1.f, 1000.f);
	View = lookAt(vec3(0,0,5), vec3(0,0,0), vec3(0,1,0));
}

Scene::~Scene() {
	for (auto o = objects.begin(); o!=objects.end(); o++) {
		delete (*o);
	}
}

void Scene::Initialize()
{	
	for (auto o = objects.begin(); o!= objects.end(); o++) {
		(*o)->Initialize();
	}
}

void Scene::Update(Time time) {}

void Scene::Draw() 
{	
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

	mat4 vp = Projection * View;
	for (auto o = objects.begin(); o!= objects.end(); o++) {
		(*o)->Draw(vp);
	}

	glutSwapBuffers();
}

void Scene::Mouse(int button, int state, int x, int y) {}
void Scene::Keyboard(unsigned char key, int x, int y) {}

//Reset viewport and projection matrix
void Scene::Resize(int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	Projection = perspective(45.0f, (GLfloat)w/h, 1.f, 1000.f);
}
