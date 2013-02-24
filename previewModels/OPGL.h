#ifndef _OPGL_
#define _OPGL_

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "imageloader.h"
#include "OB.h"

void update(int value);
class OPGL {
	float BOX_SIZE;
    public:
	OB *object;	// defined in init
	float _angle[2];
	GLuint _textureId;
	OPGL () {
		BOX_SIZE = 7.0f;
		_angle[0] = 0;
		_angle[1] = 0;
	}
	~OPGL();
	
	void onMouseClick( int button, int state, int x, int y );
	
	void onMouseMove( int x, int y );
	
	void onKeyDown(unsigned char key, int x, int y);
	
	void onResize(int w, int h);
	
	void init();
	void loop(int value);
	
	GLuint loadTexture(Image* image);

	void drawScene();
};
#endif
