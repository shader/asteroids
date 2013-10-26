#include "scene.h"

Scene& SceneManager::current() {
	return *(scenes.top().get());
}

void SceneManager::Update(Time time, const InputState &input) {
	current().Update(time, input);

	while( !event_queue.empty() ) {
		event_queue.front()();
		event_queue.pop();
	}
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
	scene->Initialize();
}