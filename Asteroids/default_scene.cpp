#include "scene.h"
#include "object.h"
#include "utils.h"
#include "content.h"

DefaultScene::DefaultScene(SceneManager *manager) : Scene(manager) {}

void DefaultScene::Load() {
	light_dir = vec3(1);
	light_color = vec3(1);

	Content::Load(basic, "shader.vert", "shader.frag");
	Content::Load(explosion, "explosion.vert", "color.frag");

	Content::Load(bullet, []()->Mesh{ return Ship(); });
	Content::Load(icosahedron, []()->Mesh{ return Icosahedron(); });
	Content::Load(ship, []()->Mesh{ return Ship(); });

	Scene::Load();
}

void DefaultScene::Initialize() {
	objects.erase(objects.begin(), objects.end());
	destroyer = new Destroyer(this);
	destroyer->Load();
	Add(destroyer);

	bullet_count = asteroid_count = 0;

	spawn_asteroid();
	
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
			manager->Restart();
		}
	}
}

void DefaultScene::Update(Time time, const InputState &input) {	
	double t = time.Elapsed().Seconds();

	process_collisions();

	//wrap edges
	for (auto obj = objects.begin(); obj!=objects.end(); obj++) {
		if (dot((*obj)->velocity, (*obj)->position) > 0) {
			auto p = project((*obj)->position - normalize((*obj)->position)*(*obj)->size, View, Projection, vec4(0,0,1,1));
			if (p.x > 1 || p.x < 0) {
				(*obj)->position.x = -(*obj)->position.x;
			}
			if (p.y > 1 || p.y < 0) {
				(*obj)->position.y = -(*obj)->position.y;
			}
		}
	}

	//handle keyboard input
	if (bullet_count < 20 && !(prev_state.keyboard[' '] || prev_state.keyboard['z']) && (input.keyboard[' '] || input.keyboard['z'])) {
  		bullet_count++;
		Bullet *bullet = new Bullet(this);
		bullet->models.push_back(bullet_model);
		bullet->Initialize();
		bullet->position = destroyer->position + destroyer->orientation*vec3(0,1,0) * (destroyer->radius + bullet->radius);
		bullet->orientation = destroyer->orientation;
		bullet->velocity = destroyer->velocity + destroyer->orientation*vec3(0,16,0);
		bullet->destroyed += [&](Object* obj){ bullet_count--; };
		Add(bullet);
	}

	if (input.special_keys[GLUT_KEY_LEFT]) {
		destroyer->orientation = destroyer->orientation * quat(vec3(0,0,10 * t));
		destroyer->orientation = normalize(destroyer->orientation);
	}
	if (input.special_keys[GLUT_KEY_RIGHT]) {
		destroyer->orientation = destroyer->orientation * quat(vec3(0,0,-10 * t));
		destroyer->orientation = normalize(destroyer->orientation);
	}
	if (input.special_keys[GLUT_KEY_UP] || input.keyboard['x']) {
		destroyer->velocity = destroyer->velocity + destroyer->orientation * vec3(0,50 * t,0);
	}

	if (input.keyboard['a'] && !prev_state.keyboard['a']) {
		auto asteroid = spawn_asteroid();
		asteroid->Initialize();
	}

	Scene::Update(time, input);
}