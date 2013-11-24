#include "headers.h"
using namespace glm;

Model::Model(ShaderType shader) {
	shader_type = shader;
	position = vec3(0.0f, 0.0f, 0.0f);
	size = vec3(1.0f, 1.0f, 1.0f);
	draw_mode = GL_TRIANGLES;

	Material m = { vec4(1), vec4(0), vec4(1), vec4(1), 32.0f };
	material = m;
	
	verticesID = texcoordsID = normalsID = indicesID = vertex_array = materialID = -1;
}

Model::Model(Mesh &mesh, ShaderType shader) {
	this->mesh = mesh;
	shader_type = shader;
	position = vec3(0.0f, 0.0f, 0.0f);
	size = vec3(1.0f, 1.0f, 1.0f);
	draw_mode = GL_TRIANGLES;
	
	Material m = { vec4(1), vec4(0), vec4(1), vec4(1), 32.0f };
	material = m;

	verticesID = texcoordsID = normalsID = indicesID = vertex_array = materialID = -1;
}

Model::~Model() {
	DeleteBuffers();
}

void Model::GenBuffers(Shader &shader) {
	glGenVertexArrays(1, &vertex_array);
	glGenBuffers (1, &verticesID);
	glGenBuffers (1, &indicesID);
	if (shader["texcoord"]!=-1) glGenBuffers (1, &texcoordsID);
	if (shader["normal"]!=-1) glGenBuffers (1, &normalsID);
	if (shader.UniformBlockIndex("Material") !=-1 ) glGenBuffers (1, &materialID);
}

void Model::DeleteBuffers() {
	if (verticesID != -1) glDeleteBuffers(1, &verticesID);
	if (texcoordsID != -1) glDeleteBuffers (1, &texcoordsID);
	if (normalsID != -1) glDeleteBuffers(1, &normalsID);
	if (indicesID != -1) glDeleteBuffers(1, &indicesID);
	if (indicesID != -1) glDeleteBuffers(1, &materialID);
	if (vertex_array != -1) glDeleteVertexArrays(1, &vertex_array);
}

void Model::Bind() {
	Bind(Content::shader(shader_type), draw_mode);
}

void Model::Bind(GLenum mode) {
	draw_mode = mode;
	Bind(Content::shader(shader_type), draw_mode);
}

void Model::Bind(Shader& shader, GLenum mode) {
	//Bind model vertex data to VertexArray, so it can be used for drawing later. Call whenever vertex data is updated
	DeleteBuffers();
	GenBuffers(shader);
	indices.clear();
	radius = 0;
	mesh.BoundingBox();

	vector<vec3> vertices(mesh.vertices.size());
	vector<vec2> texcoords(mesh.vertices.size());
	vector<vec3> normals(mesh.vertices.size());
	for (uint i = 0; i<mesh.vertices.size(); i++) {
		vertices[i] = mesh.vertices[i].position;
		normals[i] = mesh.vertices[i].normal();
		if (shader["texcoord"] != -1) texcoords[i] = mesh.vertices[i].texture_coord;
		radius = glm::max(radius, length(vertices[i]));
	}

	draw_mode = mode;
	switch (mode) {
	case(GL_POINTS):
		for (GLuint i=0; i < mesh.vertices.size(); i++)
			indices.push_back(i);
		break;
	case(GL_LINES):
		for (auto f = mesh.faces.begin(); f != mesh.faces.end(); f++) {
			Edge *e = &f->first();
			do {
				indices.push_back(e->tail_vertex);
				indices.push_back(e->head_vertex);
				e = &e->next();
			} while (e != &f->first());
		}
		break;
	case(GL_TRIANGLES):
		for (auto f = mesh.faces.begin(); f != mesh.faces.end(); f++) {
			Edge* e = &f->first(); //get first edge in face
			indices.push_back(e->tail_vertex);
			indices.push_back(e->head_vertex);
			indices.push_back(e->next().head_vertex);
		}
		break;
	};

	glBindVertexArray(vertex_array);
		//position
		glBindBuffer (GL_ARRAY_BUFFER, verticesID);
		glBufferData (GL_ARRAY_BUFFER, sizeof(vec3)*mesh.vertices.size(), &vertices[0], GL_STATIC_DRAW);		
		glEnableVertexAttribArray(shader["vertex"]);
		glVertexAttribPointer (shader["vertex"], 3, GL_FLOAT, GL_FALSE, 0, 0);

		//texture coordinates
		if (shader["texcoord"] != -1) {
			glBindBuffer (GL_ARRAY_BUFFER, texcoordsID);
			glBufferData (GL_ARRAY_BUFFER, sizeof(vec2)*mesh.vertices.size(), &texcoords[0], GL_STATIC_DRAW);		
			glEnableVertexAttribArray(shader["texcoord"]);
			glVertexAttribPointer(shader["texcoord"], 2, GL_FLOAT, GL_FALSE, 0, 0);
		}
		
		//normal
		if (shader["normal"] != -1) {
			glBindBuffer (GL_ARRAY_BUFFER, normalsID);
			glBufferData (GL_ARRAY_BUFFER, sizeof(vec3)*mesh.vertices.size(), &normals[0], GL_STATIC_DRAW);		
			glEnableVertexAttribArray(shader["normal"]);
			glVertexAttribPointer(shader["normal"], 3, GL_FLOAT, GL_FALSE, 0, 0);
		}
		
		//index
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), &indices[0], GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void Model::Draw(Shader& shader, mat4 view, mat4 projection, GLenum mode) {
	mat4 rotation = mat4_cast(orientation);
	mat4 trans = translation(position);
	mat4 model_view = view * trans * rotation * scale(size);
	mat4 MVP = projection * model_view;
	mat3 normal_matrix = mat3(inverse(transpose(view * trans * rotation)));
	glBindVertexArray(vertex_array);
	shader.Begin();
		glUniformMatrix4fv(shader("MVP"), 1, GL_FALSE, &MVP[0][0]);
		if (shader("model_view")!=-1) glUniformMatrix4fv(shader("model_view"), 1, GL_FALSE, &model_view[0][0]);
		if (shader("normal_matrix")!=-1) glUniformMatrix3fv(shader("normal_matrix"), 1, GL_FALSE, &normal_matrix[0][0]);

		if (shader.UniformBlockIndex("Material") !=-1 ) {
			glBindBuffer(GL_UNIFORM_BUFFER, materialID);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(Material), &material, GL_STATIC_DRAW);
			glBindBufferBase(GL_UNIFORM_BUFFER, shader.UniformBlockIndex("Material"), materialID);
		}

		glDrawElements(mode, indices.size(), GL_UNSIGNED_INT, 0);
	shader.End();
	glBindVertexArray(0);
}

void Model::Draw(Shader& shader, mat4 view, mat4 projection) {
	Draw(shader, view, projection, draw_mode);
}

void Model::Draw(mat4 view, mat4 projection) {
	Draw(Content::shader(shader_type), view, projection, draw_mode);
}