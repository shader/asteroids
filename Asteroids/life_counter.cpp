#include "object.h"
#include "utils.h"
#include "content.h"
#include "scene.h"

LifeCounter::LifeCounter(Scene* scene, int max_lives) : Object(scene) {
	size = vec3(0.5);
	position = vec3(0.75, 19, -10);
	this->max_lives = this->lives = max_lives;

	icon.mesh = Ship();
	icon.Bind();
	rotation = quat(vec3(0));
}

void LifeCounter::Initialize() {
	lives = max_lives;
}

void LifeCounter::Die() {
	lives--;
	if (lives <= 0) {
		//GameOver();
	}
}

void LifeCounter::Update(Time time, InputState const &input) {
	double t = time.Elapsed().Seconds();
	orientation = orientation * quat(vec3(0,t,0));
}

void LifeCounter::Draw(mat4 view, mat4 projection) {
	projection = ortho(0.0f, 20.0f * (scene->width / (float)scene->height), 0.0f, 20.0f, -20.0f, 20.0f);
	Content::shader(basic).Begin();
	for(int i=0; i<lives; i++) {
		mat4 model = translation(position + vec3(i * icon.size.x, 0, 0))  * mat4_cast(orientation) * scale(size);
		model = model;
		glUniformMatrix4fv(Content::shader(basic)("inv_view"), 1, GL_FALSE, &(model[0][0]));
		icon.Draw(model, projection);
	}
	Content::shader(basic).End();
}