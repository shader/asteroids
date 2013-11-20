#include "object.h"
#include "utils.h"
#include "model.h"
#include "scene.h"
#include "content.h"

Alien::Alien(Scene *scene) : Object(scene) {
	Model* model = new Model(Content::mesh(alien));
	models.push_back(shared_ptr<Model>(model));
}

int Alien::bullet_count = 0;

void Alien::Load() {
	Object::Load();
}

void Alien::Initialize() {
	position = vec3(5,0,0);
	orientation = quat(vec3(0,0,0));

	Object::Initialize();
}

void Alien::Update(Time time, InputState const &input) {
	double t = time.Elapsed().Seconds();
	velocity -= velocity * .4 * t; //'friction' decay
	float speed = length(velocity);

	//bullets
	//if (bullet_count < 20 && !(prev_state.keyboard[' '] || prev_state.keyboard['z']) && (input.keyboard[' '] || input.keyboard['z'])) {
 // 		bullet_count++;
	//	Bullet *bullet = new Bullet(scene, this);
	//	bullet->Initialize();
	//	bullet->Load();
	//	bullet->position = position + orientation*vec3(0,1,0) * (radius + bullet->radius);
	//	bullet->orientation = orientation;
	//	bullet->velocity = velocity + orientation*vec3(0,20,0);
	//	bullet->destroyed += [&](Object* obj){ bullet_count--; };
	//	scene->Add(bullet);
	//}
	Object::Update(time, input);
}

function<void()> Alien::Collision(Object &other) {
	if (!(typeid(other) == typeid(Bullet) && ((Bullet*)&other)->source == this)) {
		auto s = scene;
		auto p = position;
		auto v = velocity;
		return [=](){ 
			Explosion* explosion = new Explosion(s);
			explosion->position = p;
			explosion->velocity = v;
			explosion->Initialize();
			s->Add(explosion);
			s->Remove(this);
		};
	} else {
		return [](){};
	}
}