#include "object.h"
#include "utils.h"
#include "scene.h"
#include "content.h"

Asteroid::Asteroid(Scene *scene) : Object(scene) {
	velocity = position = vec3(0);
	size = vec3(2);
	value = 20;
}

void Asteroid::Load() {
	models.push_back(shared_ptr<Model>(new Model(Content::mesh(rock))));
	Object::Load();
}

void Asteroid::Initialize() {
	Object::Initialize();
}

void Asteroid::Update(Time time, InputState const &input) {
	Object::Update(time, input);
}

function<void()> Asteroid::Collision(Object &other) {
	if ((typeid(other) == typeid(Bullet) && typeid(*((Bullet*)&other)->source) == typeid(Destroyer)) || typeid(other) == typeid(Destroyer)) {
		//add points if killed by player
		return [=](){
			if (scene->Get(this)) {
				scene->Get<ScoreBoard>()->Score(value);
				this->Split();
			}
		};
	} else if (typeid(other) == typeid(Bullet)) {
		//split if shot by alien, but no points
		return [=](){
			if (scene->Get(this)) {
				this->Split();
			}
		};
	} else {
		//otherwise don't split
		return [](){};
	}
}

void Asteroid::Split() {
	if (size.x > .5) {
		auto a = new Asteroid(scene), b = new Asteroid(scene);
		a->value = b->value = (size.x == 2) ? 50 : 100;
		a->position = b->position = position;
		a->size = b->size = size / 2.0f;
		float angle = rand(0,TAU/8);
		a->velocity = velocity * quat(vec3(0,0,angle));
		b->velocity = velocity * quat(vec3(0,0,-angle));
		a->angular_vel = angular_vel + vec3(rand_vec2(),0);
		b->angular_vel = angular_vel + vec3(rand_vec2(),0);
		dynamic_cast<DefaultScene*>(scene)->add_asteroid(a);
		dynamic_cast<DefaultScene*>(scene)->add_asteroid(b);
		
		a->models.push_back(models[0]); b->models.push_back(models[0]);
		a->BoundingVolumes(); b->BoundingVolumes();
		a->Initialize(); b->Initialize();
	}
	scene->Remove(this);
}