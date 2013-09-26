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

void DefaultScene::Update(Time time) {	
	double t = time.Total().Seconds();

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

	Scene::Update(time);
}


void DefaultScene::Keyboard(unsigned char key, int x, int y) {
	switch(key) {
	case ' ':
		Bullet *bullet = new Bullet(this);
		bullet->position = destroyer->position;
		bullet->orientation = destroyer->orientation;
		bullet->velocity = destroyer->velocity + destroyer->orientation*vec3(0,8,0);
		bullet->Initialize();
		objects.push_back(bullet);
		break;
	}
}

void DefaultScene::Keyboard(int key, int x, int y) {
	switch(key) {
	case GLUT_KEY_LEFT:
		destroyer->orientation = destroyer->orientation * quat(vec3(0,0,0.1));
		destroyer->orientation = normalize(destroyer->orientation);
		break;
	case GLUT_KEY_RIGHT:
		destroyer->orientation = destroyer->orientation * quat(vec3(0,0,-0.1));
		destroyer->orientation = normalize(destroyer->orientation);
		break;
	case GLUT_KEY_UP:
		destroyer->velocity += destroyer->orientation * vec3(0,0.2,0);
		break;
	case GLUT_KEY_DOWN:
		destroyer->velocity -= destroyer->orientation * vec3(0,0.2,0);
		break;
	}
}