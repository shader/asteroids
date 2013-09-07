#pragma once
#include "timer.h"
#include "object.h"

class Scene {
public:
	Scene();
	~Scene();
	
	mat4 Projection, View;
	vector<Object*> objects;

	void Initialize();
	void Update(Time time);
	void Draw();
	void Mouse(int button, int state, int x, int y);
	void Keyboard(unsigned char key, int x, int y);
	void Resize(int w, int h);
};

class DefaultScene : public Scene {
public:
	void Initialize();
	void Update(Time time);
};