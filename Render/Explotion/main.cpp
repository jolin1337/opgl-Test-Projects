/*
 * explosion.c - Simple particle system based explosion demonstration.
 *                   ,
 * Author: Gustav Taxen, nv91-gta@nada.kth.se
 *
 * This program is in the Public Domain. You may freely use, modify
 * and distribute this program.
 *
 *
 * Description:
 *    This program shows an easy way to make simple explosions using
 *    OpenGL. Press SPACE to blow the rotating cube to pieces! Use
 *    the menu to toggle between normalized speed vectors and non-
 *    normalized speed vectors.
 *
 * Theory:
 *    This program uses an extremely simple particle system to create
 *    an explosion effect. Each particle is moved from the origin
 *    towards a random direction and, if activated, a random speed.
 *    The color of the particles are changed from white to orange to
 *    red to create a glowing effect. The GL_POINTS primitive is used
 *    to render the particles.
 *       The debris is similar to the particles, with the addition of
 *    orientation and orientation speed.
 *       A point light source is placed in the center of the explosion.
 * 
 */


#include <GL/glut.h>

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <sys/types.h>


#define NUM_PARTICLES    10000         /* Number of particles  */
#define NUM_DEBRIS       30            /* Number of debris     */

/* GLUT menu entries */

enum{
	PAUSE, NORMALIZE_SPEED, QUIT
};


/* A particle */

struct particleData {
	float   position[3];
	float   speed[3];
	float   color[3];
};


/* A piece of debris */

struct debrisData {
	float   position[3];
	float   speed[3];
	float   orientation[3];        /* Rotation angles around x, y, and z axes */
	float   orientationSpeed[3];
	float   color[3];
	float   scale[3];
};


/* Globals */

particleData     particles[NUM_PARTICLES];
debrisData       debris[NUM_DEBRIS];   
int              fuel = 0;                /* "fuel" of the explosion */

float            angle = 0.0;             /* camera rotation angle */


/* Light sources and material */

GLfloat  light0Amb[4] =  { 1.0, 0.6, 0.2, 1.0 };
GLfloat  light0Dif[4] =  { 1.0, 0.6, 0.2, 1.0 };   
GLfloat  light0Spec[4] = { 0.0, 0.0, 0.0, 1.0 };   
GLfloat  light0Pos[4] =  { 0.0, 0.0, 0.0, 1.0 };

GLfloat  light1Amb[4] =  { 0.0, 0.0, 0.0, 1.0 };
GLfloat  light1Dif[4] =  { 1.0, 1.0, 1.0, 1.0 };   
GLfloat  light1Spec[4] = { 1.0, 1.0, 1.0, 1.0 };   
GLfloat  light1Pos[4] =  { 0.0, 5.0, 5.0, 0.0 };

GLfloat  materialAmb[4] = { 0.25, 0.22, 0.26, 1.0 };
GLfloat  materialDif[4] = { 0.63, 0.57, 0.60, 1.0 };
GLfloat  materialSpec[4] = { 0.99, 0.91, 0.81, 1.0 };
GLfloat  materialShininess = 27.8;


int      wantNormalize = 1;   /* Speed vector normalization flag */
int      wantPause = 0;       /* Pause flag */



/*
 * newSpeed
 *
 * Randomize a new speed vector.
 *
 */

void newSpeed (float dest[3]) {
	float x;
	float y;
	float z;
	float len;

	x = ((2.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0)*0.2;
	y = ((2.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0)*0.2;
	z = ((2.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0)*0.2;

	/*
	 * Normalizing the speed vectors gives a "fireball" effect
	 *
	 */

	if (wantNormalize) {
		len = sqrt (x * x + y * y + z * z);

		if (len) {
			x = x / len;
			y = y / len;
			z = z / len;
		}
	}

	dest[0] = x;
	dest[1] = y;
	dest[2] = z;
}


/*
 * newExplosion
 *
 * Create a new explosion.
 *
 */

void newExplosion (void) {
	int i;

	for (i = 0; i < NUM_PARTICLES; i++) {
		particles[i].position[0] = 0.0;
		particles[i].position[1] = 0.0;
		particles[i].position[2] = 0.0;

		particles[i].color[0] = 1.0;
		particles[i].color[1] = 1.0;
		particles[i].color[2] = 0.5;

		newSpeed (particles[i].speed);
	}

	for (i = 0; i < NUM_DEBRIS; i++) {
		debris[i].position[0] = 0.0;
		debris[i].position[1] = 0.0;
		debris[i].position[2] = 0.0;

		debris[i].orientation[0] = 0.0;
		debris[i].orientation[1] = 0.0;
		debris[i].orientation[2] = 0.0;

		debris[i].color[0] = 0.7;
		debris[i].color[1] = 0.7;
		debris[i].color[2] = 0.7;

		debris[i].scale[0] = (2.0 * 
				((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
		debris[i].scale[1] = (2.0 * 
				((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
		debris[i].scale[2] = (2.0 * 
				((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;

		newSpeed (debris[i].speed);
		newSpeed (debris[i].orientationSpeed);
	}
	
	fuel = 50;
}


/*
 * display
 *
 * Draw the scene.
 *
 */

void display (void) {
	int i;

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity ();

	/* Place the camera */

	glTranslatef (0.0, 0.0, -10.0);
	glRotatef (angle, 0.0, 1.0, 0.0);

	/* If no explosion, draw cube */

	if (fuel == 0) {
		glEnable (GL_LIGHTING);
		glDisable (GL_LIGHT0);
		glEnable (GL_DEPTH_TEST);
		glutSolidCube (0.7);
	}
	if (fuel > 0) {
		glPushMatrix ();

		glDisable (GL_LIGHTING);
		glDisable (GL_DEPTH_TEST);

		glBegin (GL_POINTS);

		for (i = 0; i < NUM_PARTICLES; i++) {
			glColor3fv (particles[i].color);
			glVertex3fv (particles[i].position);
		}
	
		glEnd ();

		glPopMatrix ();

		glEnable (GL_LIGHTING); 
		glEnable (GL_LIGHT0); 
		glEnable (GL_DEPTH_TEST);

		glNormal3f (0.0, 0.0, 1.0);

		for (i = 0; i < NUM_DEBRIS; i++) {
			glColor3fv (debris[i].color);

			glPushMatrix ();
				
			glTranslatef (debris[i].position[0],
				debris[i].position[1],
				debris[i].position[2]);

			glRotatef (debris[i].orientation[0], 1.0, 0.0, 0.0);
			glRotatef (debris[i].orientation[1], 0.0, 1.0, 0.0);
			glRotatef (debris[i].orientation[2], 0.0, 0.0, 1.0);

			glScalef (debris[i].scale[0],
					debris[i].scale[1],
					debris[i].scale[2]);

			glBegin (GL_TRIANGLES);
			glVertex3f (0.0, 0.5, 0.0);
			glVertex3f (-0.25, 0.0, 0.0);
			glVertex3f (0.25, 0.0, 0.0);
			glEnd ();	  
			
			glPopMatrix ();
		}
	}

	glutSwapBuffers ();
}


/*
 * keyboard
 *
 * Keyboard callback.
 *
 */

void keyboard (unsigned char key, int x, int y) {
	switch (key){
		case ' ':
			newExplosion ();
			break;
		case 27:
			exit (0);
			break;
		case 'p':
			wantPause = 1 - wantPause;
			break;
	}
}


/*
 * idle
 *
 * Update animation variables.
 *
 */

void idle (void) {
	int i;

	if (!wantPause) {
		if (fuel > 0) {
			for (i = 0; i < NUM_PARTICLES; i++) {
				particles[i].position[0] += particles[i].speed[0] * 0.2;
				particles[i].position[1] += particles[i].speed[1] * 0.2;
				particles[i].position[2] += particles[i].speed[2] * 0.2;
				
				particles[i].color[0] -= 1.0 / 40.0;
				if (particles[i].color[0] < 0.0) {
					particles[i].color[0] = 0.0;
				}
				
				particles[i].color[1] -= 1.0 / 20.0;
				if (particles[i].color[1] < 0.0){
					particles[i].color[1] = 0.0;
				}
					
				particles[i].color[2] -= 1.0 / 15.0;
				if (particles[i].color[2] < 0.0) {
					particles[i].color[2] = 0.0;
				}
			}
		
			for (i = 0; i < NUM_DEBRIS; i++) {
				debris[i].position[0] += debris[i].speed[0] * 0.1;
				debris[i].position[1] += debris[i].speed[1] * 0.1;
				debris[i].position[2] += debris[i].speed[2] * 0.1;
				
				debris[i].orientation[0] += debris[i].orientationSpeed[0] * 10;
				debris[i].orientation[1] += debris[i].orientationSpeed[1] * 10;
				debris[i].orientation[2] += debris[i].orientationSpeed[2] * 10;
			}
		
			--fuel;
		}
		else
			angle += 0.3;  /* Always continue to rotate the camera */
	}
	
	glutPostRedisplay ();
}


/*
 * reshape
 *
 * Window reshape callback.
 *
 */

void reshape (int w, int h) {
	glViewport (0.0, 0.0, (GLfloat) w, (GLfloat) h);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (45.0, (GLfloat) w / (GLfloat) h, 0.1, 100.0);
	
	glMatrixMode (GL_MODELVIEW);
}


/*
 * menuSelect
 *
 * Menu selection callback.
 *
 */

void menuSelect (int value) {
	switch (value){
		case PAUSE:
			wantPause = 1 - wantPause;
			break;

		case NORMALIZE_SPEED:
			wantNormalize = 1 - wantNormalize;
			break;
			
		case QUIT:
			exit (0);
			break;
	}
}


/*
 * main
 *
 * Setup OpenGL and hand over to GLUT.
 *
 */

int main (int argc, char *argv[]) {
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	
	glutCreateWindow ("Explosion demo");
	glutKeyboardFunc (keyboard);
	glutIdleFunc (idle);
	glutFullScreen();
	glutDisplayFunc (display);
	glutReshapeFunc (reshape);

	srand (time (NULL));

	glEnable (GL_LIGHT0);
	glEnable (GL_LIGHT1);
	glLightfv (GL_LIGHT0, GL_AMBIENT, light0Amb);
	glLightfv (GL_LIGHT0, GL_DIFFUSE, light0Dif);
	glLightfv (GL_LIGHT0, GL_SPECULAR, light0Spec);
	glLightfv (GL_LIGHT0, GL_POSITION, light0Pos);
	glLightfv (GL_LIGHT1, GL_AMBIENT, light1Amb);
	glLightfv (GL_LIGHT1, GL_DIFFUSE, light1Dif);
	glLightfv (GL_LIGHT1, GL_SPECULAR, light1Spec);
	glLightfv (GL_LIGHT1, GL_POSITION, light1Pos);
	glLightModelf (GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	//glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT, materialAmb);
	//glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, materialDif);
	//glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, materialSpec);
	//glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, materialShininess);
	glEnable (GL_NORMALIZE);

	glutCreateMenu (menuSelect);
	glutAddMenuEntry ("Pause", PAUSE);
	glutAddMenuEntry ("Toggle normalized speed vectors", NORMALIZE_SPEED);
	glutAddMenuEntry ("Quit", QUIT);
	glutAttachMenu (GLUT_RIGHT_BUTTON);

	glutMainLoop ();

	return 0;
}
