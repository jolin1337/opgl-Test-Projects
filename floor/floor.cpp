/***
	Floor example/test for Glut

*/

#include "mouse.h"
#include "key.h"
#include "floorTexture.h"

bool texture = false, reflect = false, shadow = false;

void redraw(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(texture)
		redrawFloor();
	glutSwapBuffers();
}
void settings(int v){
	if(texture)
		floorSettings(v);
}
void update(){
	glutPostRedisplay();
}

int main(int argc, char **argv)
{

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);

#if 0
	/* In GLUT 4.0, you'll be able to do this an be sure to
		 get 2 bits of stencil if the machine has it for you. */
	glutInitDisplayString("samples stencil>=2 rgb double depth");
#endif

	glutCreateWindow("1337");
	glutInitWindowSize(600, 600);
	glutFullScreen();


	glutCreateMenu(settings);
	glutAddMenuEntry("----------------", M_NONE);

	int i;
	for (i=1; i<argc; i++) {
		if (!strcmp("texture", argv[i])) {
			initFloor(argc, argv);
			texture = true;
		} else if(!strcmp("reflect", argv[i])){
			reflect = true;
		} else if(!strcmp("shadow", argv[i])){
			shadow = true;
		}
	}

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	if (shadow && glutGet(GLUT_WINDOW_STENCIL_SIZE) <= 1) {
		printf("dinoshade: Sorry, I need at least 2 bits of stencil.\n");
		exit(1);
	}

	/* Register GLUT callbacks. */
	glutDisplayFunc(redraw);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(key);
	glutSpecialFunc(special);
	glutIdleFunc(update);
	glutMainLoop();
	return 0;             /* ANSI C requires main to return int. */
}
