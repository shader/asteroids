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
	Content::Load(textured, "texture.vert", "texture.frag");

	Content::Load(bullet, []()->Mesh{ return Ship(); });
	Content::Load(icosahedron, []()->Mesh{ return Icosahedron(); });
	Content::Load(rock, []()->Mesh{ return Rock(7, 0.75, 3); });
	Content::Load(ship, []()->Mesh{ return Ship(); });
	Content::Load(alien, []()->Mesh {
		Mesh alien = Icosahedron();
		alien.Subdivide(3, true);
		alien.Normalize();
		alien.BoundingBox();
		return alien;
	});
	
	lives = new LifeCounter(this, 3);
	lives->GameOver += [&](){ level = 1; manager->Restart(); };
	Add(lives);
		
	Background *background = new Background(this);
	Add(background);

	ScoreBoard * score_board = new ScoreBoard(this);
	Add(score_board);

	Destroyer *destroyer = new Destroyer(this);
	destroyer->Killed += [=](){ lives->Die(); };
	Add(destroyer);
	
	Scene::Load();
}

void DefaultScene::Initialize() {
	objects.remove_if([](unique_ptr<Object> const &p){
		return (typeid(*p) == typeid(Asteroid) ||
			typeid(*p) == typeid(Alien)); 
	});

	asteroid_count = 0;

	spawn_asteroid();
		
	Scene::Initialize();
}

Asteroid* DefaultScene::spawn_asteroid() {
	auto asteroid = new Asteroid(this);
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
	queue<function<void()>> callbacks;
	int collisions = 0;
	for (auto a = objects.begin(); a!= objects.end(); a++) {
		if ((*a)->flags[ObjectFlags::Enabled] && (*a)->flags[ObjectFlags::Collide])
		for (auto b = objects.begin(); b!=a; b++) {
			if ((*b)->flags[ObjectFlags::Enabled] && (*b)->flags[ObjectFlags::Collide])
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

	//increase level when cleared
	if (collisions > 0) {
		if (asteroid_count <=0) {
			level = (level + 1) % 8; //max 8 asteroids
			Find(typeid(Destroyer))[0]->Initialize();
			for (int i=0; i<level; i++) {
				spawn_asteroid();
			}
		}
	}
}

void DefaultScene::Update(Time time, const InputState &input) {	
	double t = time.Elapsed().Seconds();

	process_collisions();

	//wrap edges
	for (auto obj = objects.begin(); obj!=objects.end(); obj++) {		
		if ((*obj)->flags[ObjectFlags::Enabled] && (*obj)->flags[ObjectFlags::Wrap])
		if (dot((*obj)->velocity, (*obj)->position) > 0) {
			auto p = project((*obj)->position - normalize((*obj)->position)*(*obj)->size, View, Projection, vec4(0,0,1,1));
			if (p.x > 1 || p.x < 0) {
				if (auto alien = dynamic_cast<Alien*>(obj->get()))
					event_queue.push([=](){ Remove(alien); });

				(*obj)->position.x = -(*obj)->position.x;
			}
			if (p.y > 1 || p.y < 0) {
				(*obj)->position.y = -(*obj)->position.y;
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