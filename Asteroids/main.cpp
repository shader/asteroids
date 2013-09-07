#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

#include "headers.h"
using namespace std;
using namespace glm;

void LoadShaders();
void Resize(int w, int h);
void Initialize();
void Cleanup();

Timer timer;
bool quit;
DefaultScene scene;
Asteroid *little, *medium, *big;
Ship *ship;
mat4 Projection, View;

void Close() {
	quit = true;
}

void Resize(int w, int h) {
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	Projection = perspective(45.0f, (GLfloat)w/h, 1.f, 1000.f);
}

void Mouse(int button, int state, int x, int y) {
	scene.Mouse(button, state, x, y);
}

void Keyboard(unsigned char key, int x, int y) {
	scene.Keyboard(key, x, y);
}

void Update(Time time) {	
	double t = time.Total().Seconds();
	quat rot(vec3(0, t, t));
	little->orientation = rot;
	medium->orientation = rot;
	big->orientation = rot;
	ship->orientation = rot;
}

void Draw() {
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	little->Draw(Projection * View);
	medium->Draw(Projection * View);
	big->Draw(Projection * View);
	ship->Draw(Projection * View);
	glutSwapBuffers();
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

	View = lookAt(vec3(0,0,5), vec3(0,0,0), vec3(0,1,0));

	little = new Asteroid(5, 0.5, 2);
	little->LoadShaders("shader.vert", "shader.frag");
	little->Bind();
	little->position = vec3(-3,0,0);
	little->size = vec3(0.25, 0.25, 0.25);

	medium = new Asteroid(5, 0.5, 2);
	medium->LoadShaders("shader.vert", "shader.frag");
	medium->Bind();
	medium->position = vec3(-1,0,0);
	medium->size = vec3(0.5, 0.5, 0.5);

	big = new Asteroid(5, 0.5, 2);
	big->LoadShaders("shader.vert", "shader.frag");
	big->Bind();
	big->position = vec3(1,0,0);

	ship = new Ship();
	ship->LoadShaders("shader.vert", "shader.frag");
	ship->Bind();
	ship->position = vec3(1,0,0);
	ship->mesh->Normalize();
}

int main( int argc, char *argv[] )
{	
	glutInit( &argc, argv );
	Initialize();
	
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutReshapeFunc(Resize);
	glutCloseFunc(Close);

	while(!quit) {
		Update(timer.GetTime());
		Draw();
		glutMainLoopEvent();
	}
	Cleanup();
	return 0;
}

void Cleanup() {
	delete little;
	delete medium;
	delete big;
	delete ship;
}