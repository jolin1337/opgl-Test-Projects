#include <iostream>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <assert.h>
#include "cheet.h"
 #include <vector>
using namespace std;

int main(int argc, char** argv) { 
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);
	glutInitWindowSize(600, 600);
	
	glutCreateWindow("RANDOM SHOT!");
	init();
	//opgl->object=OB::loadFromFile("obj.o");
	glutDisplayFunc(drawScene); 
	//glutIdleFunc(drawScene);
	glutKeyboardFunc(onKeyDown);
	glutReshapeFunc(onResize);
	glutMouseFunc(onMouseClick);
	glutMotionFunc(onMouseMove);

	glutTimerFunc(25, update, 0);
	
	glutMainLoop();
	return 0;
}









