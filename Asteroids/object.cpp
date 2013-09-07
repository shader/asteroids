#include "object.h"
#include "utils.h"

Object::Object() {
	position = vec3(0,0,0);
	size = vec3(1,1,1);
	orientation;
}

void Object::Draw(mat4 view_projection) {	
	mat4 model = translation(position) * mat4_cast(orientation) * scale(size);
	mat4 MVP = view_projection * model;
	for(auto m = models.begin(); m!=models.end(); m++) {
		(*m).Draw(MVP);
	}
}