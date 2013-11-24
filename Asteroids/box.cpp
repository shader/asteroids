#include "primitives.h"
#include "content.h"

LineBox::LineBox() {
	glGenVertexArrays(1, &vertex_array);
	glGenBuffers(1, &verticesID);
	glGenBuffers(1, &indicesID);
}

void LineBox::Bind() {
	Shader &shader = Content::shader(polygon);

	vector<vec3>vertices;
	int indices[] = {
		0,2, 0,4, 0,6,
		1,3, 1,5, 1,7,
		2,5, 2,7, 3,4,
		3,6, 4,7, 5,6
	};

	vertices.push_back(lower);
	vertices.push_back(upper);
	for (int i=0; i < 3; i++) {
		vec3 a = lower, b = upper;
		a[i] = upper[i];
		b[i] = lower[i];
		vertices.push_back(a);
		vertices.push_back(b);
	}

	glBindVertexArray(vertex_array);

	glBindBuffer (GL_ARRAY_BUFFER, verticesID);
	glBufferData (GL_ARRAY_BUFFER, sizeof(vec3)*vertices.size(), &vertices[0], GL_STATIC_DRAW);		
	glEnableVertexAttribArray(shader["vertex"]);
	glVertexAttribPointer (shader["vertex"], 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*24, indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void LineBox::Draw(mat4 view, mat4 projection) {
	Shader &shader = Content::shader(polygon);
	mat4 MVP = projection * view;
	
	glBindVertexArray(vertex_array);
	shader.Begin();
		glUniformMatrix4fv(shader("MVP"), 1, GL_FALSE, &MVP[0][0]);
		glUniform1i(shader("point_size"), 2);
		glUniform4f(shader("uni_color"), 1.0f, 0.0f, 0.0f, 0.5f); // red
		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
	shader.End();
	glBindVertexArray(0);
}