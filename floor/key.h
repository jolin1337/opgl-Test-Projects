
#include <GL/glut.h>  

static void key(unsigned char c, int x, int y)
{
	if (c == 27) {
		exit(0); 
	}
	glutPostRedisplay();
}

static void special(int k, int x, int y)
{
	glutPostRedisplay();
}