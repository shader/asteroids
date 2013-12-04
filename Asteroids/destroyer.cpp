#include "object.h"
#include "utils.h"
#include "scene.h"
#include "content.h"

Destroyer::Destroyer(Scene *scene) : Object(scene) {
	Model* model = new Model(Content::mesh(ship));
	models.push_back(shared_ptr<Model>(model));
	size = vec3(0.75);
	bullet_count = 0;
}

void Destroyer::Load() {
	Object::Load();
}

void Destroyer::Initialize() {
	position = velocity = vec3(0);
	orientation = quat(vec3(0));

	Object::Initialize();
}

void Destroyer::Update(Time time, InputState const &input) {
	double t = time.Elapsed().Seconds();
	velocity -= velocity * .4 * t; //'friction' decay
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
		Bullet *bullet = new Bullet(scene, this);
		bullet->Initialize();
		bullet->Load();
		bullet->position = position + orientation*vec3(0,1,0) * (radius + bullet->radius);
		bullet->orientation = orientation;
		bullet->velocity = velocity + orientation*vec3(0,bullet->speed,0);
		bullet->destroyed += [&](Object* obj){ bullet_count--; };
		scene->Add(bullet);
	}

	prev_state = input;
	Object::Update(time, input);
}

function<void()> Destroyer::Collision(Object &other) {
	if (!(typeid(other) == typeid(Bullet) && ((Bullet*)&other)->source == this)) {
		auto s = scene;
		auto p = position;
		auto v = velocity;
		return [=](){ 
			auto destroyer = this;
			flags[ObjectFlags::Enabled] = false;
			Explosion* explosion = new Explosion(s, vec4(1,0,0,1));
			explosion->position = p;
			explosion->velocity = v;
			explosion->Initialize();
			explosion->destroyed += [=](Object *exp){
				destroyer->Killed();
				destroyer->flags[ObjectFlags::Enabled] = true;
				destroyer->Initialize();
			};
			s->Add(explosion);
		};
	} else {
		return [](){};
	}
}