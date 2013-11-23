#include "object.h"
#include "scene.h"
#include "utils.h"
#include "glyph.h"
#include "content.h"

ScoreBoard::ScoreBoard(Scene *scene) : Object(scene) {
	score = 0;
	Score += [=](int points){ score += points; };
	texture.load2D("resources/spritefont_0.png", false);
	
	glGenVertexArrays(1, &vertex_array);
	glGenBuffers (1, &verticesID);
	glGenBuffers (1, &indicesID);
	glGenBuffers (1, &texcoordsID);

	flags[ObjectFlags::Collide] = false;
}

ScoreBoard::~ScoreBoard() {	
	glDeleteBuffers(1, &verticesID);
	glDeleteBuffers (1, &texcoordsID);
	glDeleteBuffers(1, &indicesID);
	glDeleteVertexArrays(1, &vertex_array);
}

void ScoreBoard::Initialize() {
	score = 0;
}

int ScoreBoard::Points() {
	return score;
}

void ScoreBoard::Draw(mat4 model, mat4 projection) {
	projection = ortho(0.0f, (float)scene->width, (float)scene->height, 0.0f, 0.0f, (float)1);
	string s = to_string((long long)score);
	model = translation(vec3(scene->width - 20 - s.length() * 16,20,0));

	vector<int> indices;
	vector<vec3> vertices;
	vector<vec2> texcoords;

	for (int i = 0; i < s.length(); i++) {
		//quad
		vertices.push_back(vec3(i*16, 0, 0));
		vertices.push_back(vec3(i*16, 32, 0));
		vertices.push_back(vec3(i*16 + 16, 32, 0));
		vertices.push_back(vec3(i*16 + 16, 0, 0));

		vec2 offset = glyphs[s[i] - 32];
		offset.y = 256 - offset.y - ((int)offset.y % 32);
		texcoords.push_back(offset/256.0f);
		texcoords.push_back((offset + vec2(0,-32))/256.0f);
		texcoords.push_back((offset + vec2(16,-32))/256.0f);
		texcoords.push_back((offset + vec2(16,0))/256.0f);

		indices.push_back(i*4);
		indices.push_back(i*4+2);
		indices.push_back(i*4+1);
		indices.push_back(i*4);
		indices.push_back(i*4+3);
		indices.push_back(i*4+2);
	}
	
	Shader *shader = &Content::shader(textured);
	mat4 MVP = projection * model;
		
	shader->Begin();
	glBindVertexArray(vertex_array);
	texture.Bind();

	//position
	glBindBuffer (GL_ARRAY_BUFFER, verticesID);
	glBufferData (GL_ARRAY_BUFFER, sizeof(vec3)*vertices.size(), &vertices[0], GL_STATIC_DRAW);		
	glEnableVertexAttribArray((*shader)["vertex"]);
	glVertexAttribPointer ((*shader)["vertex"], 3, GL_FLOAT, GL_FALSE, 0, 0);
		
	glBindBuffer (GL_ARRAY_BUFFER, texcoordsID);
	glBufferData (GL_ARRAY_BUFFER, sizeof(vec2)*vertices.size(), &texcoords[0], GL_STATIC_DRAW);		
	glEnableVertexAttribArray((*shader)["texcoord"]);
	glVertexAttribPointer((*shader)["texcoord"], 2, GL_FLOAT, GL_FALSE, 0, 0);
				
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), &indices[0], GL_STATIC_DRAW);

	glUniformMatrix4fv((*shader)("MVP"), 1, GL_FALSE, &MVP[0][0]);
	glUniform1i((*shader)("tex"), 0);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		
	glBindVertexArray(0);
	shader->End();
}
