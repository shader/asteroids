#include "content.h"

Shader& Content::shader(ShaderType type) {
	auto shader = shaders.find(type);
	if (shader != shaders.end()) {
		return shaders[type];
	}
}

Mesh& Content::mesh(MeshType type) {
	auto mesh = meshes.find(type);
	if (mesh != meshes.end()) {
		return meshes[type];
	}
}

Model* Content::model(ModelType::Type type) {
	auto model = models.find(type);
	if (model != models.end()) {
		return models[type];
	}
}

void Content::Load(ShaderType type, string vertex, string fragment) {
	auto shader = shaders.find(type);
	if (shader == shaders.end()) {
		shaders[type] = Shader();
		shaders[type].Load(vertex, fragment);
		shaders[type].Create();
	} else {
		shader->second.Load(vertex, fragment);
		shader->second.Create();
	}
}

void Content::Load(MeshType type, function<Mesh()> factory) {
	auto mesh = meshes.find(type);
	if (mesh == meshes.end()) {
		meshes.insert(pair<MeshType, Mesh>(type, factory()));
	}
}

void Content::Load(ModelType::Type type, Model *model) {
	models[type] = model;
}

map<ShaderType,Shader> Content::shaders = map<ShaderType,Shader>();
map<MeshType,Mesh> Content::meshes = map<MeshType,Mesh>();
map<ModelType::Type,Model*> Content::models = map<ModelType::Type,Model*>();