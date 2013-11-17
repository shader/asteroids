#include "content.h"

Shader& Content::shader(ShaderType type) {
	return shaders[type];
}

Mesh& Content::mesh(MeshType type) {
	return meshes[type];
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
		auto ret = meshes.insert(pair<MeshType, Mesh>(type, factory())); //pair of iterator and bool
	}
}

map<ShaderType,Shader> Content::shaders = map<ShaderType,Shader>();
map<MeshType,Mesh> Content::meshes = map<MeshType,Mesh>();