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

GLuint texture_id, vbo_cube_texcoords;
GLint uniform_mytexture;


GLint screen_width = 600, screen_height = 600;


GLfloat cube_texcoords[2*4*6] = {
	// front
	0.0, 0.0,
	1.0, 0.0,
	1.0, 1.0,
	0.0, 1.0,
};


std::string vertexShaderFileName = "triangle.v.glsl";
std::string fragmentShaderFileName = "triangle.f.glsl";

#include "setup.h"
#include "Data.h"

GLobject cube;
Bone ctrl;
int init_resources(void) {
	try{
		cube.loadObj("crazy.o");

		void *p = (void*)(&cube.vertices);
		Vertices &vtemp = (*((Vertices *)p));
		for (int i = 0; i < cube.vertices.size(); i++) {
			vtemp[i].localPosition = vtemp[i].position;
			VertexGroup vg;
			vg.vertex = &(vtemp[i]);
			vg.intencity = 1.0f;
			ctrl.vertices.push_back(vg);
		}

		setupBuffers(cube);
		Uniform u("m_transform");
		Attribute a1("position"),
				  a2("texcoord"), 
				  a3("color"),
				  a4("normal");
		cube.addUniform(u);
		cube.addAttribute(a1);
		cube.addAttribute(a2);
		cube.addAttribute(a3);
		cube.addAttribute(a4);
		return cube.loadShaders(vertexShaderFileName, fragmentShaderFileName);
	}catch(int i){
		return 0;
	}
}
Matrix4 model = Matrix4().Translate(Vector3(0.0, 0.0, -4.0)),
			view = Matrix4().lookAt(Vector3(0.0, 6.0, 3.0), Vector3(0.0, 0.0, -4.0), Vector3(0.0, 1.0, 0.0));
void idle() {
	int time_ = glutGet(GLUT_ELAPSED_TIME);
	float angle = time_ / 1000.0 * 10;
	
	ctrl.emptyPosition();
	ctrl.refresh(true);
	//void *p = (void*)(&cube.vertices);
	//(*((Vertices*)p))[0].position.x = -((int)(angle*100) % 200)/100.0f;
	//(*((Vertices*)p))[0].position.y = 0.0f;
	//(*((Vertices*)p))[0].position.z = 0.0f;
	cube.updateElement();

	Matrix4 projection = Matrix4().Perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 50.0f);
	Matrix4 anim = Matrix4().RotateA(angle*3.0f, Vector3(1, 0, 0)) * 	// X axis
					Matrix4().RotateA(angle*2.0f, Vector3(0, 1, 0)) *	// Y axis
					Matrix4().RotateA(angle*4.0f, Vector3(0, 0, 1));	// Z axis


	Matrix4 m_transform = projection * view * model * anim;
	//m_transform.Scale(Vector3(1.4,1.4,1.4));

	glUseProgram(cube.getProgram());

	glUniformMatrix4fv(cube.getUniform(0), 1, GL_FALSE, m_transform.getMatrix4());

	glutPostRedisplay();
}
 
void onDisplay() {
	/* Clear the background as white */
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(cube.getProgram());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glUniform1i(uniform_mytexture, 0);
		
		glEnableVertexAttribArray(cube.getAttribute(1));

			glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_texcoords);
			glVertexAttribPointer(
				cube.getAttribute(1), // attribute/id
				2,                  // number of elements per vertex
				GL_FLOAT,           // the type of each element/enumeration-type
				GL_FALSE,           // noralized?
				0,                  // extra data between each position
				0                   // offset to element
			);
			cube.render();

		glDisableVertexAttribArray(cube.getAttribute(1));
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(0);

	/* Display the result */ 
	glutSwapBuffers();
}
 
void free_resources() {
	glDeleteTextures(1, &texture_id);
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

int main(int argc, char* argv[]) {
	/*Vertex t[2];
	t[0].position.x = 1;
	t[0].position.y = 2;
	t[0].position.z = 3;
	t[1].position.x = 0;
	t[1].position.y = 9;
	t[1].position.z = 10;
	printf("tad: %p, yad: %p, y-1ad: %p, t+1ad: %p\n", t, &(t[0].position.y), &(t[0].position.y)-1 , ((float*)(t) + 0x1));
	printf("XVal: %f, YVal: %f, ZVal: %f\n", *((float *)(t)), *(((float*)t + 1)), *((float*)t + 2));
	printf("XVal2: %f, YVal2: %f, ZVal2: %f\n", *((float *)(t+1)), *(((float*)(t+1) + 1)), *((float*)(t+1) + 2));
	//printf("HH: %i, %i\n", sizeof(double)*3, sizeof(Vector3) - sizeof(double)*3);
	//printf("HH: %i, %i, %i\n", sizeof(Vertex), sizeof(Vector3), sizeof(Vector3)*2 + sizeof(GLfloat)*4);
	return 0;*/

	/*Matrix4 model = Matrix4().Translate(Vector3(0.0, 0.0, -4.0)),
			view = Matrix4().lookAt(Vector3(0.0, 2.0, 0.0), Vector3(0.0, 0.0, -4.0), Vector3(0.0, 1.0, 0.0)),
			projection = Matrix4().Perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 10.0f);

	Matrix4 m_transform;

	printMat<Matrix4>(view);

	m_transform = model * projection;
	m_transform2 = projection2 * model2;

	return 0;*/
	/* Glut-related initialising functions */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(screen_width, screen_height);
	glutCreateWindow("My First Object");

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
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glutIdleFunc(idle);
		glutKeyboardFunc(onKeyDown);
		glutReshapeFunc(onReshape);
		glutDisplayFunc(onDisplay);

		glutMainLoop();
	}

	/* If the program exits in the usual way,
	free resources and exit with a success */
	free_resources();
	return EXIT_SUCCESS;
}
