#include "object.h"
#include "utils.h"
#include "scene.h"
#include "content.h"
#include "particles.h"

Asteroid::Asteroid(Scene *scene) : Object(scene) {
	velocity = position = vec3(0);
	size = vec3(2);
	value = 20;
}

void Asteroid::Load() {
	Model *model = new Model(Content::mesh(rock));
	model->material.shininess = 4;
	model->material.specular = vec4(0.3);
	models.push_back(shared_ptr<Model>(model));
	Object::Load();
}

void Asteroid::Initialize() {
	Object::Initialize();
}

void Asteroid::Update(Time time, InputState const &input) {
	Object::Update(time, input);
}

function<void()> Asteroid::Collision(Object &other) {
	if (typeid(other) == typeid(Bullet)) {
		value = (typeid(((Bullet*)&other)->source) == typeid(Destroyer*)) ? value : 0;
		//add points if killed by player, otherwise no points
		return [=](){
			if (scene->Get(this)) {
				scene->Get<ScoreBoard>()->Score(value);
				this->Split();
			}
		};
	} else if (typeid(other) == typeid(Destroyer)) {
		//also points if collided with destroyer
		return [=](){
			if (scene->Get(this)) {
				scene->Get<ScoreBoard>()->Score(value);
				this->Split();
			}
		};		
	} else {
		//otherwise don't split
		return [](){};
	}
}

void Asteroid::Split() {
	Explosion* explosion = new Explosion(scene, vec4(0.5,0.5,0.5,1));
	explosion->position = position;
	explosion->velocity = velocity;
	explosion->size = size/2.0f;
	explosion->Initialize();
	scene->Add(explosion);
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
		
		//duplicate model
		a->models.push_back(models[0]); b->models.push_back(models[0]);
		a->BoundingVolumes(); b->BoundingVolumes();
		a->Initialize(); b->Initialize();
	}
	scene->Remove(this);
}