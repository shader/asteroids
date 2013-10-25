#include "object.h"
#include "rock.h"
#include "utils.h"
#include "scene.h"

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

	Object::Initialize();
}

void Asteroid::Update(Time time) {
	Object::Update(time);
}

function<void()> Asteroid::Collision(Object &other) {
	if (typeid(other) == typeid(Bullet) || typeid(other) == typeid(Destroyer)) {
		return [=](){ this->Split(); };
	} else {
		return [](){};
	}
}

void Asteroid::Split() {
	if (size.x > .5) {
		auto a = new Asteroid(scene), b = new Asteroid(scene);
		a->position = b->position = position;
		a->size = b->size = size / 2.0f;
		a->velocity = velocity + vec3(rand_vec2(),0);
		b->velocity = velocity + vec3(rand_vec2(),0);
		a->angular_vel = angular_vel + rand_vec3();	
		b->angular_vel = angular_vel + rand_vec3();
		dynamic_cast<DefaultScene*>(scene)->add_asteroid(a);
		dynamic_cast<DefaultScene*>(scene)->add_asteroid(b);
		a->Initialize(); b->Initialize();
	}
	delete this;
}