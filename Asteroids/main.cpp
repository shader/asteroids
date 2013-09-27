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
Scene* scene;

void Close() {
	quit = true;
}

void Resize(int w, int h) {
	scene->Resize(w, h);
}

void Mouse(int button, int state, int x, int y) {
	scene->Mouse(button, state, x, y);
}

void Keyboard(unsigned char key, int x, int y) {
	scene->Keyboard(key, x, y);
}

void Keyboard(int key, int x, int y) {
	scene->Keyboard(key, x, y);
}

void Draw() {
	scene->Draw();
}

void Initialize() {
	srand(time(0));
	//Initialize GLUT
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);		
	glutInitContextVersion (4, 1);
	glutInitContextFlags (GLUT_CORE_PROFILE | GLUT_DEBUG);	
	glutInitWindowSize(800,600);
	glutCreateWindow( "Asteroids - Samuel Christie" ); 

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)	{
		cerr << "Error: " << glewGetErrorString(err) << endl;
	}
	
	glPolygonMode (GL_FRONT, GL_FILL);
	glEnable( GL_DEPTH_TEST );
	glEnable(GL_BLEND);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH, GL_NICEST);

	LoadMenu();
}

int main( int argc, char *argv[] )
{	
	glutInit( &argc, argv );
	Initialize();
	
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Keyboard);
	glutReshapeFunc(Resize);
	glutCloseFunc(Close);

	scene = new DefaultScene();
	scene->Initialize();

	while(!quit) {
		scene->Update(timer.GetTime());
		scene->Draw();
		glutMainLoopEvent();
	}
	Cleanup();
	return 0;
}

void Cleanup() {
	delete scene;
}