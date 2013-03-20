/* Using the standard output for fprintf */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
/* Use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* Using the GLUT library for the base windowing setup */
#include <GL/glut.h>

#include "./tex/hatt_texture.c"
#include "utils/Matrix4.h"
#include "config.h"
/* ADD GLOBAL VARIABLES HERE */


GLint screen_width = 600, screen_height = 600;

std::string vertexShaderFileName = "shader.v.glsl";
std::string fragmentShaderFileName = "shader.f.glsl";

#include "setup.h"
#include "Data.h"
#include "Character.h"

void keyboardEvent();

Character player;
GL_Object cube;
Bone ctrl;
int init_resources(void) {
	try{

		player.loadObj("crazy.o");
		cube.loadObj("crazy.o");

		setupBuffers(player);
		setupBuffers(cube);

		void *p = (void*)(&cube.vertices);
		Vertices &vtemp = (*((Vertices *)p));
		for (int i = 0; i < cube.vertices.size(); i++) {
			vtemp[i].localPosition = vtemp[i].position;
			VertexGroup vg;
			vg.vertex = &(vtemp[i]);
			vg.intencity = 1.0f;
			ctrl.vertices.push_back(vg);
		}
		ctrl.rotation.RotateA(-90, Vector3(1.0,0.0,0.0));

		Uniform u("m_transform");
		Attribute a1("position"),
				  a2("color"),
				  a3("normal");
		cube.addUniform(u);
		cube.addAttribute(a1);
		cube.addAttribute(a2);
		cube.addAttribute(a3);
		
		player.addUniform(u);
		player.addAttribute(a1);
		player.addAttribute(a2);
		player.addAttribute(a3);
		return cube.loadShaders(vertexShaderFileName, fragmentShaderFileName) * player.loadShaders(vertexShaderFileName, fragmentShaderFileName);
	}catch(int i){
		return 0;
	}
}
Matrix4 model = Matrix4().Translate(Vector3(0.0, 0.0, 0.0));
void idle() {
	keyboardEvent();
	int time_ = glutGet(GLUT_ELAPSED_TIME);
	float angle = time_ / 1000.0 * 10;
	
	ctrl.emptyPosition();
	ctrl.refresh(true);
	//void *p = (void*)(&cube.vertices);
	//(*((Vertices*)p))[0].position.x = -((int)(angle*100) % 200)/100.0f;
	//(*((Vertices*)p))[0].position.y = 0.0f;
	//(*((Vertices*)p))[0].position.z = 0.0f;
	cube.updateElement();

	Vector3 pos = player.getPosition();
	float angX = player.getRotationX(),
		  angY = player.getRotationY();

	Matrix4 projection = Matrix4().Perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 50.0f),
			view = Matrix4().lookAt(pos, Vector3(pos.x-cos(angX), pos.y + sin(angY), pos.z + sin(angX)), Vector3(0.0, 1.0, 0.0));
	Matrix4 anim = Matrix4().RotateA(angle*3.0f, Vector3(1, 0, 0)) * 	// X axis
					Matrix4().RotateA(angle*2.0f, Vector3(0, 1, 0)) *	// Y axis
					Matrix4().RotateA(angle*4.0f, Vector3(0, 0, 1));	// Z axis


	Matrix4 m_transform = projection * view * model;// * anim;

	glUseProgram(cube.getProgram());

	glUniformMatrix4fv(cube.getUniform("m_transform"), 1, GL_FALSE, m_transform);

	glutPostRedisplay();
}
 
void onDisplay() {
	/* Clear the background as white */
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(cube.getProgram());

			cube.render();

	glUseProgram(0);

	/* Display the result */ 
	glutSwapBuffers();
}
 
void free_resources() {
}

void onReshape(int width, int height) {
  screen_width = width;
  screen_height = height;
  glViewport(0, 0, screen_width, screen_height);
}
template<class T>
void printMat(T m){
	for(int i = 0;i<4;i++){
		for(int j = 0;j<4;j++)
			printf("%f, ", m[i][j]);
		printf("\n");
	}
	printf("\n");

}

int m[2];
void onMouseMove(int x, int y){
	float speed = 0.002;
	player.rotateX((screen_width/2 - x)*speed);
	player.rotateY((screen_height/2 - y)*speed);
	m[0]++;
	if(m[0]%10)
		glutWarpPointer(screen_width/2, screen_height/2);
}

bool keys[256];
void keyboardEvent(){
	if(keys[GLUT_KEY_LEFT])
		player.moveLeft(0.1);
	if(keys[GLUT_KEY_RIGHT])
		player.moveLeft(-0.1);
	if(keys[GLUT_KEY_UP])
		player.moveForward(0.1);
	if(keys[GLUT_KEY_DOWN])
		player.moveForward(-0.1);
}

bool isFullscreen = true;
void onKeyDown(unsigned char Key, int x, int y){
	switch(Key){
		case 27:
			exit(1);
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
	}
	
}
void onKeyDownSpecial(int Key, int x, int y){
	keys[Key] = true;
}
void onKeyUpSpecial(int Key, int x, int y){
	keys[Key] = false;
}

int main(int argc, char* argv[]) {
	/* Glut-related initialising functions */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(screen_width, screen_height);
	glutCreateWindow("My First Game");

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
		glutIdleFunc(idle);
		glutReshapeFunc(onReshape);
		glutDisplayFunc(onDisplay);

		glutPassiveMotionFunc(onMouseMove);
		glutMotionFunc(onMouseMove);

		glutKeyboardFunc(onKeyDown);
		glutSpecialFunc(onKeyDownSpecial);
		glutSpecialUpFunc(onKeyUpSpecial);

		glutMainLoop();
	}

	/* If the program exits in the usual way,
	free resources and exit with a success */
	free_resources();
	return EXIT_SUCCESS;
}
