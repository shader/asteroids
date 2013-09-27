#include "object.h"
#include "ship.h"

Bullet::Bullet(Scene *scene) : Object(scene) {
	model = new Ship();
	model->size = vec3(0.25);
	models.push_back(model);
}

void Bullet::Initialize() {
	model->LoadShader(GL_VERTEX_SHADER, "shader.vert");
	model->LoadShader(GL_FRAGMENT_SHADER, "shader.frag");
	model->shader->Create();
	model->Bind();
}

void Bullet::Update(Time time) {
	age += time.ElapsedSeconds;
	if (age > 10) {
		//die
	}
	
	Object::Update(time);
}