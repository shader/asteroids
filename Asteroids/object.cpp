#include "object.h"
#include "utils.h"

void Object::Draw(mat4 view_projection) {	
	mat4 model = translation(position) * mat4_cast(orientation) * scale(size);
	mat4 MVP = view_projection * model;
	for(auto m = models.begin(); m!=models.end(); m++) {
		(*m).Draw(MVP);
	}
}