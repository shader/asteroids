#include "object.h"
#include "utils.h"
#include "ship.h"
#include "scene.h"
#include "content.h"

Destroyer::Destroyer(Scene *scene) : Object(scene) {
	Model* model = new Model(Content::mesh(ship));
	models.push_back(shared_ptr<Model>(model));
	size = vec3(0.75);
}

void Destroyer::Load() {
	for (auto m = models.begin(); m!=models.end(); m++) {
		(*m)->shader_type = basic;
	}
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