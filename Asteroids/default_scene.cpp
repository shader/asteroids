#include "scene.h"
#include "object.h"
#include "utils.h"

void DefaultScene::Initialize() {
	light_dir = vec3(1);
	light_color = vec3(1);

	destroyer = new Destroyer(this);
	destroyer->position = vec3(0,0,0);
	objects.push_back(destroyer);

	auto asteroid = new Asteroid(this);
	asteroid->size = vec3(2);
	asteroid->position = vec3(rand_vec2() * 20.0f, 0);
	asteroid->velocity = normalize(asteroid->position + vec3(rand_vec2(), 0)) * -4.0f;
	asteroid->angular_vel = rand_vec3() * 2.0f;
	asteroids.push_back(asteroid);
	objects.push_back(asteroid);

	Scene::Initialize();
}

void DefaultScene::Update(Time time, const InputState &input) {	
	double t = time.Elapsed().Seconds();

	//wrap edges
	for (auto obj = objects.begin(); obj!=objects.end(); obj++) {
		if (dot((*obj)->velocity, (*obj)->position) > 0) {
			auto p = project((*obj)->position - normalize((*obj)->position)*(*obj)->size, View, Projection, vec4(0,0,1,1));
			if (p.x > 1 || p.x < 0) {
				(*obj)->position.x = -(*obj)->position.x;
			}
			if (p.y > 1 || p.y < 0) {
				(*obj)->position.y = -(*obj)->position.y;
			}
		}
	}

	//handle keyboard input
	if (input.keyboard[' '] || input.keyboard['z']) {
		Bullet *bullet = new Bullet(this);
		bullet->position = destroyer->position;
		bullet->orientation = destroyer->orientation;
		bullet->velocity = destroyer->velocity + destroyer->orientation*vec3(0,16,0);
		bullet->Initialize();
		objects.push_back(bullet);
	}

	if (input.special_keys[GLUT_KEY_LEFT]) {
		destroyer->orientation = destroyer->orientation * quat(vec3(0,0,10 * t));
		destroyer->orientation = normalize(destroyer->orientation);
	}
	if (input.special_keys[GLUT_KEY_RIGHT]) {
		destroyer->orientation = destroyer->orientation * quat(vec3(0,0,-10 * t));
		destroyer->orientation = normalize(destroyer->orientation);
	}
	if (input.special_keys[GLUT_KEY_UP] || input.keyboard['x']) {
		destroyer->velocity = destroyer->velocity + destroyer->orientation * vec3(0,50 * t,0);
	}

	Scene::Update(time, input);
}