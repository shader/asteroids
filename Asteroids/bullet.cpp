#include "object.h"
#include "scene.h"
#include "content.h"
#include "particles.h"

Bullet::Bullet(Scene *scene, Object* source) : Object(scene) {
	this->source = source;
	size = vec3(0.1, 0.2, 0.1);
	age = 0;
	speed = 20;
	models.push_back(shared_ptr<Model>(new Model(Content::mesh(bullet))));
}

void Bullet::Initialize() {
	radius = glm::max(radius, models[0]->radius * glm::max(size.x, glm::max(size.y, size.z)));
}

void Bullet::Update(Time time, InputState const &input) {
	age += time.ElapsedSeconds;
	if (age > 2) {
		scene->Remove(this);
	} else {
		Object::Update(time, input);
	}
}

function<void()> Bullet::Collision(Object &other) {
	if (typeid(other) != typeid(Destroyer)) {
		Explosion* explosion = new Explosion(scene, vec4(0.75,0.75,0.75,1));
		explosion->position = position;
		explosion->velocity = velocity/2.0f;
		explosion->system.duration = 1;
		explosion->size = size;
		explosion->Initialize();
		scene->Add(explosion);
	}

	auto s = scene;
	return [=](){ s->Remove(this); };
}