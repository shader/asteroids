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

Timer timer;
bool quit;
Scene scene;

void Update(Time time) {
	double t = time.Total().Seconds();
	model->orientation = quat(vec3(0, t, t));
}

	shader->Load(GL_VERTEX_SHADER, "shader.vert");
	shader->Load(GL_FRAGMENT_SHADER, "shader.frag");
	shader->Create();

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

	scene = DefaultScene();
	scene.Initialize();

	glutCloseFunc(Close);

	while(!quit) {
		scene.Update(timer.GetTime());
		Render();
		glutMainLoopEvent();
	}
	Cleanup();
	return 0;
}

void Cleanup() {}