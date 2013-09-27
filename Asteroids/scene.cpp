#include "headers.h"

Scene::Scene() {
	Projection = perspective(45.0f, (GLfloat)800/600, 1.f, 1000.f);
	View = lookAt(vec3(0,0,20), vec3(0,0,0), vec3(0,1,0));
	InvView = inverse(View);
	remover = function<void(Object*)>([&](Object *obj){ this->Remove(obj); });
}

Scene::~Scene() {
	auto o = objects.begin();
	while (o != objects.end()) {
		delete (*o++);
	}
}

void Scene::Add(Object *obj) {
	obj->destroyed += this->remover;
	objects.push_back(obj);
}

void Scene::Remove(Object *obj) {
	objects.remove(obj);
}

void Scene::Initialize()
{	
	for (auto o = objects.begin(); o!= objects.end(); o++) {
		(*o)->Initialize();
	}
}

void Scene::Update(Time time, const InputState &input)
{
	auto o = objects.begin();
	while (o != objects.end()) {
		(*o++)->Update(time);
	}
}

void Scene::Draw() 
{	
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

	mat4 vp = Projection * View;
	for (auto o = objects.begin(); o!= objects.end(); o++) {
		(*o)->Draw(View, Projection);
	}

	glutSwapBuffers();
}

//Reset viewport and projection matrix
void Scene::Resize(int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	Projection = perspective(45.0f, (GLfloat)w/h, 1.f, 1000.f);
}
