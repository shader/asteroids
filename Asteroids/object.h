#pragma once
#include "model.h"
#include "timer.h"
#include "ship.h"
#include "rock.h"
#include "GL\freeglut.h"

class Object {
public:
	vec3 position, size, velocity, angular_vel;
	quat orientation;
	vector<Model*> models;

	Object();
	virtual ~Object();
	virtual void Initialize();
	virtual void Draw(mat4 view_projection);
	virtual void Update(Time time);
};

class Destroyer : public Object {
public:
	Destroyer();
	void Initialize();
	void Update(Time time);

private:
	Ship* ship;
};

class Asteroid : public Object {
public:
	Asteroid();
	void Initialize();
	void Update(Time time);

private:
	Rock* rock;
};

class Bullet : public Object {
public:
	Bullet();
	void Initialize();
	void Update(Time time);

private:
	Model* model;
};