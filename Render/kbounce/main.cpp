/* Using the standard output for fprintf */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <string>
/* Use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* Using the GLUT library for the base windowing setup */
#include <GL/glut.h>

#include "./tex/hatt_texture.c"
#include "utils/Matrix4.h"
#include "config.h"

// redefine and comment these, please!!!
#include "setup.h"
#include "Data.h"
#include "Character.h"
#include "preformance.h"
/* ADD GLOBAL VARIABLES HERE */

// Event variables:
	bool keys[256],
		 jumping = false;
	bool isFullscreen = true;


// Screen info
	std::string title = "KBounce 3D";
	GLint screen_width = 600, screen_height = 600;

// Shaderfiles
	std::string vertexShaderFileName = "shader.v.glsl";
	std::string fragmentShaderFileName = "shader.f.glsl";

// Object declaration;
	// CubeBoundary:
		GLfloat BOUNDARY = 11.0;
	const GLuint BALLS = 3, DETAIL_GRID = 11;
	Character player(0.0f,0.0f,0.0f);
	WorldObject cube, cube2;
	WorldObject balls[BALLS];
	WorldObject grid[DETAIL_GRID*3];
	Vector3 cvelocity(0.0f,0.0f,0.0f);

// Physics:
	Vector3 gravity(0.0f, 9.82f, 0.0f);
	GLfloat mass = 4;


void keyboardEvent();


int init_resources(void) {
	try{
		player.moveLeft(9);
		player.moveForward(7);
		player.rotateX(-M_PI*3/4);
		player.rotateY(M_PI/12);
		player.addForce(gravity*-1);

		player.loadObj("crazy.o");
		cube.loadObj("cube.o");
		cube2.loadObj("cube.o");

		setupBuffers(player);
		setupBuffers(cube);
		setupBuffers(cube2);

		addObjectToShader(&cube);
		addObjectToShader(&cube2);
		bool c1 = cube.loadShaders(vertexShaderFileName, fragmentShaderFileName),
			 c2 = cube2.loadShaders(vertexShaderFileName, fragmentShaderFileName),
			 c3 = 1;

		for(int i=0; i< BALLS; i++){
			balls[i].loadObj("crazy.o");
			balls[i].setScale(Vector3(0.25f,0.25f,0.25f));
			setupBuffers(balls[i]);
			addObjectToShader(&(balls[i]));
			c3 *= balls[i].loadShaders(vertexShaderFileName, fragmentShaderFileName);

			balls[i].addVelocity(Vector3(m_rand()-0.5f, m_rand()-0.5f, m_rand()-0.5f).normalize().mult(60.0));
		}
		GLfloat scale_grid = ((GLfloat)DETAIL_GRID*2.0f)/((GLfloat)BOUNDARY);
		for(int i=0; i< DETAIL_GRID; i++){
			for(int j = 0;j < DETAIL_GRID; j++){
				grid[i].loadObj("cube.o");

				grid[i].setScale(Vector3(scale_grid, scale_grid, scale_grid));
				setupBuffers(grid[i]);
				addObjectToShader(&(grid[i]));
				c3 *= grid[i].loadShaders(vertexShaderFileName, fragmentShaderFileName);


				grid[i].setPosition(Vector3(i*scale_grid, j*scale_grid, k*scale_grid - DETAIL_GRID-2.0f));
				/*if(i%6 == 0)
					grid[i].setPosition(Vector3(DETAIL_GRID + 1.0f, (i-3)*scale_grid-DETAIL_GRID, (i-3)*scale_grid-DETAIL_GRID));
				else if(i%6 == 1)
					grid[i].setPosition(Vector3((i-2)*scale_grid-DETAIL_GRID, DETAIL_GRID + 1.0f, (i-2)*scale_grid-DETAIL_GRID));
				else if(i%6 == 2)
					grid[i].setPosition(Vector3((i-1)*scale_grid-DETAIL_GRID, (i-1)*scale_grid-DETAIL_GRID, DETAIL_GRID + 1.0f));
				else if(i%6 == 3)
					grid[i].setPosition(Vector3(-DETAIL_GRID - 1.0f , i*scale_grid-DETAIL_GRID, i*scale_grid-DETAIL_GRID));
				else if(i%6 == 4)
					grid[i].setPosition(Vector3((i+1)*scale_grid-DETAIL_GRID, -DETAIL_GRID - 1.0f, (i+1)*scale_grid-DETAIL_GRID));
				else if(i%6 == 5)
					grid[i].setPosition(Vector3((i+2)*scale_grid-DETAIL_GRID, (i+2)*scale_grid-DETAIL_GRID, -DETAIL_GRID - 1.0f));*/
			}
		}
		Vector3 pos = cube2.getPosition()+ Vector3(0.5f, 3.5f, 1.5f);
		cube2.setPosition(pos);
		//cube2.setRotation(Vector3(30*M_PI/180, 0, 40*M_PI/180));
		return c1 * c2 * c3;
	}catch(int i){
		return 0;
	}
}


void idle() {
	keyboardEvent();
	int time_ = glutGet(GLUT_ELAPSED_TIME);
	float angle = time_ / 1000.0 * 10;

	Vector3 pos = player.getPosition();
	float angX = player.getRotationX(),
		  angY = player.getRotationY();

	Vector3 cpos = cube.getPosition(),
			cpos2= cube2.getPosition();

	GLfloat D = cpos.distance(&cpos2),
			W = (cube.dimension() + cube2.dimension())/2;
	float bound = 6.0;
	//if(time_%100 < 10)
	//	gravity = Vector3(rand()%10-5, rand()%10-5, rand()%10-5);
	cvelocity.add(&gravity);
	Vector3 s = cvelocity;
	s.mult(-mass/1000).add(&cpos2);
	bool collides = true;
	if(cpos2.x < -bound && cvelocity.x > 0.0f){
		cvelocity.x = -abs(cvelocity.x)*0.5;
		gravity.x = -abs(gravity.x);
	}
	else if(cpos2.x > bound && cvelocity.x < 0.0f){
		cvelocity.x = abs(cvelocity.x)*0.5;
		gravity.x = abs(gravity.x);
	}
	else if(cpos2.y < -bound && cvelocity.y > 0.0f){
		cvelocity.y = -abs(cvelocity.y)*0.5;
		gravity.y = -abs(gravity.y);
	}
	else if(cpos2.y > bound && cvelocity.y < 0.0f){
		cvelocity.y = abs(cvelocity.y)*0.5;
		gravity.y = abs(gravity.y);
	}
	else if(cpos2.z < -bound && cvelocity.z > 0.0f){
		cvelocity.z = -abs(cvelocity.z)*0.5;
		gravity.z = -abs(gravity.z);
	}
	else if(cpos2.z > bound && cvelocity.z < 0.0f){
		cvelocity.z = abs(cvelocity.z)*0.5;
		gravity.z = abs(gravity.z);
	}
	else{
		collides = false;
	}
	
	if(!collides){
		if(cpos.x + cube.min.x <= cpos2.x + cube2.max.x && cpos.x + cube.max.x >= cpos2.x + cube2.min.x && 
			cpos.z + cube.min.z <= cpos2.z + cube2.max.z && cpos.z + cube.max.z >= cpos2.z + cube2.min.z && 
			cpos.y + cube.min.y <= cpos2.y + cube2.max.y && cpos.y + cube.max.y >= cpos2.y + cube2.min.y){
			collides = true;
			cvelocity.x = cvelocity.y = cvelocity.z = 0.0f;
		}
		
		if(!collides)
			cube2.setPosition(s);
	}

	player.updateForce();
	for (int i = 0; i < BALLS; i++){
		balls[i].updateForce();
		Vector3 ball_pos = balls[i].getPosition(),
				ball_vel = balls[i].dynamics.velocity;
		//printf("x: %f, y: %f, z %f: %f\n", ball_pos.x, ball_pos.y, ball_pos.z, BOUNDARY);
		if(ball_pos.x < -BOUNDARY || ball_pos.x > BOUNDARY)
			balls[i].addVelocity(Vector3(-2 * ball_vel.x, 0.0f, 0.0f));
		if(ball_pos.y < -BOUNDARY || ball_pos.y > BOUNDARY)
			balls[i].addVelocity(Vector3(0.0f, -2 * ball_vel.y, 0.0f));
		if(ball_pos.z < -BOUNDARY || ball_pos.z > BOUNDARY)
			balls[i].addVelocity(Vector3(0.0f, 0.0f, -2 * ball_vel.z));
	}

	Matrix4 model = cube.getTransformMatrix(),
			model2 = cube2.getTransformMatrix();
	Matrix4 projection = Matrix4().Perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 50.0f),
			view = Matrix4().lookAt(pos, Vector3(pos.x-cos(angX), pos.y + sin(angY), pos.z + sin(angX)), Vector3(0.0f, 1.0f, 0.0f));
	Matrix4 anim = Matrix4().RotateA(angle*3.0f, Vector3(1, 0, 0)) * 	// X axis
					Matrix4().RotateA(angle*2.0f, Vector3(0, 1, 0)) *	// Y axis
					Matrix4().RotateA(angle*4.0f, Vector3(0, 0, 1));	// Z axis


	Matrix4 m_transform = projection * view;

	glUseProgram(cube.getProgram());
	glUniformMatrix4fv(cube.getUniform("m_transform"), 1, GL_FALSE, m_transform * model);

	glUseProgram(cube2.getProgram());
	glUniformMatrix4fv(cube2.getUniform("m_transform"), 1, GL_FALSE, m_transform * model2);

	for (int i = 0; i < BALLS; i++){
		glUseProgram(balls[i].getProgram());
		glUniformMatrix4fv(
			balls[i].getUniform("m_transform"), 	// m_transform is the transformation of balls[i] in shaders
			1, 										// one uniform-buffer 
			GL_FALSE, 								// no nprmalization
			m_transform * balls[i].getTransformMatrix()
		);
	}
	for (int i = 0; i < DETAIL_GRID*3; i++){
		grid[i].updateForce();
		glUseProgram(grid[i].getProgram());
		glUniformMatrix4fv(
			grid[i].getUniform("m_transform"), 	// m_transform is the transformation of grid[i] in shaders
			1, 										// one uniform-buffer 
			GL_FALSE, 								// no nprmalization
			m_transform * grid[i].getTransformMatrix()
		);
	}

	glutPostRedisplay();
}
 
void onDisplay() {
	if(showPreformance)
		fps++;
	/* Clear the background as white */
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glutSolidCube(1.0);
	glUseProgram(cube.getProgram());
		cube.render();
	glUseProgram(cube2.getProgram());
		cube2.render();
		glTranslatef(0.4f,0.4f,0.4f);
		cube2.render();

	for (int i = 0; i < BALLS; i++) {
		glUseProgram(balls[i].getProgram());
			balls[i].render();
	}
	for (int i = 0; i < DETAIL_GRID*3; i++) {
		glUseProgram(grid[i].getProgram());
			grid[i].render();
	}

	glUseProgram(0);
	PreformanceIRL(1);
	/* Display the result */ 
	glutSwapBuffers();
}
 
void free_resources() {
}

void onReshape(int width, int height) {
  screen_width = width;
  screen_height = height;
  glViewport(0, 0, screen_width, screen_height);
  glutWarpPointer(screen_width/2, screen_height/2);
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

void onMouseMove(int x, int y){
	static int m = 0;
	m++;
	if(m == 1) return;
	float speed = 0.002;
	player.rotateX((screen_width/2 - x)*speed);
	player.rotateY((screen_height/2 - y)*speed);
	if(m%100)
		glutWarpPointer(screen_width/2, screen_height/2);
}

void keyboardEvent(){
	if(!jumping){
		if(keys[GLUT_KEY_LEFT])
			player.moveLeft(0.1);
		if(keys[GLUT_KEY_RIGHT])
			player.moveLeft(-0.1);
		if(keys[GLUT_KEY_UP])
			player.moveForward(0.1);
		if(keys[GLUT_KEY_DOWN])
			player.moveForward(-0.1);
	}
	if(player.getPosition().y < 0){
		//player.clearVelocity();
		player.clearForce();
		player.addForce(gravity*-1);
		jumping = false;
		player.setY(0);
	}
}

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
			break;
		case ' ':
			if(!jumping){
				jumping = true;
				player.clearVelocity();
				player.addVelocity(Vector3(0.0f, 323.0f, 0.0f));
			}
			break;
	}
	
}
void onKeyUp(unsigned char Key, int x, int y){
	switch(Key){
		case ' ':;
			//jumping = false;
			//player.addVelocity(Vector3(0.0f, -23.0f, 0.0f));
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
	int windowH = glutCreateWindow(title.c_str());
	//windowH = glutCreateWindow(WINDOW_TITLE_PREFIX);
 
	if(windowH < 1) {
		fprintf(
			stderr,
			"ERROR: Could not create a new rendering window.\n"
		);
		exit(EXIT_FAILURE);
	}


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
		glutKeyboardUpFunc(onKeyUp);
		glutSpecialFunc(onKeyDownSpecial);
		glutSpecialUpFunc(onKeyUpSpecial);
		
		glutTimerFunc(250, threadFunc, 4);
		glutMainLoop();
	}

	/* If the program exits in the usual way,
	free resources and exit with a success */
	free_resources();
	return EXIT_SUCCESS;
}
