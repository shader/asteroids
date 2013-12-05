#pragma once
#include <glm/glm.hpp>
#include "object.h"

using namespace glm;

struct System {
	vec4 start_color;
	vec4 end_color;
	float age;
	float duration;
};

struct Particle {
	vec3 position, velocity;
	float age;
};

class ParticleSystem : public Object {
	public:
	System system;

	ParticleSystem(Scene *scene, vec4 start_color, vec4 end_color, int count);
	~ParticleSystem();
	vector<Particle> particles;
	void Initialize();
	void Update(Time time, InputState const &input);
	void Draw(mat4 view, mat4 projection);

private:
	GLuint vertex_array, verticesID, indicesID, systemID;
	vector<vec3> vertices;
	vector<GLuint> indices;
};

class Explosion : public ParticleSystem {
public:
	Explosion(Scene *scene, vec4 start_color, vec4 end_color = vec4(0), int count=500);
	void Initialize();
	void Update(Time time, InputState const &input);
};

class Flame : public ParticleSystem {
public:
	Flame(Scene *scene, vec4 start_color, vec4 end_color = vec4(0), int count=500);
	void Initialize();
	void Update(Time time, InputState const &input);
};