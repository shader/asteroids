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
	Object::Load();
}

void Destroyer::Initialize() {
	position = vec3(0,0,0);
	orientation = quat(vec3(0,0,0));
	bullet_count = 0;

	Object::Initialize();
}

void Destroyer::Update(Time time, InputState const &input) {
	double t = time.Elapsed().Seconds();	
	velocity -= velocity * .3 * t; //'friction' decay
	float speed = length(velocity);
	if (speed > 0)
		velocity = normalize(velocity) * glm::min(speed, 16.0f);
		
	if (input.special_keys[GLUT_KEY_LEFT]) {
		orientation = orientation * quat(vec3(0,0,10 * t));
		orientation = normalize(orientation);
	}
	if (input.special_keys[GLUT_KEY_RIGHT]) {
		orientation = orientation * quat(vec3(0,0,-10 * t));
		orientation = normalize(orientation);
	}
	if (input.special_keys[GLUT_KEY_UP] || input.keyboard['x']) {
		velocity = velocity + orientation * vec3(0,50 * t,0);
	}

	//bullets
	if (bullet_count < 20 && !(prev_state.keyboard[' '] || prev_state.keyboard['z']) && (input.keyboard[' '] || input.keyboard['z'])) {
  		bullet_count++;
		Bullet *bullet = new Bullet(scene);
		bullet->Initialize();
		bullet->Load();
		bullet->position = position + orientation*vec3(0,1,0) * (radius + bullet->radius);
		bullet->orientation = orientation;
		bullet->velocity = velocity + orientation*vec3(0,16,0);
		bullet->destroyed += [&](Object* obj){ bullet_count--; };
		scene->Add(bullet);
	}

	prev_state = input;
	Object::Update(time, input);
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