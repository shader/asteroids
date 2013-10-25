#pragma once
#include "model.h"
#include "timer.h"
#include "ship.h"
#include "rock.h"
#include "event.h"
#include "GL\freeglut.h"
#include "memory"

class Scene;
class Object {
public:
	vec3 position, size, velocity, angular_vel;
	quat orientation;
	vector<Model*> models;
	Scene *scene;
	Event<Object*> destroyed;
	float radius;

	Object(Scene *scene);
	virtual ~Object();
	virtual void Initialize();
	virtual void Draw(mat4 view, mat4 projection);
	virtual void Update(Time time);
	virtual function<void()> Collision(Object &other);
};

class Destroyer : public Object {
public:
	Destroyer(Scene *scene);
	void Initialize();
	void Update(Time time);
	function<void()> Collision(Object &other);

private:
	Ship* ship;
};

class Asteroid : public Object {
public:
	Asteroid(Scene *scene);
	void Initialize();
	void Update(Time time);
	function<void()> Collision(Object &other);
	void Split();

private:
	Rock* rock;
};

class Bullet : public Object {
public:
	Bullet(Scene *scene);
	void Initialize();
	void Update(Time time);
	function<void()> Collision(Object &other);

private:
	Model* model;
	double age;
};