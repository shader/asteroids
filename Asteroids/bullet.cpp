#include "object.h"
#include "scene.h"
#include "content.h"

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
	auto s = scene;
	return [=](){ s->Remove(this); };
}