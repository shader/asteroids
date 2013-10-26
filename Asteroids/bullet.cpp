#include "object.h"
#include "ship.h"
#include "scene.h"

Bullet::Bullet(Scene *scene) : Object(scene) {
	model = new Ship();
	size = vec3(0.25);
	models.push_back(model);
	age = 0;
}

void Bullet::Initialize() {
	model->LoadShader(GL_VERTEX_SHADER, "shader.vert");
	model->LoadShader(GL_FRAGMENT_SHADER, "shader.frag");
	model->shader->Create();
	
	Object::Initialize();
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
	return [&](){ age=2; };
}