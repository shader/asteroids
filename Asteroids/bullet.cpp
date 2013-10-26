#include "object.h"
#include "ship.h"
#include "scene.h"

Bullet::Bullet(Scene *scene) : Object(scene) {
	size = vec3(0.25);
	age = 0;
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