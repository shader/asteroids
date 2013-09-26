#pragma once
#include "model.h"
#include "timer.h"
#include "ship.h"
#include "rock.h"
#include "GL\freeglut.h"

class Scene;
class Object {
public:
	vec3 position, size, velocity, angular_vel;
	quat orientation;
	vector<Model*> models;
	Scene *scene;

	Object(Scene *scene);
	virtual ~Object();
	virtual void Initialize();
	virtual void Draw(mat4 view, mat4 projection);
	virtual void Update(Time time);
};

class Destroyer : public Object {
public:
	Destroyer(Scene *scene);
	void Initialize();
	void Update(Time time);

private:
	Ship* ship;
};

class Asteroid : public Object {
public:
	Asteroid(Scene *scene);
	void Initialize();
	void Update(Time time);

private:
	Rock* rock;
};

class Bullet : public Object {
public:
	Bullet(Scene *scene);
	void Initialize();
	void Update(Time time);

private:
	Model* model;
};