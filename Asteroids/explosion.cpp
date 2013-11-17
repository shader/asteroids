#include "object.h"
#include "utils.h"
#include "scene.h"
#include "content.h"
using namespace std;

Explosion::Explosion(Scene *scene) : Object(scene) {
	glGenVertexArrays(1, &vertex_array);
	glGenBuffers (1, &verticesID);
	glGenBuffers (1, &indicesID);

	age=0.0f;
	particles.resize(100, Particle());
	vertices.resize(100, vec3(0));
	indices.resize(100,0);
}

Explosion::~Explosion() {
	glDeleteBuffers(1, &verticesID);
	glDeleteBuffers(1, &indicesID);
	glDeleteVertexArrays(1, &vertex_array);
}

void Explosion::Initialize() {
	for (uint i=0; i<particles.size(); i++) {
		particles[i].age = 0;
		particles[i].position = vec3(0);
		particles[i].velocity = rand_ball() * 2.0f;
	}
	
	Object::Initialize();
}

void Explosion::Update(Time time, InputState const &input) {
	age += time.ElapsedSeconds;
	if (age > 3) {
		scene->manager->Restart();
	} else {		
		for (auto p = particles.begin(); p!=particles.end(); p++) {
			p->age += time.ElapsedSeconds;
			p->position += p->velocity * time.ElapsedSeconds;
		}
		Object::Update(time, input);
	}
}

void Explosion::Draw(mat4 view, mat4 projection) {
	Shader* shader = &Content::shader(explosion);
	for (uint i=0; i<particles.size(); i++) {
		vertices[i] = particles[i].position;
		indices[i] = i;
	}

	mat4 model = translation(position) * mat4_cast(orientation) * scale(size);
	mat4 mvp = projection * view * model;
	glBindVertexArray(vertex_array);
	shader->Begin();
		glBindBuffer (GL_ARRAY_BUFFER, verticesID);
		glBufferData (GL_ARRAY_BUFFER, sizeof(vec3)*vertices.size(), &vertices[0], GL_STATIC_DRAW);		
		glEnableVertexAttribArray((*shader)["vertex"]);
		glVertexAttribPointer ((*shader)["vertex"], 3, GL_FLOAT, GL_FALSE, 0, 0);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), &indices[0], GL_STATIC_DRAW);

		glUniformMatrix4fv((*shader)("MVP"), 1, GL_FALSE, &mvp[0][0]);
		glUniform1f((*shader)("age"), age);

		glDrawElements(GL_POINTS, particles.size(), GL_UNSIGNED_INT, 0);
	shader->End();
	glBindVertexArray(0);
}
