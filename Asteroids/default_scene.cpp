#include "scene.h"
#include "object.h"
#include "utils.h"
#include "content.h"

DefaultScene::DefaultScene(SceneManager *manager) : Scene(manager) {
	level = 1;
}

void DefaultScene::Load() {
	light_dir = vec3(1);
	light_color = vec3(1);	

	Content::Load(basic, "shader.vert", "shader.frag");
	Content::Load(explosion, "explosion.vert", "color.frag");

	Content::Load(bullet, []()->Mesh{ return Ship(); });
	Content::Load(icosahedron, []()->Mesh{ return Icosahedron(); });
	Content::Load(ship, []()->Mesh{ return Ship(); });
	
	lives = new LifeCounter(this, 3);
	lives->GameOver += [&](){ level = 1; manager->Restart(); };
	Add(lives);

	Scene::Load();
}

void DefaultScene::Initialize() {
	objects.remove_if([](unique_ptr<Object> const &p){
		return typeid(*p.get()) == typeid(Destroyer) ||
		typeid(*p.get()) == typeid(Asteroid) ||
		typeid(*p.get()) == typeid(Bullet) ||
		typeid(*p.get()) == typeid(Explosion);
	});
	
	Destroyer *destroyer = new Destroyer(this);
	destroyer->Load();
	destroyer->destroyed += [&](Object *obj){ lives->Die(); };
	Add(destroyer);

	asteroid_count = 0;

	for (int i=0; i<level; i++) {
		spawn_asteroid();
	}
		
	Scene::Initialize();
}

Asteroid* DefaultScene::spawn_asteroid() {
	auto asteroid = new Asteroid(this);
	asteroid->size = vec3(2);
	asteroid->position = vec3(rand_vec2() * 20.0f, 0);
	asteroid->velocity = normalize(asteroid->position + vec3(rand_vec2(), 0)) * -4.0f;
	asteroid->angular_vel = rand_vec3() * 2.0f;
	asteroid->Load();
	add_asteroid(asteroid);
	return asteroid;
}

void DefaultScene::add_asteroid(Asteroid* asteroid) {
	Add(asteroid);
	asteroid_count++;
	asteroid->destroyed += [&](Object* obj){ asteroid_count--; };
}

void DefaultScene::process_collisions() {
	//map<Object*,function<void()>> callbacks;
	queue<function<void()>> callbacks;
	int collisions = 0;
	for (auto a = objects.begin(); a!= objects.end(); a++) {
		for (auto b = objects.begin(); b!=a; b++) {
			if (length((*a)->position - (*b)->position) < (*a)->radius + (*b)->radius) {
				callbacks.push((*a)->Collision(**b));
				callbacks.push((*b)->Collision(**a));
				collisions++;
			}
		}
	}

	//handle state changes
	while (!callbacks.empty()) {
		callbacks.front()();
		callbacks.pop();
	}

	if (collisions > 0) {
		if (asteroid_count <=0) {
			level += 1;
			manager->Restart();
		}
	}
}

void DefaultScene::Update(Time time, const InputState &input) {	
	double t = time.Elapsed().Seconds();

	process_collisions();

	//wrap edges
	for (auto obj = objects.begin(); obj!=objects.end(); obj++) {
		if (dot((*obj)->velocity, (*obj)->position) > 0) { // && typeid(*(obj->get())) != typeid(Explosion)) {
			auto p = project((*obj)->position - normalize((*obj)->position)*(*obj)->size, View, Projection, vec4(0,0,1,1));
			if (p.x > 1 || p.x < 0 || p.y > 1 || p.y < 0) {
				auto v = normalize((*obj)->velocity);
				(*obj)->position -= 2 * dot(v, (*obj)->position) * v;
			}
		}
	}

	//handle keyboard input
	if (input.keyboard['a'] && !prev_state.keyboard['a']) {
		auto asteroid = spawn_asteroid();
		asteroid->Initialize();
	}

	Scene::Update(time, input);
}