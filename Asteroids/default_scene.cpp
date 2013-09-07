#include "scene.h"

void DefaultScene::Initialize() {

}

void DefaultScene::Update(Time time) {	
	double t = time.Total().Seconds();
	quat rot(vec3(0, t, t));
	for (auto o = objects.begin(); o!=objects.end(); o++) {
		(*o).orientation = rot;
	}
}