#pragma once
#include "timer.h"
#include "object.h"

class Scene {
public:
	Scene();
	virtual ~Scene();
	
	mat4 Projection, View;
	vector<Object*> objects;

	virtual void Initialize();
	virtual void Update(Time time);
	virtual void Draw();
	virtual void Mouse(int button, int state, int x, int y);
	virtual void Keyboard(unsigned char key, int x, int y);	
	virtual void Keyboard(int key, int x, int y);
	virtual void Resize(int w, int h);
};

class DefaultScene : public Scene {
public:
	void Initialize();
	void Update(Time time);
	virtual void Keyboard(int key, int x, int y);

private:
	Destroyer *destroyer;
	vector<Asteroid*> asteroids;
};