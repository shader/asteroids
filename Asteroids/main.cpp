#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

#include "headers.h"
using namespace std;
using namespace glm;

void LoadShaders();
void Initialize();
void Cleanup();

Timer timer;
bool quit;
Scene* scene;
InputState input;

void Close() {
	quit = true;
}

void Mouse(int button, int state, int x, int y) {
	input.mouse.buttons[button] = (state == GLUT_DOWN);
	input.mouse.x = x;
	input.mouse.y = y;
}

void KeyboardDown(unsigned char key, int x, int y) {
	input.keyboard[key] = true;
	input.mouse.x = x;
	input.mouse.y = y;
}

void KeyboardUp(unsigned char key, int x, int y) {
	input.keyboard[key] = false;
	input.mouse.x = x;
	input.mouse.y = y;
}

void KeyboardSpecialDown(int key, int x, int y) {
	input.special_keys[key] = true;
	input.mouse.x = x;
	input.mouse.y = y;
}

void KeyboardSpecialUp(int key, int x, int y) {
	input.special_keys[key] = false;
	input.mouse.x = x;
	input.mouse.y = y;
}

void Draw() {
	scene->Draw();
}

void Initialize() {
	srand(time(0));
	//Initialize GLUT
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);		
	glutInitContextVersion (4, 0);
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

	LoadMenu();
}

int main( int argc, char *argv[] )
{	
	glutInit( &argc, argv );
	Initialize();
	
	glutMouseFunc(Mouse);
	glutKeyboardFunc(KeyboardDown);
	glutKeyboardUpFunc(KeyboardUp);
	glutSpecialFunc(KeyboardSpecialDown);
	glutSpecialUpFunc(KeyboardSpecialUp);
	glutCloseFunc(Close);

	scene = new DefaultScene();
	scene->Initialize();

	while(!quit) {
		scene->Update(timer.GetTime(), input);
		scene->Draw();
		glutMainLoopEvent();
	}
	Cleanup();
	return 0;
}

void Cleanup() {
	delete scene;
}