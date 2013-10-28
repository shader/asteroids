#include "object.h"
#include "utils.h"
#include "ship.h"
#include "scene.h"

Destroyer::Destroyer(Scene *scene) : Object(scene) {
	ship = new Ship();
	models.push_back(shared_ptr<Model>(ship));
	size = vec3(0.75);
}

void Destroyer::Load() {
	ship->LoadShaders("shader.vert", "shader.frag");

	Object::Load();
}

void Destroyer::Initialize() {
	position = vec3(0,0,0);
	orientation = quat(vec3(0,0,0));

	Object::Initialize();
}

void Destroyer::Update(Time time) {
	double t = time.Elapsed().Seconds();	
	velocity -= velocity * .3 * t; //'friction' decay
	float speed = length(velocity);
	if (speed > 0)
		velocity = normalize(velocity) * glm::min(speed, 16.0f);
	Object::Update(time);
}

function<void()> Destroyer::Collision(Object &other) {
	return [=](){ 
		Explosion* explosion = new Explosion(scene);
		explosion->position = position;
		explosion->velocity = velocity;
		explosion->Initialize();
		scene->Add(explosion);
		//explosion->destroyed += [](Object* obj){ obj->scene->Initialize(); };
		scene->Remove(this);
	};
}