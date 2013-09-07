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
Model* model, *little, *medium, *big, *alien;
mat4 Projection, View;
int frameCount, currentTime, previousTime;
string fps;
ostringstream buf;
Timer timer;
bool quit;

void LoadShaders() {
	shader = new Shader();
	shader->Load(GL_VERTEX_SHADER, "shader.vert");
	shader->Load(GL_FRAGMENT_SHADER, "shader.frag");
	shader->Create();
}

void Update(Time time) {
	double t = time.Total().Seconds();
	quat rot(vec3(t, 0, 0));
	model->orientation = rot;
	little->orientation = rot;
	medium->orientation = rot;
	big->orientation = rot;
	alien->orientation = rot;
}

void Render() {
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	model->Draw(shader, Projection * View);
	little->Draw(shader, Projection * View);
	medium->Draw(shader, Projection * View);
	big->Draw(shader, Projection * View);
	alien->Draw(shader, Projection * View);
	glutSwapBuffers();
}

//Reset viewport and projection matrix
void Resize(int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	Projection = perspective(45.0f, (GLfloat)w/h, 1.f, 1000.f);
}

void Close() {
	quit = true;
}

void Initialize() {
	srand(time(0));
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
	
	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
	glEnable( GL_DEPTH_TEST );

	LoadMenu();
}

int main( int argc, char *argv[] )
{	
	glutInit( &argc, argv );
	Initialize();

	LoadShaders();

	View = lookAt(vec3(0,0,10), vec3(0,0,0), vec3(0,1,0));

	little = new Asteroid(5, 0.5, 2);
	little->Bind(shader, GL_LINES);
	little->position = vec3(-4,0,0);
	little->size = vec3(0.25, 0.25, 0.25);

	medium = new Asteroid(5, 0.5, 2);
	medium->Bind(shader, GL_LINES);
	medium->position = vec3(-2,0,0);
	medium->size = vec3(0.5, 0.5, 0.5);

	big = new Asteroid(5, 0.5, 2);
	big->Bind(shader, GL_LINES);
	big->position = vec3(0,0,0);

	model = new Ship();
	model->mesh->Normalize();
	model->Bind(shader, GL_LINES);
	model->position = vec3(2,0,0);

	alien = new Ship();
	alien->Subdivide();
	alien->mesh->Normalize();
	alien->Bind(shader, GL_LINES);
	alien->position = vec3(4,0,0);
	alien->size = vec3(0.5,1,0.5);

	glutDisplayFunc(Render);
	glutReshapeFunc(Resize);
	glutCloseFunc(Close);

	while(!quit) {
		Update(timer.GetTime());
		Render();
		glutMainLoopEvent();
	}
	Cleanup();
	return 0;
}

void Cleanup() {
	delete shader;
	delete model;
	delete little;
	delete medium;
	delete big;
}