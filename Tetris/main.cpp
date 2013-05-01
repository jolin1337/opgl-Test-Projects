#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>

#include "World.h"
/*
Vector poses[4] = {
	Vector(1, 0),
	Vector(0, 2),
	Vector(0, 3),
	Vector(1, 2)
};

BlockSet4 block(poses);*/
World world;
GLint screen_width = 600, screen_height = 600;
bool isFullscreen = false,
	 keys[256];

int init_resources(){
	glMatrixMode(GL_PROJECTION);
	gluPerspective( 45.0,	// fov
					1.0,	// aspect ratio
					20.0, 	// z near
					100.0 	// z far
	);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0.0, 3.0, 6.0,	// eye 
		0.0, 0.0, 0.0,			// center 
		0.0, 1.0, 0.0);			// up direction 


	// Lights:
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	glShadeModel (GL_SMOOTH);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	return 1;
}

void onReshape(int width, int height);
void idle();
void onDisplay();

void onMouseMove(int x, int y);

void onKeyDown(unsigned char Key, int x, int y);
void onKeyUp(unsigned char Key, int x, int y);

void onKeyDownSpecial(int Key, int x, int y);
void onKeyUpSpecial(int Key, int x, int y);

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(screen_width, screen_height);
	glutCreateWindow("TITLES: KESO");

	/* Extension wrangler initialising */
	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
		return EXIT_FAILURE;
	}
	if (!GLEW_VERSION_2_0) {
		fprintf(stderr, "Error: your graphic card does not support OpenGL 2.0\n");
		return 1;
	}

	/* When all init functions runs without errors,
	the program can initialise the resources */
	if (init_resources() == 1) {
		/* And then we can display it if everything goes OK */
		if(isFullscreen)
			glutFullScreen();
		//glEnable(GL_CULL_FACE);
		glutSetCursor(GLUT_CURSOR_NONE);
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glutReshapeFunc(onReshape);
		glutIdleFunc(idle);
		glutDisplayFunc(onDisplay);

		glutPassiveMotionFunc(onMouseMove);
		glutMotionFunc(onMouseMove);

		glutKeyboardFunc(onKeyDown);
		glutKeyboardUpFunc(onKeyUp);
		glutSpecialFunc(onKeyDownSpecial);
		glutSpecialUpFunc(onKeyUpSpecial);
		
		glutMainLoop();
	}
	return 0;
}

void onReshape(int width, int height) {
	screen_width = width;
	screen_height = height;
	glViewport(0, 0, screen_width, screen_height);
	glutWarpPointer(screen_width/2, screen_height/2);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)screen_width / (float)screen_height, 1.0, 200.0);
	glMatrixMode(GL_MODELVIEW);
}

void idle(){
	world.update();
	glutWarpPointer(screen_width/2, screen_height/2);
	glutPostRedisplay();
}

void onDisplay() {
	/* Clear the background as white */
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// Light:
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	world.render();
	/* Display the result */ 
	glutSwapBuffers();
}

void onMouseMove(int x, int y){
	float speed = 0.02;
	world.rotateX((screen_width/2 - x)*speed);
}

void onKeyDown(unsigned char Key, int x, int y){
	switch(Key){
		case 27:
			exit(0);
			break;
		case 'f':
			if(isFullscreen && screen_height > 0 && screen_width > 0){
				isFullscreen = false;
				glutPositionWindow(0,0);
			}
			else{
				glutFullScreen();
				isFullscreen = true;
			}
			break;
	}
	world.onKeyEvent(Key, true);
	
}
void onKeyUp(unsigned char Key, int x, int y){
	world.onKeyEvent(Key, false);
}
void onKeyDownSpecial(int Key, int x, int y){
	keys[Key] = true;
}
void onKeyUpSpecial(int Key, int x, int y){
	keys[Key] = false;
}