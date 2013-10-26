#pragma once
#include "timer.h"
#include "object.h"
#include "input.h"

class Scene {
	function<void(Object*)> remover;

public:
	Scene();
	virtual ~Scene();
	
	mat4 Projection, View, InvView;
	list<unique_ptr<Object>> objects;
	int width, height;
	InputState prev_state;

	vec3 light_dir, light_color;

	virtual void Initialize();
	virtual void Update(Time time, const InputState &input);
	virtual void Draw();
	virtual void Resize(int w, int h);

	virtual void Add(Object *obj);
	virtual void Remove(Object *obj);
};

class DefaultScene : public Scene {
public:
	void Initialize();
	void Update(Time time, const InputState &input);
	void Keyboard(const InputState &input);
	Asteroid *spawn_asteroid();
	void add_asteroid(Asteroid *asteroid);

private:
	Destroyer *destroyer;
	int bullet_count, asteroid_count;
	void process_collisions();
};