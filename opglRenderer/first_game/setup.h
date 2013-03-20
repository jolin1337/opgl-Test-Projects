/* Using the standard output for fprintf */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/* Use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* Using the GLUT library for the base windowing setup */
#include <GL/glut.h>

#include "Data.h"

void setupBuffers(GL_Object &obj){

	glGenBuffers(1, &(obj.vbo_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, obj.vbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, obj.size()*sizeof(Vertex), obj.getVertices(), GL_DYNAMIC_DRAW);
	

	glGenBuffers(1, &obj.ibo_indicies);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.ibo_indicies);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj.f_size()*sizeof(GLushort), obj.getFaces(), GL_STATIC_DRAW);

}