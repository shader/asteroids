#include "scene.h"
#include "object.h"

void DefaultScene::Initialize() {
	destroyer = new Destroyer();
	destroyer->position = vec3(0,0,0);
	objects.push_back(destroyer);

	//asteroids.push_back(new Asteroid());
	//objects.reserve(objects.size() + asteroids.size());
	//copy(asteroids.begin(), asteroids.end(), objects.end());

	Scene::Initialize();
}

void DefaultScene::Update(Time time) {	
	double t = time.Total().Seconds();

	for (auto obj = objects.begin(); obj!=objects.end(); obj++) {
		//project(destroyer->position, View, Projection, vec4(0,0,1,1));
		auto l = length((*obj)->position);
		if (length((*obj)->position) > 12) {
			(*obj)->position = -(*obj)->position;
		}
	}

	Scene::Update(time);
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
		destroyer->velocity += vec3(0,0.05,0) * destroyer->orientation;
		break;
	case GLUT_KEY_DOWN:
		destroyer->velocity -= (vec3(0,0.05,0) * destroyer->orientation);
		break;
	}
}