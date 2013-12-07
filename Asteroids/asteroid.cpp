#include "object.h"
#include "utils.h"
#include "scene.h"
#include "content.h"
#include "particles.h"

Asteroid::Asteroid(Scene *scene) : Object(scene) {
	velocity = position = vec3(0);
	size = vec3(2);
	value = 20;
	generation = 0;
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
	explosion->Initialize();
	scene->Add(explosion);
	if (generation < 2) {
		auto a = new Asteroid(scene), b = new Asteroid(scene);
		a->generation = b->generation = generation+1;
		a->value = b->value = (generation == 0) ? 50 : 100;
		a->position = b->position = position;
		float angle = rand(0,TAU/8);
		a->velocity = velocity * quat(vec3(0,0,angle));
		b->velocity = velocity * quat(vec3(0,0,-angle));
		a->angular_vel = angular_vel + vec3(rand_vec2(),0);
		b->angular_vel = angular_vel + vec3(rand_vec2(),0);
		dynamic_cast<DefaultScene*>(scene)->add_asteroid(a);
		dynamic_cast<DefaultScene*>(scene)->add_asteroid(b);
		
		//duplicate model
		Mesh m1, m2;
		Plane plane(0);
		plane[generation]=1;
		models[0]->mesh.Slice(Plane(rand_vec3()), m1, m2);
		auto a_model = shared_ptr<Model>(new Model(m1));
		auto b_model = shared_ptr<Model>(new Model(m2));
		a_model->Bind();
		b_model->Bind();
		a->models.push_back(a_model); b->models.push_back(b_model);
		a->BoundingVolumes(); b->BoundingVolumes();
		a->Initialize(); b->Initialize();
	}
	scene->Remove(this);
}