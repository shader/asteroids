#include "headers.h"
using namespace glm;

Model::Model() {
	position = vec3(0.0f, 0.0f, 0.0f);
	size = vec3(1.0f, 1.0f, 1.0f);
	draw_mode = GL_TRIANGLES;
	mesh = new Mesh();
	shader = new Shader();
	glGenVertexArrays(1, &vertex_array);
	glGenBuffers (1, &verticesID);
	glGenBuffers (1, &colorsID);
	glGenBuffers (1, &indicesID);
}

Model::~Model() {
	glDeleteBuffers(1, &verticesID);
	glDeleteBuffers(1, &colorsID);
	glDeleteBuffers(1, &indicesID);
	glDeleteVertexArrays(1, &vertex_array);
	delete mesh;
	delete shader;
}

void Model::LoadShader(GLenum type, string path) {
	shader->Load(type, path);
}


void Model::LoadShaders(string vertex, string fragment) {
	shader->Load(GL_VERTEX_SHADER, vertex);
	shader->Load(GL_FRAGMENT_SHADER, fragment);
	shader->Create();
}

void Model::Bind() {
	Bind(shader, draw_mode);
}

void Model::Bind(Shader* shader) {
	Bind(shader, draw_mode);
}

void Model::Bind(Shader* shader, GLenum mode) {
	//Bind model vertex data to VertexArray, so it can be used for drawing later. Call whenever vertex data is updated
	vec3* vertices;
	indices.empty();

	vertices = new vec3[mesh->vertices->size()];
	for (set<Vertex*>::iterator i=mesh->vertices->begin(); i!=mesh->vertices->end(); i++)
		vertices[(*i)->index] = ((*i)->position);

	draw_mode = mode;
	switch (mode) {
	case(GL_POINTS):
		for (GLuint i=0; i<mesh->vertices->size(); i++)
			indices.push_back(i);
		break;
	case(GL_LINES):
		for (auto e = mesh->edges->begin(); e != mesh->edges->end(); e++) {
			indices.push_back((*e)->tail->index);
			indices.push_back((*e)->head->index);
		}
		break;
	case(GL_TRIANGLES):
		for (auto f = mesh->faces->begin(); f != mesh->faces->end(); f++) {
			Edge* e = (*f)->first; //get first edge in face
			indices.push_back(e->tail->index);
			indices.push_back(e->head->index);
			indices.push_back(e->next->head->index);
		}
		break;
	};

	glBindVertexArray(vertex_array);	 
	shader->Begin();
		glBindBuffer (GL_ARRAY_BUFFER, verticesID);
		glBufferData (GL_ARRAY_BUFFER, sizeof(vec3)*mesh->vertices->size(), vertices, GL_STATIC_DRAW);		
		glEnableVertexAttribArray((*shader)["vertex"]);
		glVertexAttribPointer ((*shader)["vertex"], 3, GL_FLOAT, GL_FALSE, 0, 0);
		
		if (colors.size() > 0) {
			glBindBuffer (GL_ARRAY_BUFFER, colorsID);
			glBufferData (GL_ARRAY_BUFFER, sizeof(Color)*colors.size(), &colors[0], GL_STATIC_DRAW);		
			glEnableVertexAttribArray((*shader)["color"]);
			glVertexAttribPointer ((*shader)["color"], 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), &indices[0], GL_STATIC_DRAW);
	shader->End();
	glBindVertexArray(0);
	delete [] vertices;
}

void Model::Draw(Shader* shader, mat4 ViewProjection, GLenum mode) {
	mat4 model = translation(position) * mat4_cast(orientation) * scale(size);
	mat4 MVP = ViewProjection * model;
	glBindVertexArray(vertex_array);
	shader->Begin();
		glUniformMatrix4fv((*shader)("MVP"), 1, GL_FALSE, &MVP[0][0]);
		glDrawElements(mode, indices.size(), GL_UNSIGNED_INT, 0);
	shader->End();
	glBindVertexArray(0);
}

void Model::Draw(Shader* shader, mat4 view_projection) {
	Draw(shader, view_projection, draw_mode);
}

void Model::Draw(mat4 view_projection) {
	Draw(shader, view_projection, draw_mode);
}

void Model::Subdivide(int times) {
	Mesh* new_mesh;
	for (int i=0; i<times; i++) {
		new_mesh = subdivide(mesh);
		delete mesh;
		mesh = new_mesh;
	}
}

void Model::Perturb(float max_radius) {
	mesh->Perturb(max_radius);
}