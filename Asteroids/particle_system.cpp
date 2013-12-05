#include "particles.h"
#include "scene.h"
#include "content.h"

ParticleSystem::ParticleSystem(Scene *scene, vec4 start_color, vec4 end_color, int count) : Object(scene) {
	glGenVertexArrays(1, &vertex_array);
	glGenBuffers (1, &verticesID);
	glGenBuffers (1, &indicesID);
	glGenBuffers (1, &systemID);

	system.start_color = start_color;
	system.end_color = end_color;
	system.age = 0.0f;
	system.duration = 3;

	particles.resize(count, Particle());
	vertices.resize(count, vec3(0));
	indices.resize(count,0);
	flags[ObjectFlags::Collide] = false;
}

ParticleSystem::~ParticleSystem() {
	glDeleteBuffers(1, &verticesID);
	glDeleteBuffers(1, &indicesID);
	glDeleteBuffers(1, &systemID);
	glDeleteVertexArrays(1, &vertex_array);
}

void ParticleSystem::Initialize() {
	for (uint i=0; i<particles.size(); i++) {
		particles[i].age = 0;
	}
	
	Object::Initialize();
}

void ParticleSystem::Update(Time time, InputState const &input) {
	velocity -= velocity * .5 * time.ElapsedSeconds; //'friction' decay
	system.age += time.ElapsedSeconds;
	if (system.age > system.duration) {
		scene->Remove(this);
	} else {		
		for (auto p = particles.begin(); p!=particles.end(); p++) {
			p->age += time.ElapsedSeconds;
			p->position += p->velocity * time.ElapsedSeconds;
		}
		Object::Update(time, input);
	}
}

void ParticleSystem::Draw(mat4 view, mat4 projection) {
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
		glBindBuffer(GL_UNIFORM_BUFFER, systemID);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(System), &system, GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, shader->UniformBlockIndex("ParticleSystem"), systemID);
		
		glDrawElements(GL_POINTS, particles.size(), GL_UNSIGNED_INT, 0);
	shader->End();
	glBindVertexArray(0);
}