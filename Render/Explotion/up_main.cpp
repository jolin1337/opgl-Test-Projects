#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "Bim.h"
#include "Smoke.h"
#include "BallExplotion.h"

const GLuint BIM_COUNT = 1;

// Overal Test:
Bim bims[BIM_COUNT];

// Smoke Test:
Smoke sma("img/exptextures3.jpg", AIR, 10.0f);	// Air
Smoke smf(FIRE, 2.0f);	// Fire
BallExplotion bomb;

int fuel = 0;


void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void display(void);
void newExplosion(bool exec = true);
void idle(void);
void init(void);

int main(int argc, char *argv[]) {
	glutInit (&argc, argv);
	srand (time (NULL));

	glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	
	glutInitWindowSize(600, 600);
	glutCreateWindow ("Explosion example");
	init();

	glutMainLoop ();
	return 0;
}

void init(void){
	//glutFullScreen();

	smf.setImage(sma.getImage());

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutKeyboardFunc (keyboard);
	glutIdleFunc (idle);
	glutDisplayFunc (display);
	glutReshapeFunc (reshape);
}
void reshape (int w, int h){
	glViewport (0.0, 0.0, (GLfloat) w, (GLfloat) h);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (45.0, (GLfloat) w / (GLfloat) h, 0.1, 100.0);
	
	glMatrixMode (GL_MODELVIEW);
}

void idle(void){
	for (int i = 0; i < BIM_COUNT; i++) {
		bims[i].update();
	}
	if(bims[0].hasDetonated()){
		sma.update();
		smf.update();
	}
	bomb.update();
	glutPostRedisplay ();
}
void display (void){
	glClearColor(0.03,0.03,0.03, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity ();

	/* Place the camera */
	float size = 0.8f;

	glTranslatef (0.0, 0.0, -3.0);
	glRotatef (40.0f, 1.0f, 0.0f, 0.0f);

	/*glPushMatrix();
	glScalef(size, size, size);
	for (int i = 0; i < BIM_COUNT; i++) {
		bims[i].render();
	}
	glPopMatrix();
	*/
	//glScalef(size*1.5f, size*1.5f, size*1.5f);
	if(bims[0].hasDetonated() && !bims[0].hasExploded()){
		sma.renderPrep();
		sma.render(Vector3(-40.0f, 0.0f, 0.0f));
		smf.renderPrep();
		//for (int j = 0; j < 3; j++) {
			smf.render(Vector3(-40.0f, 0.0f, 0.0f));
		//}
	}

	bomb.render();

	glutSwapBuffers ();
}

void keyboard (unsigned char key, int x, int y){
	switch(key){
		case ' ':
			newExplosion();
			break;
		case 'f':
			glutFullScreen();
			break;
		case 'n':
			glutPositionWindow(0,0);
			break;
		default:
			exit(0);
			break;		
	}
}
void newExplosion (bool exec){
	int bc = BIM_COUNT;
	for (int i = 0; i < bc; i++) {
		bool hasD = !bims[i].hasDetonated();
		Vector3 pos = bims[i].position;
		bims[i] = Bim();
		bims[i].position = pos;
		if(hasD && exec){
			bims[i].detonate();
			sma.reset();
			smf.reset();
		}
	}
}