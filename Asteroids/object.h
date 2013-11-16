#pragma once
#include "model.h"
#include "timer.h"
#include "ship.h"
#include "rock.h"
#include "event.h"
#include "GL\freeglut.h"
#include "memory"
#include "particles.h"

class Scene;
class Object {
public:
	vec3 position, size, velocity, angular_vel;
	quat orientation;
	vector<shared_ptr<Model>> models;
	Scene *scene;
	Event<Object*> destroyed;
	float radius;

	Object(Scene *scene);
	virtual ~Object();
	virtual void Load();
	virtual void Initialize();
	virtual void Draw(mat4 view, mat4 projection);
	virtual void Update(Time time);
	virtual function<void()> Collision(Object &other);
};

class Destroyer : public Object {
public:
	Destroyer(Scene *scene);
	void Load();
	void Initialize();
	void Update(Time time);
	function<void()> Collision(Object &other);
};

class Asteroid : public Object {
public:
	Asteroid(Scene *scene);
	void Load();
	void Initialize();
	void Update(Time time);
	function<void()> Collision(Object &other);
	void Split();
};

class Bullet : public Object {
public:
	Bullet(Scene *scene);
	void Initialize();
	void Update(Time time);
	function<void()> Collision(Object &other);

private:
	double age;
};

class Explosion : public Object {
public:
	Explosion(Scene *scene);
	~Explosion();
	vector<Particle> particles;
	void Initialize();
	void Update(Time time);
	void Draw(mat4 view, mat4 projection);

private:
	GLuint verticesID, indicesID, vertex_array;
	vector<vec3> vertices;
	vector<GLuint> indices;
	double age;
};