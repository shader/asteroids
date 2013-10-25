#include "object.h"
#include "utils.h"
#include "ship.h"

Destroyer::Destroyer(Scene *scene) : Object(scene) {
	ship = new Ship();
	models.push_back(ship);
	size = vec3(0.75);
}

void Destroyer::Initialize() {
	ship->LoadShader(GL_VERTEX_SHADER, "shader.vert");
	ship->LoadShader(GL_FRAGMENT_SHADER, "shader.frag");
	ship->shader->Create();
	ship->Bind();
}

void Destroyer::Update(Time time) {
	double t = time.Elapsed().Seconds();	
	velocity -= velocity * .3 * t; //'friction' decay
	float speed = length(velocity);
	if (speed > 0)
		velocity = normalize(velocity) * glm::min(speed, 16.0f);
	Object::Update(time);
}