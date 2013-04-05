/* Using the standard output for fprintf */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/* Use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* Using the GLUT library for the base windowing setup */
#include <GL/glut.h>

#include "Data.h"

void setupBuffers(GLobject &obj){
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
	glBufferData(GL_ARRAY_BUFFER, obj.size()*sizeof(Vertex), obj.getVertices(), GL_STREAM_DRAW);
	

	glGenBuffers(1, &obj.ibo_indicies);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.ibo_indicies);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj.f_size()*sizeof(GLushort), obj.getFaces(), GL_STATIC_DRAW);
	


	//for (int i = 1; i < 6; i++)
	//	memcpy(&cube_texcoords[i*4*2], &cube_texcoords[0], 2*4*sizeof(GLfloat));
	glGenBuffers(1, &vbo_cube_texcoords);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_texcoords);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_texcoords), cube_texcoords, GL_STATIC_DRAW);

}