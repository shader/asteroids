#include "object.h"
#include "model.h"
#include "utils.h"
#include "scene.h"

Asteroid::Asteroid(Scene *scene) : Object(scene) {
	models.push_back(shared_ptr<Model>(new Rock(7, 0.75, 3)));
	velocity = rand_vec3();
	position = rand_vec3();
}

void Asteroid::Load() {
	Object::Load();
}

void Asteroid::Initialize() {
	Object::Initialize();
}

void Asteroid::Update(Time time, InputState const &input) {
	Object::Update(time, input);
}

function<void()> Asteroid::Collision(Object &other) {
	if (typeid(other) == typeid(Bullet) || typeid(other) == typeid(Destroyer)) {
		return [=](){ this->Split(); };
	} else {
		return [](){};
	}
}

void Asteroid::Split() {
	if (size.x > .5) {
		auto a = new Asteroid(scene), b = new Asteroid(scene);
		a->position = b->position = position;
		a->size = b->size = size / 2.0f;
		float angle = rand(0,TAU/8);
		a->velocity = velocity * quat(vec3(0,0,angle));
		b->velocity = velocity * quat(vec3(0,0,-angle));
		a->angular_vel = angular_vel + vec3(rand_vec2(),0);
		b->angular_vel = angular_vel + vec3(rand_vec2(),0);
		dynamic_cast<DefaultScene*>(scene)->add_asteroid(a);
		dynamic_cast<DefaultScene*>(scene)->add_asteroid(b);
		
		a->Load(); b->Load();
		a->Initialize(); b->Initialize();
	}
	scene->Remove(this);
}