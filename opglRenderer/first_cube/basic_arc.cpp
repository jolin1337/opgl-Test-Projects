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

GLuint program = 0;

GLuint ibo_cube_elements,
	   vbo_cube_vertices, vbo_cube_colors,
	   uniform_fade;

GLuint texture_id, vbo_cube_texcoords;
GLint uniform_mytexture;

GLint uniform_m_transform;

GLint attribute_coord3d, attribute_v_color, attribute_texcoord;

GLint screen_width = 600, screen_height = 600;


GLfloat cube_vertices[] = {
	// front
	-1.0, -1.0,  1.0,
	 1.0, -1.0,  1.0,
	 1.0,  1.0,  1.0,
	-1.0,  1.0,  1.0,
	// top
	-1.0,  1.0,  1.0,
	 1.0,  1.0,  1.0,
	 1.0,  1.0, -1.0,
	-1.0,  1.0, -1.0,
	// back
	 1.0, -1.0, -1.0,
	-1.0, -1.0, -1.0,
	-1.0,  1.0, -1.0,
	 1.0,  1.0, -1.0,
	// bottom
	-1.0, -1.0, -1.0,
	 1.0, -1.0, -1.0,
	 1.0, -1.0,  1.0,
	-1.0, -1.0,  1.0,
	// left
	-1.0, -1.0, -1.0,
	-1.0, -1.0,  1.0,
	-1.0,  1.0,  1.0,
	-1.0,  1.0, -1.0,
	// right
	 1.0, -1.0,  1.0,
	 1.0, -1.0, -1.0,
	 1.0,  1.0, -1.0,
	 1.0,  1.0,  1.0,
};
GLfloat cube_colors[] = {
	// front colors
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0,
	1.0, 0.0, 1.0,
	// back colors
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0,
	1.0, 1.0, 0.0,
};
GLushort cube_elements[] = {
	// front
	 0,  1,  2,
	 2,  3,  0,
	// top
	 4,  5,  6,
	 6,  7,  4,
	// back
	 8,  9, 10,
	10, 11,  8,
	// bottom
	12, 13, 14,
	14, 15, 12,
	// left
	16, 17, 18,
	18, 19, 16,
	// right
	20, 21, 22,
	22, 23, 20,
};
GLfloat cube_texcoords[2*4*6] = {
	// front
	0.0, 0.0,
	1.0, 0.0,
	1.0, 1.0,
	0.0, 1.0,
};

#include <fstream>
#include <vector>
#include <fstream>
#include <vector>
class GL_Object {
public:
	GL_Object():size(0), f_size(0), _allokatedMem(false){}
	~GL_Object(){
		glDeleteBuffers(1, &vbo_vertices);
		glDeleteBuffers(1, &ibo_indicies);
		if(_allokatedMem){
			delete[] vertices;
			delete[] faces;
			_allokatedMem = false;
		}
	}

	void clean(){
		glDeleteBuffers(1, &vbo_vertices);
		glDeleteBuffers(1, &ibo_indicies);
		if(_allokatedMem){
			delete[] vertices;
			delete[] faces;
			_allokatedMem = false;
		}	
	}
	GL_Object loadObj(std::string fileName){
		if(_allokatedMem){
			delete[] vertices;
			delete[] faces;
			_allokatedMem = false;
		}
		std::ifstream ifs(fileName.c_str());
		if(!ifs.is_open())
			throw 401;
		std::string line;
		size = f_size = 0;
		std::vector<GLfloat> v;
		std::vector<GLushort> f;

		while(!ifs.eof()){
			char c = ifs.get();
			GLfloat x;
			GLushort y;
			switch(c){
				case 'v':
				case 'V':
					ifs >> x;
					v.push_back(x);
					ifs >> x;
					v.push_back(x);
					ifs >> x;
					v.push_back(x);
					break;
				case 'f':
				case 'F':
					ifs >> y;
					f.push_back(y-1);
					ifs >> y;
					f.push_back(y-1);
					ifs >> y;
					f.push_back(y-1);
					break;
			}
			if(c != '\n')
				getline(ifs, line, '\n');
		}
		size = v.size()/3;
		f_size = f.size()/3;
		
		vertices = new GLfloat[v.size()];
		for(int i=0;i<v.size();i++)
			vertices[i] = v[i];

		faces = new GLushort[f.size()];
		for(int i=0;i<f.size();i++)
			faces[i] = f[i];
		_allokatedMem = true;
	}
	GLuint getVboVerticesArray() const {
		return vbo_vertices;
	}
	GLuint getVboTextureArray() const {
		return vbo_vertices;
	}
	GLuint getIboIndiciesArray() const {
		return ibo_indicies;
	}
	void render(){

	}
	friend void setupBuffers(GL_Object &);
	friend int init_resources(void);
private:
	bool _allokatedMem;
	GLfloat *vertices, *textureCoordinates;
	GLushort *faces;
	unsigned int size, f_size; 
	GLuint vbo_vertices, ibo_indicies, vbo_textureCoordinates;
};
class Bone{
public:
	std::vector<GLfloat *> vertices;

};

char * vertexShaderFileName = (char *)"triangle.v.glsl";
char * fragmentShaderFileName = (char *)"triangle.f.glsl";
#include "setup.h"


GL_Object cube;
Bone b;
int init_resources(void) {
	/*cube.vertices = cube_vertices;
	cube.faces = cube_elements;
	cube.textureCoordinates = cube_texcoords;
	cube.size = 24;
	cube.f_size = 12;*/
	try{
		cube.loadObj("cube.o");
		//b.vertices.push_back(&(cube.vertices[0]));
		//b.vertices.push_back(&(cube.vertices[1]));
		//printf("%i\n", b.vertices.size());
	/*for(int i=b.vertices.size(); i> 0; i--){
		*(b.vertices[i]) = 0.0f;
	}*/
	}catch(int i){
		return 0;
	}
	setupBuffers(cube);
	return setupShaders();
}
void idle() {
	float cur_fade = sinf(glutGet(GLUT_ELAPSED_TIME) / 1000.0 * (2*3.14) / 5) / 2 + 0.5; // 0->1->0 every 5 seconds
	int time_ = glutGet(GLUT_ELAPSED_TIME);
	float angle = time_ / 1000.0 * 15;  // 45° per second
	
	Matrix4 model = Matrix4().Translate(Vector3(0.0, 0.0, -4.0)),
			view = Matrix4().lookAt(Vector3(0.0, 2.0, 0.0), Vector3(0.0, 0.0, -4.0), Vector3(0.0, 1.0, 0.0)),
			projection = Matrix4().Perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 10.0f);
	Vector3 axis_y(0.0, 1.0, 0.0);
	//Matrix4 anim = Matrix4().RotateA(angle, axis_y);
	Matrix4 anim = Matrix4().RotateA(angle*3.0f, Vector3(1, 0, 0)) * 	// X axis
					Matrix4().RotateA(angle*2.0f, Vector3(0, 1, 0)) *	// Y axis
					Matrix4().RotateA(angle*4.0f, Vector3(0, 0, 1));		// Z axis


	Matrix4 m_transform = projection * view * model * anim;
	

	glUseProgram(program);

	glUniformMatrix4fv(uniform_m_transform, 1, GL_FALSE, m_transform.getMatrix4());

	glutPostRedisplay();
}
 
void onDisplay() {
	/* Clear the background as white */
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	if(program){

		glUseProgram(program);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glUniform1i(uniform_mytexture, /*GL_TEXTURE*/0);
		
		glEnableVertexAttribArray(attribute_texcoord);
		glEnableVertexAttribArray(attribute_coord3d);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_texcoords);
		glVertexAttribPointer(
			attribute_texcoord, // attribute
			2,                  // number of elements per vertex, here (x,y)
			GL_FLOAT,           // the type of each element
			GL_FALSE,           // take our values as-is
			0,                  // no extra data between each position
			0                   // offset of first element
		);
		glBindBuffer(GL_ARRAY_BUFFER, cube.getVboVerticesArray());
		// Describe our vertices array to OpenGL (it can't guess its format automatically) 
		glVertexAttribPointer(
			attribute_coord3d,  // attribute
			3,                  // number of elements per vertex, here (x,y,z)
			GL_FLOAT,           // the type of each element
			GL_FALSE,           // take our values as-is
			0, 					// no extra data between each position
			0                   // pointer to the C array
		);


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube.getIboIndiciesArray());
		int size;
		glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
		glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

		glDisableVertexAttribArray(attribute_coord3d);
		glDisableVertexAttribArray(attribute_v_color);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}
	/* Display the result */ 
	glutSwapBuffers();
}
 
void free_resources() {
	if(program != 0)
		glDeleteProgram(program);
	glDeleteTextures(1, &texture_id);
	glDeleteBuffers(1, &vbo_cube_vertices);
	glDeleteBuffers(1, &ibo_cube_elements);
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
int main(int argc, char* argv[]) {
	/* Glut-related initialising functions */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(screen_width, screen_height);
	glutCreateWindow("My First Cube =>");

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
		/* We can display it if everything goes OK */
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glutIdleFunc(idle);
		glutReshapeFunc(onReshape);
		glutDisplayFunc(onDisplay);

		glutMainLoop();
	}

	/* If the program exits in the usual way,
	free resources and exit with a success */
	free_resources();
	return EXIT_SUCCESS;
}
