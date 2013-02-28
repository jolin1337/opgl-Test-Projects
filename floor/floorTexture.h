#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <math.h>       /* for cos(), sin(), and sqrt() */
#include <GL/glut.h>    /* OpenGL Utility Toolkit header */


/* Some <math.h> files do not define M_PI... */
#ifndef M_PI
#define M_PI 3.14159265
#endif

static int linearFiltering = 0, useMipmaps = 0,
	   showTexture = 1;


static GLfloat  lightPosition[4];
static GLfloat  lightColor[] = {0.8, 1.0, 0.8, 1.0};
static int showLight = 1;
static float lightAngle = 0.0, lightHeight = 20;

/* Nice floor texture tiling pattern. */
static std::string circles[] = {
	"...............x",
	"......xxxx.....x",
	"....xxxxxxxx...x",
	"...xxxxxxxxxx..x",
	"...xxxxx.xxxx..x",
	"..xxxx....xxxx.x",
	"..xxx......xxx.x",
	"..xxxx....xxxx.x",
	"..xxxxx..xxxxx.x",
	"...xxxxxxxxxx..x",
	"...xxxxxxxxxx..x",
	"....xxxxxxxx...x",
	"......xxxx.....x",
	"...............x",
	"...............x",
	"xxxxxxxxxxxxxxxx",
};


static void makeFloorTexture(void) {
	GLubyte floorTexture[16][16][3];
	GLubyte *loc;
	int s, t;

	/* Setup RGB image for the texture. */
	loc = (GLubyte*) floorTexture;
	for (t = 0; t < 16; t++) {
		for (s = 0; s < 16; s++) {
			if (circles[t][s] == 'x') {
			 /* Nice green. */
				loc[0] = 0xaf;
				loc[1] = 0xaf;
				loc[2] = 0x7f;
			} else {
			 /* Light gray. */
				loc[0] = 0xf0;
				loc[1] = 0xf0;
				loc[2] = 0xe0;
			}
			loc += 3;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	if (useMipmaps) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_LINEAR);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 16, 16,
			GL_RGB, GL_UNSIGNED_BYTE, floorTexture);
	} else {
		if (linearFiltering) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		} else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
		glTexImage2D(GL_TEXTURE_2D, 0, 3, 16, 16, 0,
			GL_RGB, GL_UNSIGNED_BYTE, floorTexture);
	}
}


static GLfloat floorVertices[4][3] = {
	{ -20.0, 0.0, 20.0 },
	{ 20.0, 0.0, 20.0 },
	{ 20.0, 0.0, -20.0 },
	{ -20.0, 90.0, -20.0 },
};
/* Draw a floor (possibly textured). */
static void drawFloor(void) {
	if(showLight)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);

	if(showTexture){
		glEnable(GL_TEXTURE_2D);

		glBegin(GL_TRIANGLES);
			glTexCoord2f(0.0, 0.0);
			glVertex3fv(floorVertices[0]);
			glTexCoord2f(0.0, 16.0);
			glVertex3fv(floorVertices[1]);
			glTexCoord2f(16.0, 16.0);
			glVertex3fv(floorVertices[2]);
			glTexCoord2f(0.0, 0.0);
			glVertex3fv(floorVertices[0]);
			glTexCoord2f(16.0, 16.0);
			glVertex3fv(floorVertices[2]);
			glTexCoord2f(16.0, 0.0);
			glVertex3fv(floorVertices[3]);
		glEnd();

		glDisable(GL_TEXTURE_2D);
	}
	else{
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
			glVertex3fv(floorVertices[0]);
			glVertex3fv(floorVertices[1]);
			glVertex3fv(floorVertices[2]);
			glVertex3fv(floorVertices[3]);
		glEnd();
	}
}

void redrawFloor(){


	/* Reposition the light source. */
	lightPosition[0] = 12*cos(lightAngle);
	lightPosition[1] = lightHeight;
	lightPosition[2] = 12*sin(lightAngle);
	lightPosition[3] = 1.0;
	
	glPushMatrix();
		/* Perform scene rotations based on user mouse input. */
		glRotatef(angle2, 1.0, 0.0, 0.0);
		glRotatef(angle, 0.0, 1.0, 0.0);
		 
		/* Tell GL new light source position. */
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);


		glFrontFace(GL_CW);  /* Switch face orientation.*/ 
		glColor4f(0.1, 0.1, 0.7, 1.0);
		drawFloor();
		glFrontFace(GL_CCW);
		
		glColor4f(0.7, 0.0, 0.0, 0.3);
		glColor4f(1.0, 1.0, 1.0, 0.3);
		drawFloor();

		if(showLight){
			glPushMatrix();
				glDisable(GL_LIGHTING);
				glColor3f(1.0, 1.0, 0.0);
				/* Draw a yellow ball at the light source. */
				glTranslatef(-lightPosition[0], -lightPosition[1], -lightPosition[2]);
				glutSolidSphere(0.2, 5, 5);
				glEnable(GL_LIGHTING);
			glPopMatrix();
		}

	glPopMatrix();

}

enum {
	M_NONE, M_LIGHT, M_TEXTURE
};

void floorSettings(int type){
	switch(type){
		case M_LIGHT:
			showLight = !showLight;
			break;
		case M_TEXTURE:
			showTexture = !showTexture;
	}
	glutPostRedisplay();
}


void initFloor(int argc, char **argv){
	int i;
	for (i=1; i<argc; i++) {
		if (!strcmp("-linear", argv[i])) {
			linearFiltering = 1;
		} else if (!strcmp("-mipmap", argv[i])) {
			useMipmaps = 1;
		}
	}

	glutAddMenuEntry("Toggle Light", M_LIGHT);
	glutAddMenuEntry("Toggle Texture", M_TEXTURE);
	glutAddMenuEntry("----------------", M_NONE);


	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glLineWidth(3.0);

	glMatrixMode(GL_PROJECTION);
	gluPerspective( /* field of view in degree */ 45.0,
	/* aspect ratio */ 1.0,
		/* Z near */ 20.0, /* Z far */ 100.0);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0.0, 8.0, 60.0,  /* eye is at (0,0,30) */
		0.0, 8.0, 0.0,      /* center is at (0,0,0) */
		0.0, 1.0, 0.);      /* up is in postivie Y direction */

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.1);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	makeFloorTexture();

}