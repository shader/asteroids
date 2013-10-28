#include "headers.h"

Scene::Scene(SceneManager *manager) {
	this->manager = manager;
	Projection = perspective(45.0f, (GLfloat)800/600, 1.f, 1000.f);
	View = lookAt(vec3(0,0,20), vec3(0,0,0), vec3(0,1,0));
	InvView = inverse(View);
	remover = function<void(Object*)>([&](Object *obj){ this->Remove(obj); });
}

Scene::~Scene() {}

void Scene::Add(Object *obj) {
	obj->destroyed += this->remover;
	objects.push_back(move(unique_ptr<Object>(obj)));
}

void Scene::Remove(Object *obj) {
	objects.remove_if([=](unique_ptr<Object> const &p){ return p.get() == obj; });
}

void Scene::Load() {
	for (auto o = objects.begin(); o!= objects.end(); o++) {
		(*o)->Load();
	}
};

void Scene::Initialize()
{
	for (auto o = objects.begin(); o!= objects.end(); o++) {
		(*o)->Initialize();
	}
	event_queue.clear();
}

void Scene::Update(Time time, const InputState &input)
{
	int w = glutGet (GLUT_WINDOW_WIDTH);
	int h = glutGet (GLUT_WINDOW_HEIGHT);
	if (w != width || h != height) {
		Resize(w, h);
	}

	auto o = objects.begin();
	while (o != objects.end()) {
		(*o++)->Update(time);
	}

	prev_state = input;
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
	width = w;
	height = h;
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	Projection = perspective(45.0f, (GLfloat)w/h, 1.f, 1000.f);
}
