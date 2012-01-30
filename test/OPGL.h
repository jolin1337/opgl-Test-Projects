#ifndef _OPGL_
#define _OPGL_

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "imageloader.h"

void update(int value);
class OPGL {
	float BOX_SIZE;
    public:
	float _angle;
	GLuint _textureId;
	OPGL () {
		BOX_SIZE = 7.0f;
		_angle = 0;
	}
	
	void onKeyDown(unsigned char key, int x, int y);
	
	void onResize(int w, int h);
	
	void init();
	void loop(int value);
	
	GLuint loadTexture(Image* image);

	void drawScene();
};
#endif
