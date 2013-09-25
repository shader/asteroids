#include "object.h"
#include "utils.h"

Object::Object() {
	position = vec3(0,0,0);
	size = vec3(1,1,1);
	angular_vel = vec3(0);
}

Object::~Object() {
	for (auto m = models.begin(); m!=models.end(); m++) {
		delete (*m);
	}
}

void Object::Initialize() {}

void Object::Draw(mat4 view_projection) {	
	mat4 model = translation(position) * mat4_cast(orientation) * scale(size);
	mat4 MVP = view_projection * model;
	for(auto m = models.begin(); m!=models.end(); m++) {
		(*m)->Draw(MVP);
	}
}

void Object::Update(Time time) {
	double t = time.Elapsed().Seconds();
	//velocity -= velocity * .2 * t; //'friction' decay
	position += velocity * t;
	orientation = orientation * quat(angular_vel * t);
}