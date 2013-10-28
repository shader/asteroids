#include "object.h"
#include "ship.h"
#include "scene.h"

Bullet::Bullet(Scene *scene) : Object(scene) {
	size = vec3(0.25);
	age = 0;
}

void Bullet::Initialize() {
	radius = glm::max(radius, models[0]->radius * glm::max(size.x, glm::max(size.y, size.z)));
}

void Bullet::Update(Time time) {
	age += time.ElapsedSeconds;
	if (age > 2) {
		scene->Remove(this);
	} else {
		Object::Update(time);
	}
}

function<void()> Bullet::Collision(Object &other) {
	return [&](){ scene->Remove(this); };
}