#include "scene.h"
#include "object.h"

void DefaultScene::Initialize() {
	objects.push_back(new Destroyer());
	Scene::Initialize();
}

void DefaultScene::Update(Time time) {	
	double t = time.Total().Seconds();
	quat rot(vec3(0, t, t));
	for (auto o = objects.begin(); o!=objects.end(); o++) {
		(*o)->orientation = rot;
	}
	Scene::Update(time);
}