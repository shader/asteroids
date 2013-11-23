#pragma once
#include "model.h"
#include "timer.h"
#include "event.h"
#include "GL\freeglut.h"
#include "memory"
#include "particles.h"
#include "input.h"
#include "texture.h"
#include <random>
#include <bitset>

class Scene;

struct ObjectFlags {
	enum Flags {
		Enabled,
		Draw,
		Update,
		Collide,
		Wrap,
	};
};

class Object {
public:
	bitset<5> flags;
	vec3 position, size, velocity, angular_vel;
	quat orientation;
	vector<shared_ptr<Model>> models;
	Scene *scene;
	Event<Object*> destroyed;
	float radius;

	float BoundingSphere();

	Object(Scene *scene);
	virtual ~Object();
	virtual void Load();
	virtual void Initialize();
	virtual void Draw(mat4 view, mat4 projection);
	virtual void Update(Time time, InputState const &input);
	virtual function<void()> Collision(Object &other);
};

class Destroyer : public Object {
public:
	Destroyer(Scene *scene);
	void Load();
	void Initialize();
	void Update(Time time, InputState const &input);
	function<void()> Collision(Object &other);

	Event<void> Killed;
private:
	int bullet_count;
	InputState prev_state;
};

class Alien : public Object {
	float evasiveness;
	int value;
	discrete_distribution<int> target_distribution;

public:
	Alien(Scene *scene);
	void Fire();
	void Evade();
	void Load();
	void Initialize();
	void Update(Time time, InputState const &input);
	function<void()> Collision(Object &other);

	Event<void> Killed;
private:
	float age;
};

class Asteroid : public Object {
public:
	float value;

	Asteroid(Scene *scene);
	void Load();
	void Initialize();
	void Update(Time time, InputState const &input);
	function<void()> Collision(Object &other);
	void Split();
};

class Bullet : public Object {
public:
	float speed;

	Bullet(Scene *scene, Object* source);
	Object* source;
	void Initialize();
	void Update(Time time, InputState const &input);
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
	void Update(Time time, InputState const &input);
	void Draw(mat4 view, mat4 projection);

private:
	GLuint verticesID, indicesID, vertex_array;
	vector<vec3> vertices;
	vector<GLuint> indices;
	double age;
};

class LifeCounter : public Object {
public:
	Event<void> GameOver;

	LifeCounter(Scene *scene, int lives = 3);

	void Initialize();
	void Update(Time time, InputState const &input);
	void Draw(mat4 view, mat4 projection);
	void Die();

private:
	int lives, max_lives;
	Model icon;
	quat rotation;
};

class Background : public Object {
public:
	Background(Scene *scene);
	void Draw(mat4 view, mat4 projection);
	Texture texture;
	Model quad;
};

class ScoreBoard : public Object {
public:
	int score;
	ScoreBoard(Scene *scene);
	void Draw(mat4 view, mat4 projection);
	Texture texture;
	Model quad;
};