#include "scene.h"

Scene& SceneManager::current() {
	return *(scenes.top().get());
}

void SceneManager::Update(Time time, const InputState &input) {
	int w = glutGet (GLUT_WINDOW_WIDTH);
	int h = glutGet (GLUT_WINDOW_HEIGHT);
	if (w != width || h != height) {
		Resize(w, h);
	}
	
	if (input.keyboard['p'] && !prev_state.keyboard['p']) {
		pause ^= 1;
	}

	if (!pause)
		current().Update(time, input);

	while( !event_queue.empty() ) {
		event_queue.front()();
		event_queue.pop();
	}

	prev_state = input;
}

void SceneManager::Draw() {
	current().Draw();
}

void SceneManager::Resize(int w, int h) {
	current().Resize(w, h);
}

void SceneManager::Restart() {
	event_queue.push([this](){ this->current().Initialize(); });
}

void SceneManager::Push(Scene *scene) {
	scenes.push(unique_ptr<Scene>(scene));
	scene->Load();
	scene->Initialize();
}