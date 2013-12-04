#include "scene.h"
#include "object.h"
#include "utils.h"
#include "content.h"

DefaultScene::DefaultScene(SceneManager *manager) : Scene(manager) {
	level = 1;
	draw_spheres = draw_boxes = false;
}

void DefaultScene::Load() {
	light_dir = vec3(1);
	light_color = vec3(1);	

	Content::Load(basic, "shader.vert", "shader.frag");
	Content::Load(explosion, "particle.vert", "color.frag");
	Content::Load(polygon, "polygon.vert", "color.frag");
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

	auto sphere = new Model(Sphere());
	sphere->Bind();
	sphere->material.shininess = 32.0f;
	sphere->material.specular = vec4(1,1,1,1);
	sphere->material.diffuse = vec4(0,0,0.5,0.3);
	sphere->material.color = vec4(0,0,1,0.5);
	sphere->material.emission = vec4(0,0,0.1,0.1);
	Content::Load(ModelType::sphere, sphere);
	
	lives = new LifeCounter(this, 3);
	lives->GameOver += [&](){ level = 1; manager->Restart(); };
	Add(lives);
		
	Background *background = new Background(this);
	Add(background);

	score_board = new ScoreBoard(this);
	score_board->flags[ObjectFlags::Draw] = false;
	Add(score_board);

	score_board->Score += [=](int points) {
		if (level >= 3 && rand(0, 50) < 1 && !(this->Get<Alien>())) {
  			BigAlien* alien = new BigAlien(this);
			alien->Load(); alien->Initialize();
			Add(alien);
		} else if (this->Get<ScoreBoard>()->Points() > 10000 && rand(0, 100) < 1 && !(this->Get<Alien>())) {
			SmallAlien* alien = new SmallAlien(this);
			alien->Load(); alien->Initialize();
			Add(alien);
		}
	};

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
	level = 1;

	spawn_asteroid();
		
	Scene::Initialize();
}

Asteroid* DefaultScene::spawn_asteroid() {
	auto asteroid = new Asteroid(this);
	asteroid->position = vec3(rand_vec2() * 20.0f, 0);
	asteroid->velocity = normalize(asteroid->position + vec3(rand_vec2(), 0)) * -5.0f;
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

	Scene::Update(time, input);
}

void DefaultScene::Keyboard(const InputState &input, const InputState &prev) {
	if (input.keyboard['1'] && !prev.keyboard['1']) {
		auto asteroid = spawn_asteroid();
		asteroid->Initialize();
	}

	if (input.keyboard['2'] && !prev.keyboard['2']) {
		auto alien = new BigAlien(this);
		alien->Load(); alien->Initialize();
		Add(alien);
	}

	if (input.keyboard['3'] && !prev.keyboard['3']) {
		auto alien = new SmallAlien(this);
		alien->Load(); alien->Initialize();
		Add(alien);
	}

	if (input.keyboard['4'] && !prev.keyboard['4']) {
		draw_boxes ^= 1;
	}
	
	if (input.keyboard['5'] && !prev.keyboard['5']) {
		draw_spheres ^= 1;
	}

	if (input.keyboard['a'] && !prev.keyboard['a']) {
		manager->Push(new AsteroidScene(manager));
	}
}

void DefaultScene::Draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 vp = Projection * View;
	for (auto o = objects.begin(); o!= objects.end(); o++) {
		if ((*o)->flags[ObjectFlags::Enabled] && (*o)->flags[ObjectFlags::Draw]) {
			(*o)->Draw(View, Projection);
			if ((*o)->flags[ObjectFlags::Collide]) {
				if (draw_spheres)
					(*o)->DrawSphere(View, Projection);
				if (draw_boxes) {
					LineBox box = LineBox();
					Box b = (*o)->WorldBox();
					box.lower = b.lower;
					box.upper = b.upper;
					box.Bind();
					box.Draw(View*translation((*o)->position), Projection);
				}
			}
		}
	}

	score_board->Draw(mat4(1), mat4(1));
		
	glutSwapBuffers();
}