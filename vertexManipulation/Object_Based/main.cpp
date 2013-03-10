/* Using the standard output for fprintf */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
/* Use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* Using the GLUT library for the base windowing setup */
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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


GLfloat cube_texcoords[2*4*6] = {
	// front
	0.0, 0.0,
	1.0, 0.0,
	1.0, 1.0,
	0.0, 1.0,
};
#include <fstream>
#include <vector>
class Bone{
public:
	std::vector<GLfloat *> vertices;

};

#include "./utils/Vector3.h"

struct ShaderInfo {
	std::string name;
	GLint id;
	operator GLint(){
		return id;
	}
};
struct Uniform : public ShaderInfo {
	Uniform(){}
	int setUp(GLuint prog){
		if(name == "")return 0;
		id = glGetUniformLocation(prog, name.c_str());
		if (id == -1) {
			fprintf(stderr, "Could not bind uniform %s\n", name.c_str());
			return 0;
		}
		printf("Loaded uniform %s, with id: %i\n", name.c_str(), id);	
		return 1;
	}

};
struct Attribute : public ShaderInfo {
	Attribute(){}
	int setUp(GLuint prog){
		if(name == "")return 0;
		id = glGetAttribLocation(prog, name.c_str());
		if (id== -1) {
			fprintf(stderr, "Could not bind attribute %s\n", name.c_str());
			return 0;
		}
		printf("Loaded attribute %s, with id: %i\n", name.c_str(), id);
		return 1;
	}
};

std::string vertexShaderFileName = "triangle.v.glsl";
std::string fragmentShaderFileName = "triangle.f.glsl";

int setupR(GLuint &prog){
	GLint link_ok = GL_FALSE;
	GLuint vs, fs;
	if ((vs = create_shader(vertexShaderFileName.c_str(), GL_VERTEX_SHADER))   == 0) return 0;
	if ((fs = create_shader(fragmentShaderFileName.c_str(), GL_FRAGMENT_SHADER)) == 0) return 0;

	prog = glCreateProgram();
	
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);

	glLinkProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &link_ok);
	if (!link_ok) {
		fprintf(stderr, "glLinkProgram:\n");
		return 0;
	}

	//if(setupUniforms() == 0 || setUpAttributes() == 0)
	//	return 0;

	return 1;
}

/**
	IMPORTANT!: do not change order or add some variables to Vertex struct, 
		if you do the render wont work as expected! ie: glVertexAttribPointer 
		will calculate the wrong vercies
*/
struct Vertex {
	Vector3 position, localPosition, worldPosition,
			normal;
	GLfloat color[4];
};
struct Face {
	GLushort indicies[3];
	Vertex *vertices[3], normal;
};
typedef std::vector<Vertex> Vertices;
typedef std::vector<GLushort> Faces;

typedef std::vector<Uniform> Uniforms;
typedef std::vector<Attribute> Attributes;

class GL_Object {
public:
	friend void setupBuffers(GL_Object &obj);
	friend void idle();
	GL_Object():_allokatedMem(false), linkedShaders(false), prog(0){}
	~GL_Object(){
		glDeleteProgram(prog);
	}
	void loadObj(std::string fileName){
		if(_allokatedMem){
			_allokatedMem = false;
		}
		std::ifstream ifs(fileName.c_str());
		if(!ifs.is_open())
			throw 401;
		std::string line;
		
		while(!ifs.eof()){
			char c = ifs.get();
			Vertex v;
			GLfloat v2;
			Face f;
			GLushort f2;
			switch(c){
				case 'v':
				case 'V':
					ifs >> v.position.x;
					ifs >> v.position.y;
					ifs >> v.position.z;
					vertices.push_back(v);/*
					ifs >> v2;
					vertices.push_back(v2);
					ifs >> v2;
					vertices.push_back(v2);
					ifs >> v2;
					vertices.push_back(v2);*/
					break;
				case 'f':
				case 'F':/*
					ifs >> f.indicies[0];
					ifs >> f.indicies[1];
					ifs >> f.indicies[2];
					if(vertices.size() > f.indicies[0]-1 && f.indicies[0] > 0){
						f.indicies[0] -= 1;
						f.vertices[0] = &(vertices[f.indicies[0]]);
					}
					if(vertices.size() > f.indicies[1]-1 && f.indicies[1] > 0){
						f.indicies[1] -= 1;
						f.vertices[1] = &(vertices[f.indicies[1]]);
					}
					if(vertices.size() > f.indicies[2]-1 && f.indicies[2] > 0){
						f.indicies[2] -= 1;
						f.vertices[2] = &(vertices[f.indicies[2]]);
					}
					faces.push_back(f);*/
					ifs >> f2;
					faces.push_back(f2-1);
					ifs >> f2;
					faces.push_back(f2-1);
					ifs >> f2;
					faces.push_back(f2-1);
					break;
			}
			if(c != '\n')
				getline(ifs, line, '\n');
		}
		unsigned int vsize = vertices.size(), fsize = faces.size();
		if(vsize == 0 || fsize == 0)
			throw 501;
		_allokatedMem = true;
	}
	int setupUniforms(){
		if(!linkedShaders)return 0;
		int res = 1;
		for(Uniforms::iterator it = uniforms.begin(); it != uniforms.end(); it++)
			if((*it).setUp(prog) == 0)
				res = 0;
		return res;
	}
	int setupAttributes(){
		if(!linkedShaders)return 0;
		int res = 1;
		for(Attributes::iterator it = attributes.begin(); it != attributes.end(); it++)
			if((*it).setUp(prog) == 0)
				res = 0;
		return res;
	}
	int loadShaders(std::string fileName_v, std::string fileName_f){
		GLint link_ok = GL_FALSE;
		GLuint vs, fs;
		if ((vs = create_shader(fileName_v.c_str(), GL_VERTEX_SHADER))   == 0) return 0;
		if ((fs = create_shader(fileName_f.c_str(), GL_FRAGMENT_SHADER)) == 0) return 0;

		prog = glCreateProgram();

		glAttachShader(prog, vs);
		glAttachShader(prog, fs);

		glLinkProgram(prog);
		glGetProgramiv(prog, GL_LINK_STATUS, &link_ok);
		if (!link_ok) {
			linkedShaders = false;
			fprintf(stderr, "glLinkProgram:\n");
			return 0;
		}
		linkedShaders = true;

		if(this->setupUniforms() + this->setupAttributes() == 0)
			return 0;

		return 1;
	}
	void render(){
		glEnableVertexAttribArray(getAttribute(0));
			glBindBuffer(GL_ARRAY_BUFFER, getVboVerticesArray());
			// Describe our vertices array to OpenGL (it can't guess its format automatically) 
			int size;
			glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
			glVertexAttribPointer(
				getAttribute(0), 
				3, 
				GL_FLOAT, 
				GL_FALSE, 
				sizeof(Vertex),//sizeof(Vector3)*2 + sizeof(float)*4,
				0
			);

			
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, getIboIndiciesArray());
			glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
			glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

		glDisableVertexAttribArray(getAttribute(0));

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
	bool isModel(){
		return _allokatedMem;
	}
	void addAttribute(Attribute a){
		attributes.push_back(a);
	}
	Attribute getAttribute(GLuint a) const{
		if(a < attributes.size())
			return attributes[a];
	}
	Uniform getUniform(GLuint u) const{
		if(u < uniforms.size())
			return uniforms[u];
	}
	void addUniform(Uniform u){
		uniforms.push_back(u);
	}
	GLuint getProgram() const{
		return prog;
	}

private:
	Vertices vertices;
	Faces faces;
	GLuint vbo_vertices, ibo_indicies, vbo_textureCoordinates;
	
	Uniforms uniforms;
	Attributes attributes;
	GLuint prog;

	bool _allokatedMem,
		 linkedShaders;
};


#include "setup.h"


GL_Object cube;
int init_resources(void) {
	/*cube.vertices = cube_vertices;
	cube.faces = cube_elements;
	cube.textureCoordinates = cube_texcoords;
	cube.size = 24;
	cube.f_size = 12;*/
	try{
		cube.loadObj("obj.o");
	}catch(int i){
		return 0;
	}
	setupBuffers(cube);
	Uniform u;
	u.name = "m_transform";
	Attribute a1, a2;
	a1.name = "coord3d";
	a2.name = "texcoord";
	u.id = uniform_m_transform;
	a1.id = attribute_coord3d;
	a2.id = attribute_texcoord;
	cube.addUniform(u);
	cube.addAttribute(a1);
	cube.addAttribute(a2);
	return cube.loadShaders(vertexShaderFileName, fragmentShaderFileName);
}
Matrix4 model = Matrix4().Translate(Vector3(0.0, 0.0, -4.0)),
			view = Matrix4().lookAt(Vector3(0.0, 2.0, 0.0), Vector3(0.0, 0.0, -4.0), Vector3(0.0, 1.0, 0.0));
void idle() {
	float cur_fade = sinf(glutGet(GLUT_ELAPSED_TIME) / 1000.0 * (2*3.14) / 5) / 2 + 0.5; // 0->1->0 every 5 seconds
	int time_ = glutGet(GLUT_ELAPSED_TIME);
	float angle = time_ / 1000.0 * 15;  // 45° per second
	
	void *p = (void*)(&cube.vertices);
	(*((Vertices*)p))[0].position.x = 0;

	Matrix4 projection = Matrix4().Perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 10.0f);
	Matrix4 anim = Matrix4().RotateA(angle*3.0f, Vector3(1, 0, 0)) * 	// X axis
					Matrix4().RotateA(angle*2.0f, Vector3(0, 1, 0)) *	// Y axis
					Matrix4().RotateA(angle*4.0f, Vector3(0, 0, 1));		// Z axis


	Matrix4 m_transform = projection * view * model * anim;
	//m_transform.Scale(Vector3(0.4,0.4,0.4));

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

	glm::mat4 model2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
	glm::mat4 view2 = glm::lookAt(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 projection2 = glm::perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 10.0f);
	glm::mat4 m_transform2;
	
	printMat<Matrix4>(view);
	printMat<glm::mat4>(view2);

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