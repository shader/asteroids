#pragma once
#include <functional>
#include "utils.h"
#include "shader.h"
#include "geometry.h"

class Content {
public:
	static Shader& shader(ShaderType type);
	static Mesh& mesh(MeshType type);

	static void Load(ShaderType type, string vertex, string fragment);
	static void Load(MeshType type, function<Mesh()>);

private:
	static map<MeshType,Mesh> meshes;
	static map<ShaderType,Shader> shaders;
};