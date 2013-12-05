#include "particles.h"
#include "utils.h"
#include "scene.h"
#include "content.h"
using namespace std;

Explosion::Explosion(Scene *scene, vec4 start_color, vec4 end_color, int count) : ParticleSystem(scene, start_color, end_color, count) {
	system.start_color = start_color;
	system.end_color = end_color;
	system.age = 0.0f;
	system.duration = 3;
	
	flags[ObjectFlags::Collide] = false;
}

void Explosion::Initialize() {
	for (uint i=0; i<particles.size(); i++) {
		particles[i].age = 0;
		particles[i].position = rand_ball() * size;
		particles[i].velocity = rand_ball() * 2.0f;
		particles[i].velocity += normalize(particles[i].velocity) * 1.0;
	}
	
	Object::Initialize();
}

void Explosion::Update(Time time, InputState const &input) {
	velocity -= velocity * .5 * time.ElapsedSeconds; //'friction' decay
	system.age += (float)time.ElapsedSeconds;
	if (system.age > system.duration) {
		scene->Remove(this);
	} else {		
		for (auto p = particles.begin(); p!=particles.end(); p++) {
			p->age += (float)time.ElapsedSeconds;
			p->position += p->velocity * time.ElapsedSeconds;
		}
		Object::Update(time, input);
	}
}
