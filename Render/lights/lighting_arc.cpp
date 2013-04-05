/* Using the standard output for fprintf */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/* Use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* Using the GLUT library for the base windowing setup */
#include <GL/glut.h>

#include "utils/Matrix4.h"
/* ADD GLOBAL VARIABLES HERE */

GLuint program = 0;

GLuint ibo_cube_elements,
	   vbo_cube_vertices, vbo_cube_colors, vbo_cube_normals,
	   uniform_fade;

GLuint texture_id, vbo_cube_texcoords;
GLint uniform_mytexture;

GLint uniform_m_transform, uniform_m_3x3_inv_transp;

GLint attribute_coord3d, attribute_v_color, attribute_texcoord, attribute_v_normal;

GLint screen_width = 600, screen_height = 600;

char *close_file(FILE *input){
	if(input != 0)
		fclose(input);
	return NULL;
}

char* file_read(const char* filename)
{
	FILE* input = fopen(filename, "rb");
	if(input == NULL) return close_file(input);

	if(fseek(input, 0, SEEK_END) == -1) return close_file(input);
	long size = ftell(input);
	if(size == -1 || fseek(input, 0, SEEK_SET) == -1) return close_file(input);

	/*if using c-compiler: dont cast malloc's return value*/
	char *content = (char*) malloc( (size_t) size +1  ); 
	if(content == NULL) return close_file(input);

	fread(content, 1, (size_t)size, input);
	if(ferror(input)) {
		free(content);
		return close_file(input);
	}

	fclose(input);
	content[size] = '\0';
	return content;
}
 
/**
 * Display compilation errors from the OpenGL shader compiler
 */
void print_log(GLuint object) {
	GLint log_length = 0;
	if (glIsShader(object))
		glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
	else if (glIsProgram(object))
		glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
	else {
		fprintf(stderr, "printlog: Not a shader or a program\n");
		return;
	}

	char* log = (char*)malloc(log_length);

	if (glIsShader(object))
		glGetShaderInfoLog(object, log_length, NULL, log);
	else if (glIsProgram(object))
		glGetProgramInfoLog(object, log_length, NULL, log);

	fprintf(stderr, "%s", log);
	free(log);
}

/**
 * Compile the shader from file 'filename', with error handling
 */
GLuint create_shader(const char* filename, GLenum type) {
	const GLchar* source = file_read(filename);
	if (source == NULL) {
		fprintf(stderr, "Error opening %s: ", filename); perror("");
		return 0;
	}
	GLuint res = glCreateShader(type);
  const GLchar* sources[] = {
	    // Define GLSL version
#ifdef GL_ES_VERSION_2_0
	    "#version 100\n"
#else
	    "#version 120\n"
#endif
	    ,
	    // GLES2 precision specifiers
#ifdef GL_ES_VERSION_2_0
	    // Define default float precision for fragment shaders:
	    (type == GL_FRAGMENT_SHADER) ?
	    "#ifdef GL_FRAGMENT_PRECISION_HIGH\n"
	    "precision highp float;           \n"
	    "#else                            \n"
	    "precision mediump float;         \n"
	    "#endif                           \n"
	    : ""
	    // Note: OpenGL ES automatically defines this:
	    // #define GL_ES
#else
	    // Ignore GLES 2 precision specifiers:
	    "#define lowp   \n"
	    "#define mediump\n"
	    "#define highp  \n"
#endif
	    ,
	    source };
	glShaderSource(res, 3, sources, NULL);
	free((void*)source);

	glCompileShader(res);
	GLint compile_ok = GL_FALSE;
	glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
	if (compile_ok == GL_FALSE) {
		fprintf(stderr, "%s:", filename);
		print_log(res);
		glDeleteShader(res);
		return 0;
	}

	return res;
}

int init_resources(void) {/*
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, // target
					0,  // level, 0 = base, no minimap,
					GL_RGB, // internalformat
					res_texture.width,  // width
					res_texture.height,  // height
					0,  // border, always 0 in OpenGL ES
					GL_RGB,  // format
					GL_UNSIGNED_BYTE, // type
					res_texture.pixel_data);*/

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
	glGenBuffers(1, &vbo_cube_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

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
	glGenBuffers(1, &vbo_cube_colors);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);

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

	glGenBuffers(1, &ibo_cube_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);

	GLfloat cube_texcoords[2*4*6] = {
		// front
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
	};
	//for (int i = 1; i < 6; i++)
	//	memcpy(&cube_texcoords[i*4*2], &cube_texcoords[0], 2*4*sizeof(GLfloat));
	glGenBuffers(1, &vbo_cube_texcoords);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_texcoords);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_texcoords), cube_texcoords, GL_STATIC_DRAW);

	GLfloat cube_normals[6*3*2] = {
		// front
		0.0, 0.0, 1.0,
		0.0, 0.0, -1.0,
		// top
		0.0, 1.0, 0.0,
		0.0, -1.0, 0.0,
		// back
		0.0, 0.0, -1.0,
		0.0, 0.0, 1.0,
		// bottom
		0.0, -1.0, 0.0,
		0.0, 1.0, 0.0,
		// left
		-1.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		// right
		1.0, 0.0, 0.0,
		-1.0, 0.0, 0.0,

	};
	glGenBuffers(1, &vbo_cube_normals);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_normals), cube_normals, GL_STATIC_DRAW);

	GLint link_ok = GL_FALSE;
	GLuint vsl, vs, fs;
	//if ((vs = create_shader("rotation.v.glsl", GL_VERTEX_SHADER)) == 0) return 0;
	if ((vsl = create_shader("light.v.glsl", GL_VERTEX_SHADER))   == 0) return 0;
	if ((fs = create_shader("light.f.glsl", GL_FRAGMENT_SHADER)) == 0) return 0;

	program = glCreateProgram();
	
	glAttachShader(program, vs);
	glAttachShader(program, vsl);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if (!link_ok) {
		fprintf(stderr, "glLinkProgram:");
		return 0;
	}

	const char* uniform_name, *attribute_name;
	uniform_name = "m_transform";
	uniform_m_transform = glGetUniformLocation(program, uniform_name);
	if (uniform_m_transform == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
		return 0;
	}
	uniform_name = "m_3x3_inv_transp";
	uniform_m_3x3_inv_transp = glGetUniformLocation(program, uniform_name);
	if (uniform_m_3x3_inv_transp == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
		return 0;
	}

	attribute_name = "coord3d";
	attribute_coord3d = glGetAttribLocation(program, attribute_name);
	if (attribute_coord3d == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return 0;
	}

	attribute_name = "v_normal";
	attribute_v_normal = glGetAttribLocation(program, attribute_name);
	if (attribute_v_normal == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return 0;
	}

	
	return 1;
}
void idle() {
	float cur_fade = sinf(glutGet(GLUT_ELAPSED_TIME) / 1000.0 * (2*3.14) / 5) / 2 + 0.5; // 0->1->0 every 5 seconds
	int time_ = glutGet(GLUT_ELAPSED_TIME);
	float angle = time_ / 1000.0 * 10;  // 10° per second
	
	Matrix4 model = Matrix4().Translate(Vector3(0.0, 0.0, -4.0)),
			view = Matrix4().lookAt(Vector3(0.0, 2.0, 0.0), Vector3(0.0, 0.0, -4.0), Vector3(0.0, 1.0, 0.0)),
			projection = Matrix4().Perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 10.0f);
	Vector3 axis_y(0.0, 1.0, 0.0);
	//Matrix4 anim = Matrix4().RotateA(angle, axis_y);
	Matrix4 anim = Matrix4().RotateA(angle*3.0f, Vector3(1, 0, 0)) * 	// X axis
					Matrix4().RotateA(angle*2.0f, Vector3(0, 1, 0)) *	// Y axis
					Matrix4().RotateA(angle*4.0f, Vector3(0, 0, 1));		// Z axis


	Matrix4 m_transform = projection * view * model * anim;
	
	Matrix4 m_3x3_inv_transp = Matrix4();
	glUseProgram(program);

	glUniformMatrix3fv(uniform_m_3x3_inv_transp, 1, GL_FALSE, m_3x3_inv_transp);
	glUniformMatrix4fv(uniform_m_transform, 1, GL_FALSE, m_transform.getMatrix4());

	glutPostRedisplay();
}
 
void onDisplay() {
	/* Clear the background as white */
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	if(program){

		glUseProgram(program);

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture_id);
		//glUniform1i(uniform_mytexture, /*GL_TEXTURE*/0);
		
		glEnableVertexAttribArray(attribute_coord3d);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
		// Describe our vertices array to OpenGL (it can't guess its format automatically) 
		glVertexAttribPointer(
			attribute_coord3d,  // attribute
			3,                  // number of elements per vertex, here (x,y)
			GL_FLOAT,           // the type of each element
			GL_FALSE,           // take our values as-is
			0, 					// no extra data between each position
			0                   // pointer to the C array
		);

		glEnableVertexAttribArray(attribute_v_normal);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_normals);
		// Describe our vertices array to OpenGL (it can't guess its format automatically) 
		glVertexAttribPointer(
			attribute_v_normal,  // attribute
			3,                  // number of elements per vertex, here (x,y)
			GL_FLOAT,           // the type of each element
			GL_FALSE,           // take our values as-is
			0, 					// no extra data between each position
			0                   // pointer to the C array
		);
		
		/*glEnableVertexAttribArray(attribute_v_color);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
		glVertexAttribPointer(
			attribute_v_color,          // attribute
			3,                          // number of elements per vertex, here (r,g,b)
			GL_FLOAT,                   // the type of each element
			GL_FALSE,                   // take our values as-is
			0,      					// no extra data between each position
			0 							// offset of first element
		);*/

		//glEnableVertexAttribArray(attribute_texcoord);
		//glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_texcoords);
		//glVertexAttribPointer(
		//	attribute_texcoord, // attribute
		//	2,                  // number of elements per vertex, here (x,y)
		//	GL_FLOAT,           // the type of each element
		//	GL_FALSE,           // take our values as-is
		//	0,                  // no extra data between each position
		//	0                   // offset of first element
		//);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
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
	glDeleteBuffers(1, &vbo_cube_colors);
	glDeleteBuffers(1, &vbo_cube_normals);
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
	glutCreateWindow("My First Cube");

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
	if (1 == init_resources()) {
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
