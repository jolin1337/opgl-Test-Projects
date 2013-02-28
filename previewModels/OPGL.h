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

void update();
class OPGL {
	float BOX_SIZE;
	unsigned fps;
	bool showPreformance;
    public:
	OB object;	// defined in init
	float _angle[2];
	GLuint _textureId;
	OPGL () {
		showPreformance = false;
		BOX_SIZE = 7.0f;
		_angle[0] = 0;
		_angle[1] = 0;
		fps = 0;
	}
	~OPGL();
	
	void onMouseClick( int button, int state, int x, int y );
	
	void onMouseMove( int x, int y );
	
	void onKeyDown(unsigned char key, int x, int y);
	
	void onResize(int w, int h);
	
	void init();
	void loop();
	void PreformanceCheck(int value);
	
	GLuint loadTexture(Image* image);

	void drawFloor();
	void drawScene();
};
#endif
