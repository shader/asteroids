#include "object.h"
#include "rock.h"
#include "utils.h"

Asteroid::Asteroid(Scene *scene) : Object(scene) {
	rock = new Rock(7, 0.75, 3);
	models.push_back(rock);
	velocity = rand_vec3();
	position = rand_vec3();
}

void Asteroid::Initialize() {
	rock->LoadShader(GL_VERTEX_SHADER, "shader.vert");
	rock->LoadShader(GL_FRAGMENT_SHADER, "shader.frag");
	rock->shader->Create();
	rock->Bind();
}

void Asteroid::Update(Time time) {
	Object::Update(time);
}