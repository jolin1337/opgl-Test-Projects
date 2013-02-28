#include <iostream>
#include <stdlib.h>

#include "GL/glew.h"

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <GL/freeglut.h>

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
	
	glutIdleFunc(update);
	glutTimerFunc(0, tf, 0);
	
#ifdef GLEW_OK
	GLenum GlewInitResult;
	GlewInitResult = glewInit();

	if (GLEW_OK != GlewInitResult) {
		fprintf(
			stderr,
			"ERROR: %s\n",
			glewGetErrorString(GlewInitResult)
		);
		exit(EXIT_FAILURE);
	}
	
	fprintf(
		stdout,
		"INFO: OpenGL Version: %s\n",
		glGetString(GL_VERSION)
	);
#endif

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glutMainLoop();
	return 0;
}