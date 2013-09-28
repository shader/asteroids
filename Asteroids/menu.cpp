#include "GL/freeglut.h"

void handle_menu( int ID )
{
	switch( ID ) {
	case 0:
		exit( 0 );
		break;
	case 1:
		glutFullScreenToggle();
		break;
	}
}

void LoadMenu() {
	glutCreateMenu( handle_menu ); // Setup GLUT popup menu
	glutAddMenuEntry( "Quit", 0 );
	glutAddMenuEntry( "Toggle Fullscreen", 1);
	glutAttachMenu( GLUT_RIGHT_BUTTON );
}