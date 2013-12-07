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
	list<shared_ptr<Object>> objects;
	queue<function<void()>> event_queue;
	int width, height;

	vec3 light_dir, light_color;

	virtual void Load();
	virtual void Initialize();
	virtual void Update(Time time, const InputState &input);
	virtual void Draw();
	virtual void Resize(int w, int h);
	virtual void Keyboard(const InputState &input, const InputState &prev);

	virtual void Add(Object *obj);
	virtual vector<Object*> Find(const type_info &id);
	virtual void Remove(Object *obj);

	template<typename T>
	T* Get(T* obj = nullptr) {
		for (auto o = objects.begin(); o!=objects.end(); o++) {
			if (obj && obj == o->get()) return obj;
			else if (auto r = dynamic_cast<T*>(o->get())) return r;
		}
		return nullptr;
	}
};

class SceneManager {
public:
	bool pause;
	InputState prev_state;
	stack<unique_ptr<Scene>> scenes;
	int width, height;
	Scene& current();

	void Update(Time time, const InputState &input);
	void Draw();
	void Resize(int w, int h);
	void Restart();
	void Push(Scene * scene);
	void Pop();
private:
	queue<function<void()>> event_queue;
};

class DefaultScene : public Scene {
public:
	DefaultScene(SceneManager *manager);
	void Load();
	void Initialize();
	void Update(Time time, const InputState &input);
	void Draw();
	void Keyboard(const InputState &input, const InputState &prev);
	Asteroid *spawn_asteroid();
	void add_asteroid(Asteroid *asteroid);

private:
	shared_ptr<Model> bullet_model;
	int asteroid_count, level;
	void process_collisions();
	LifeCounter *lives;
	ScoreBoard *score_board;

	bool draw_boxes, draw_spheres;
};

class AsteroidScene : public Scene {
public:
	AsteroidScene(SceneManager *manager);
	void Load();
	void Initialize();
	void Keyboard(const InputState &input, const InputState &prev);

private:
	GLenum mode;
	float radius, rate;
	bool lit;
	unique_ptr<Object> asteroid;
	shared_ptr<Model> asteroid_model;
};