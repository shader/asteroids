#pragma once
#include "timer.h"
#include "object.h"
#include "input.h"
#include <stack>
#include <queue>

class Scene {
	function<void(Object*)> remover;

public:
	Scene(class SceneManager *manager);
	virtual ~Scene();
	
	SceneManager *manager;
	mat4 Projection, View, InvView;
	list<unique_ptr<Object>> objects;
	queue<function<void()>> event_queue;
	int width, height;
	InputState prev_state;

	vec3 light_dir, light_color;

	virtual void Load();
	virtual void Initialize();
	virtual void Update(Time time, const InputState &input);
	virtual void Draw();
	virtual void Resize(int w, int h);

	Object* Get(Object *obj);
	virtual void Add(Object *obj);
	virtual vector<Object*> Find(const type_info &id);
	virtual void Remove(Object *obj);
};

class SceneManager {
public:
	stack<unique_ptr<Scene>> scenes;

	Scene& current();

	void Update(Time time, const InputState &input);
	void Draw();
	void Resize(int w, int h);
	void Restart();
	void Push(Scene * scene);
private:
	queue<function<void()>> event_queue;
};

class DefaultScene : public Scene {
public:
	DefaultScene(SceneManager *manager);
	void Load();
	void Initialize();
	void Update(Time time, const InputState &input);
	void Keyboard(const InputState &input);
	Asteroid *spawn_asteroid();
	void add_asteroid(Asteroid *asteroid);

private:
	shared_ptr<Model> bullet_model;
	int asteroid_count, level;
	void process_collisions();
	LifeCounter *lives;
};