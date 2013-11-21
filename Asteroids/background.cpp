#include "object.h"
#include "utils.h"
#include "content.h"

Background::Background(Scene *scene) : Object(scene) {
	texture.load2D("resources/background.jpg", false);
	quad.mesh = Quad();
	quad.shader_type = textured;
	quad.Bind();
	position = vec3(0,0,-99);
}

void Background::Draw(mat4 view, mat4 projection) {
	Shader *shader = &Content::shader(textured);
	projection = ortho(-1.0f,1.0f,-1.0f,1.0f,0.0f,100.0f);	
	mat4 model = translation(position);
	shader->Begin();
		glUniform1i((*shader)("tex"), 0);
		quad.Draw(model, projection);
	shader->End();
}