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
	Box BoundingBox();
	Box WorldBox();
	void BoundingVolumes();

	Object(Scene *scene);
	virtual ~Object();
	virtual void Load();
	virtual void Initialize();

	virtual void Draw(mat4 view, mat4 projection);
	virtual void DrawBox(mat4 view, mat4 projection);
	virtual void DrawSphere(mat4 view, mat4 projection, vec4 color=vec4(0,0,1,0.5));

	virtual void Update(Time time, InputState const &input);
	virtual function<void()> Collision(Object &other);

private:
	void UpdateWorldBox();
	LineBox box;
	Box world_box;
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
public:
	Alien(Scene *scene);
	void Fire();
	void Evade();
	void Load();
	void Initialize();
	void Update(Time time, InputState const &input);
	function<void()> Collision(Object &other);

	Event<void> Killed;
protected:	
	float evasiveness;
	float speed;
	int value;
	discrete_distribution<int> target_distribution;

private:
	float age;
};

class BigAlien : public Alien {
public:
	BigAlien(Scene *scene);
};

class SmallAlien : public Alien {
public:
	SmallAlien(Scene *scene);
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
	System system;

	Explosion(Scene *scene, vec4 start_color, vec4 end_color = vec4(0));
	~Explosion();
	vector<Particle> particles;
	void Initialize();
	void Update(Time time, InputState const &input);
	void Draw(mat4 view, mat4 projection);

private:
	GLuint vertex_array, verticesID, indicesID, systemID;
	vector<vec3> vertices;
	vector<GLuint> indices;
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
	ScoreBoard(Scene *scene);
	~ScoreBoard();
	void Initialize();
	void Draw(mat4 view, mat4 projection);

	Texture texture;
	Event<int> Score;
	int Points();
private:
	int score;
	GLuint vertex_array, verticesID, indicesID, texcoordsID;
};