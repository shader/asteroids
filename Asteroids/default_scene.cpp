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

	Scene::Load();
}

void DefaultScene::Initialize() {
	objects.remove_if([](unique_ptr<Object> const &p){
		return (typeid(*p.get()) != typeid(LifeCounter) &&
			typeid(*p.get()) != typeid(Background) &&
			typeid(*p.get()) != typeid(ScoreBoard));
	});

	Destroyer *destroyer = new Destroyer(this);
	destroyer->Killed += [=](){ lives->Die(); };
	destroyer->Load();
	Add(destroyer);
	
	Alien *alien= new Alien(this);
	alien->Load();
	Add(alien);
	
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
		if ((*obj)->flags[ObjectFlags::Enabled] && (*obj)->flags[ObjectFlags::Wrap])
		if (dot((*obj)->velocity, (*obj)->position) > 0) {
			auto p = project((*obj)->position - normalize((*obj)->position)*(*obj)->size, View, Projection, vec4(0,0,1,1));
			if (p.x > 1 || p.x < 0) {
				if (typeid(**obj)!=typeid(Alien))
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