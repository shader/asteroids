#include "scene.h"
#include "object.h"
#include "utils.h"

void DefaultScene::Initialize() {
	destroyer = new Destroyer();
	destroyer->position = vec3(0,0,0);
	objects.push_back(destroyer);

	auto asteroid = new Asteroid();
	asteroid->position = rand_vec3() * 10.0f;
	asteroid->position.z = 0;
	asteroid->velocity = rand_vec3() * 4.0f;
	asteroid->velocity.z = 0;
	asteroid->angular_vel = rand_vec3() * 2.0f;
	asteroids.push_back(asteroid);
	objects.push_back(asteroid);

	Scene::Initialize();
}

void DefaultScene::Update(Time time) {	
	double t = time.Total().Seconds();

	//wrap edges
	for (auto obj = objects.begin(); obj!=objects.end(); obj++) {
		auto p = project((*obj)->position, View, Projection, vec4(0,0,1,1));
		if (p.x > 1.05 || p.x < -0.05 || p.y > 1.05 || p.y < -0.05) {
			(*obj)->position = -(*obj)->position;
		}
	}

	Scene::Update(time);
}


void DefaultScene::Keyboard(unsigned char key, int x, int y) {
	switch(key) {
	case ' ':
		Bullet *bullet = new Bullet();
		bullet->position = destroyer->position;
		bullet->orientation = destroyer->orientation;
		bullet->velocity = destroyer->velocity + normalize(destroyer->velocity + destroyer->orientation*vec3(0,1,0)) * 8.0f;
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