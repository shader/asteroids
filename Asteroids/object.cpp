#include "object.h"
#include "utils.h"
#include "scene.h"
#include "content.h"

Object::Object(Scene *scene) {
	position = vec3(0,0,0);
	size = vec3(1,1,1);
	angular_vel = vec3(0);
	this->scene = scene;
	flags.set();
}

Object::~Object() {
	destroyed(this);
}

void Object::Load() {
	for (auto m = models.begin(); m!=models.end(); m++) {
		(*m)->Bind();
	}
	BoundingVolumes();
}

void Object::BoundingVolumes() {
	BoundingSphere();
	BoundingBox();
}

float Object::BoundingSphere() {
	for (auto m = models.begin(); m!=models.end(); m++) {
		if (length((*m)->position) > 0) {
			radius = glm::max(radius, length(((*m)->position + normalize((*m)->position) * (*m)->radius) * glm::max(size.x, glm::max(size.y, size.z))));
		} else {
			radius = glm::max(radius, (*m)->radius * glm::max(size.x, glm::max(size.y, size.z)));
		}
	}
	return radius;
}

Box Object::BoundingBox() {
	for(auto m = models.begin(); m!=models.end(); m++) {
		box.lower = min(box.lower, (**m).position + (*m)->mesh.box.lower);
		box.upper = max(box.upper, (**m).position + (*m)->mesh.box.upper);
	}
	box.Bind();

	Box b = {box.lower, box.upper};
	return b;
}

void Object::Initialize() {}

void Object::Draw(mat4 view, mat4 projection) {	
	mat4 model = translation(position) * mat4_cast(orientation) * scale(size);
	mat4 mv = view * model;
	for(auto m = models.begin(); m!=models.end(); m++) {
		Content::shader((*m)->shader_type).Begin();
		glUniformMatrix4fv(Content::shader((*m)->shader_type)("inv_view"), 1, GL_FALSE, &(mv[0][0]));
		(*m)->Draw(mv, projection);
		Content::shader((*m)->shader_type).End();
	}
}

void Object::DrawBox(mat4 view, mat4 projection) {	
	mat4 model = translation(position) * mat4_cast(orientation) * scale(size);
	mat4 mv = view * model;

	box.Draw(mv, projection);
}

void Object::DrawSphere(mat4 view, mat4 projection) {	
	mat4 model = translation(position) * mat4_cast(orientation) * scale(size);

	mat4 mv = view * model;

	Content::model(ModelType::sphere)->Draw(mv, projection);
}

void Object::Update(Time time, InputState const &input) {
	double t = time.Elapsed().Seconds();
	position += velocity * t;
	orientation = orientation * quat(angular_vel * t);
}

function<void()> Object::Collision(Object &other) { return [](){}; };