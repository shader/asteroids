#include "object.h"
#include "utils.h"
#include "scene.h"

Object::Object(Scene *scene) {
	position = vec3(0,0,0);
	size = vec3(1,1,1);
	angular_vel = vec3(0);
	this->scene = scene;
}

Object::~Object() {
	for (auto m = models.begin(); m!=models.end(); m++) {
		delete (*m);
	}
}

void Object::Initialize() {}

void Object::Draw(mat4 view, mat4 projection) {	
	mat4 model = translation(position) * mat4_cast(orientation) * scale(size);
	mat4 mv = view * model;
	for(auto m = models.begin(); m!=models.end(); m++) {
		(*m)->shader->Begin();
		glUniformMatrix4fv((*(*m)->shader)("inv_view"), 1, GL_FALSE, &(mv[0][0]));
		(*m)->Draw(mv, projection);
		(*m)->shader->End();
	}
}

void Object::Update(Time time) {
	double t = time.Elapsed().Seconds();
	position += velocity * t;
	orientation = orientation * quat(angular_vel * t);
}