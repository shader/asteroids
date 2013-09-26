#include "object.h"
#include "ship.h"

Destroyer::Destroyer(Scene *scene) : Object(scene) {
	ship = new Ship();
	models.push_back(ship);
}

void Destroyer::Initialize() {
	ship->LoadShader(GL_VERTEX_SHADER, "shader.vert");
	ship->LoadShader(GL_FRAGMENT_SHADER, "shader.frag");
	ship->shader->Create();
	ship->Bind();
}

void Destroyer::Update(Time time) {
	Object::Update(time);
}