/* Using the standard output for fprintf */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/* Use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* Using the GLUT library for the base windowing setup */
#include <GL/glut.h>


void setupBuffers(GL_Object &obj){
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
					res_texture.pixel_data);

	glGenBuffers(1, &(obj.vbo_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, obj.vbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, obj.size*3*4, obj.vertices, GL_STATIC_DRAW);
	

	glGenBuffers(1, &obj.ibo_indicies);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.ibo_indicies);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj.f_size*3*4, obj.faces, GL_STATIC_DRAW);
	


	for (int i = 1; i < 6; i++)
		memcpy(&cube_texcoords[i*4*2], &cube_texcoords[0], 2*4*sizeof(GLfloat));
	glGenBuffers(1, &vbo_cube_texcoords);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_texcoords);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_texcoords), cube_texcoords, GL_STATIC_DRAW);

}
int setupUniforms(){
	const char * uniName = "m_transform";
	uniform_m_transform = glGetUniformLocation(program, uniName);
	if (uniform_m_transform == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniName);
		return 0;
	}	
	return 1;
}
int setUpAttributes(){
	const char* attribute_name;
	attribute_name = "coord3d";
	attribute_coord3d = glGetAttribLocation(program, attribute_name);
	if (attribute_coord3d == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return 0;
	}

	attribute_name = "texcoord";
	attribute_texcoord = glGetAttribLocation(program, attribute_name);
	if (attribute_texcoord == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return 0;
	}
	return 1;
}
int setupShaders(){
	GLint link_ok = GL_FALSE;
	GLuint vs, fs;
	if ((vs = create_shader(vertexShaderFileName, GL_VERTEX_SHADER))   == 0) return 0;
	if ((fs = create_shader(fragmentShaderFileName, GL_FRAGMENT_SHADER)) == 0) return 0;

	program = glCreateProgram();
	
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if (!link_ok) {
		fprintf(stderr, "glLinkProgram:\n");
		return 0;
	}

	if(setupUniforms() == 0 || setUpAttributes() == 0)
		return 0;

	return 1;
}