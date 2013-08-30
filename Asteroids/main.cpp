#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

#include "headers.h"
using namespace std;
using namespace glm;

void LoadShaders();
void Update();
void Render();
void Resize();
void Initialize();
void Cleanup();

Shader* shader;
Model model;
mat4 Projection, View;
int frameCount, currentTime, previousTime;
string fps;
ostringstream buf;

void LoadShaders() {
	shader = new Shader();
	shader->Load(GL_VERTEX_SHADER, "shader.vert");
	shader->Load(GL_FRAGMENT_SHADER, "shader.frag");
	shader->Create();
}

float t = 0;
void Update() {
	t+=0.001f;
	model.orientation = quat(vec3(0, t, t));
	glutPostRedisplay();
}

void Render() {
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	model.Draw(shader, Projection * View);
	glutSwapBuffers();
}

//Reset viewport and projection matrix
void Resize(int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	Projection = perspective(45.0f, (GLfloat)w/h, 1.f, 1000.f);
}

void Initialize() {
	//Initialize GLUT
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );		
	glutInitContextVersion (3, 3);
	glutInitContextFlags (GLUT_CORE_PROFILE | GLUT_DEBUG);	
	glutInitWindowSize(800,600);
	glutCreateWindow( "Asteroids - Samuel Christie" );

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)	{
		cerr << "Error: " << glewGetErrorString(err) << endl;
	}
	
	glEnable(GL_DEPTH);
	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

	LoadMenu();
}

int main( int argc, char *argv[] )
{	
	glutInit( &argc, argv );
	Initialize();

	LoadShaders();

	View = lookAt(vec3(0,0,5), vec3(0,0,0), vec3(0,1,0));		
		
	model = Icosahedron();
	model.Init();
	for (int i=0;i<model.vertices.size();i++) destructive_perturb(&model.vertices[i], 0.5);
	model.Bind(shader, GL_LINES);

	glutDisplayFunc(Render);
	glutReshapeFunc(Resize);
	glutIdleFunc(Update);
	glutMainLoop();
	Cleanup();
	return 0;
}

void Cleanup() {
	delete shader;
}