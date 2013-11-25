#include "scene.h"
#include "object.h"
#include "utils.h"
#include "content.h"

AsteroidScene::AsteroidScene(SceneManager *manager) : Scene(manager) {
}

void AsteroidScene::Load() {
	Content::Load(basic, "shader.vert", "shader.frag");

	asteroid_model = shared_ptr<Model>(new Model(Icosahedron()));

	asteroid = unique_ptr<Object>(new Object(this));
	asteroid->models.push_back(asteroid_model);
	//asteroid->angular_vel = vec3(0,1,0);
	asteroid->size = vec3(5);
	objects.push_back(move(asteroid));
}

void AsteroidScene::Initialize() {
	asteroid_model->mesh = Icosahedron();
	radius = 0.75;
	rate = 3;
	lit = 1;
	mode = GL_TRIANGLES;
	asteroid_model->Bind(mode);
}

void AsteroidScene::Keyboard(const InputState &input, const InputState &prev) {
	if (input.keyboard['0'] && !prev.keyboard['0']) {
		mode = (mode == GL_LINES) ? GL_TRIANGLES : GL_LINES;
		asteroid_model->Bind(mode);
	}
	
	if (input.keyboard['1'] && !prev.keyboard['1']) {
		asteroid_model->mesh.Perturb(radius);
		asteroid_model->Bind(mode);
		radius /= rate;
	}
	if (input.keyboard['2'] && !prev.keyboard['2']) {
		asteroid_model->mesh.Split();
		asteroid_model->Bind(mode);
	}	
	if (input.keyboard['3'] && !prev.keyboard['3']) {
		asteroid_model->mesh.Average();
		asteroid_model->Bind(mode);
	}
	if (input.keyboard['4'] && !prev.keyboard['4']) {
		asteroid_model->mesh.Normalize();
		asteroid_model->Bind(mode);
	}
	if (input.keyboard['5'] && !prev.keyboard['5']) {
		Material ambient = { vec4(1), vec4(.6), vec4(.3), vec4(.3), 32.0f };
		Material shaded = { vec4(1), vec4(0), vec4(1), vec4(1), 32.0f };
		asteroid_model->material = (lit ^= 1) ? shaded : ambient;
		asteroid_model->Bind(mode);
	}
	if (input.keyboard['r']) {
		manager->Restart();
	}
	if (input.keyboard[27]) {
		exit(0);
	}

	Scene::Keyboard(input, prev);
}