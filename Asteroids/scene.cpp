#include "headers.h"

Scene::Scene(SceneManager *manager) {
	this->manager = manager;
	Projection = perspective(45.0f, (GLfloat)800/600, 1.f, 1000.f);
	View = lookAt(vec3(0,0,20), vec3(0,0,0), vec3(0,1,0));
	InvView = inverse(View);
	remover = function<void(Object*)>([&](Object *obj){ this->Remove(obj); });
}

Scene::~Scene() {
	for (auto obj = objects.begin(); obj!=objects.end(); obj++) {
		(*obj)->destroyed.clear();
	}
}

vector<Object*> Scene::Find(const type_info &id) {
	vector<Object*> objs;
	for (auto obj = objects.begin(); obj!=objects.end(); obj++) {
		if (typeid(**obj) == id) {
			objs.push_back(obj->get());
		}
	}
	return objs;
}

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
}

void Scene::Update(Time time, const InputState &input)
{
	auto o = objects.begin();
	while (o != objects.end()) {
		auto obj = (o++)->get();
		if (obj->flags[ObjectFlags::Enabled] && obj->flags[ObjectFlags::Update])
			obj->Update(time, input);
	}

	while(!event_queue.empty()) {
		event_queue.front()();
		event_queue.pop();
	}
}

void Scene::Keyboard(const InputState &input, const InputState &prev) {}

void Scene::Draw() 
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 vp = Projection * View;
	for (auto o = objects.begin(); o!= objects.end(); o++) {
		if ((*o)->flags[ObjectFlags::Enabled] && (*o)->flags[ObjectFlags::Draw])
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
