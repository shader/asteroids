#pragma once
#include <functional>
#include "utils.h"
#include "shader.h"
#include "geometry.h"
#include "model.h"

class Content {
public:
	static Shader& shader(ShaderType type);
	static Mesh& mesh(MeshType type);
	static Model* model(ModelType::Type type);

	static void Load(ShaderType type, string vertex, string fragment);
	static void Load(MeshType type, function<Mesh()>);
	static void Load(ModelType::Type type, Model* model);

private:
	static map<MeshType,Mesh> meshes;
	static map<ModelType::Type,Model*> models;
	static map<ShaderType,Shader> shaders;
};