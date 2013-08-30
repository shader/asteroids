#include "GL/glut.h"

void handle_menu( int ID )
{
  switch( ID ) {
  case 0:
    exit( 0 );
  }
}

void LoadMenu() {
  glutCreateMenu( handle_menu ); // Setup GLUT popup menu
  glutAddMenuEntry( "Quit", 0 );
  glutAttachMenu( GLUT_RIGHT_BUTTON );
}