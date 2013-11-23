#include "object.h"
#include "utils.h"
#include "model.h"
#include "scene.h"
#include "content.h"

Alien::Alien(Scene *scene) : Object(scene) {
	Model* model = new Model(Content::mesh(alien));
	models.push_back(shared_ptr<Model>(model));
	evasiveness = 1.0f;
	target_distribution = discrete_distribution<int>(3, 0, 3, [](double d)->double{
		d = floor(d);
		if (d == 0) return 0.3;
		if (d == 1) return 0.2;
		if (d == 2) return 0.5;
		else return 0;
	});
	age=0;
	value = 200;
}

void Alien::Load() {
	Object::Load();
}

void Alien::Initialize() {
	position = vec3(12,rand(-8.0f, 8.0f),0);
	orientation = quat(vec3(0,0,0));
	velocity = vec3(-2, rand(-1.0f, 1.0f), 0);

	Object::Initialize();
}

void Alien::Evade() {
	velocity.y = rand(-2.0f, 2.0f);
}

void Alien::Fire() {
	auto scene = this->scene;
	Bullet *bullet = new Bullet(scene, this);
	vec3 direction;
	int target = target_distribution(rand_engine);
	if (target == 0) { // asteroid
		auto asteroids = scene->Find(typeid(Asteroid));
		if (asteroids.size() > 0) {
			uniform_int_distribution<> uniform(0, asteroids.size()-1);
			auto asteroid = asteroids[uniform(rand_engine)];

			direction = normalize((length(position - asteroid->position) / bullet->speed) * asteroid->velocity + asteroid->position - position);
		}
	} else if (target == 1) { //ship
		auto destroyers = scene->Find(typeid(Destroyer));
		if (destroyers.size() > 0) {
			auto destroyer = destroyers[0];

			direction = normalize((length(position - destroyer->position) / bullet->speed) * destroyer->velocity + destroyer->position - position);
		}
	} else { //nothing
		direction = normalize(vec3(rand(0.0f, 1.0f), rand(0.0f, 1.0f), 0));
	}

	bullet->Initialize();
	bullet->Load();
	bullet->position = position + direction * (radius + bullet->radius);
	bullet->orientation = quat(vec3(0,0,acos(dot(direction, vec3(0,1,0))) * -direction.x/abs(direction.x)));
	bullet->velocity = velocity + bullet->speed*direction;
	bullet->destroyed += [=](Object* obj){ 
		if (scene->Get(this)) {
			Fire();
		}};
	scene->Add(bullet);
}

void Alien::Update(Time time, InputState const &input) {
	Object::Update(time, input);
	
	double t = time.ElapsedSeconds;
	float speed = length(velocity);
	age += t;
	orientation = quat(vec3(0, 0, time.TotalSeconds));

	//bullets
	if (age >= 2 && age - t < 2) {
		Fire();
	}
	if (floor(age / evasiveness) - floor((age-t)/evasiveness) >= 1) {
		Evade();
	}
}

function<void()> Alien::Collision(Object &other) {
	if (!(typeid(other) == typeid(Bullet) && ((Bullet*)&other)->source == this)) {
		auto s = scene;
		auto p = position;
		auto v = velocity;
		int points = (typeid(other) == typeid(Bullet)) ? value : 0;
		return [=](){ 
			Explosion* explosion = new Explosion(s);
			explosion->position = p;
			explosion->velocity = v;
			explosion->Initialize();
			((ScoreBoard*)s->Find(typeid(ScoreBoard))[0])->score += points;
			s->Add(explosion);
			s->Remove(this);
		};
	} else {
		return [](){};
	}
}