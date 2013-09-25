#include "object.h"
#include "ship.h"

Bullet::Bullet() : Object() {
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
	Object::Update(time);
}