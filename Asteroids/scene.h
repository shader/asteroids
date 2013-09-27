#pragma once
#include "timer.h"
#include "object.h"
#include "input.h"

class Scene {
public:
	Scene();
	virtual ~Scene();
	
	mat4 Projection, View, InvView;
	vector<Object*> objects;

	vec3 light_dir, light_color;

	virtual void Initialize();
	virtual void Update(Time time, const InputState &input);
	virtual void Draw();
	virtual void Resize(int w, int h);
};

class DefaultScene : public Scene {
public:
	void Initialize();
	void Update(Time time, const InputState &input);
	void Keyboard(const InputState &input);

private:
	Destroyer *destroyer;
	vector<Asteroid*> asteroids;
};