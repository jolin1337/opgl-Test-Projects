#include "OPGL.h"
OPGL *opgl=new OPGL();

void init();
void update();
void TimerFunction(int value);
void onMouseClick( int button, int state, int x, int y );
void onKeyDown(unsigned char key, int x, int y);
void onResize(int w, int h);
void drawScene();

void init() {
	opgl->init();
}

void update() {
	opgl->loop();
	glutPostRedisplay();
}
void tf(int value){
	opgl->PreformanceCheck(value);
	glutTimerFunc(250, tf, 4);
}

void onMouseClick( int button, int state, int x, int y ) {
	opgl->onMouseClick( button, state, x, y );
}
void onMouseMove( int x, int y ) {
	opgl->onMouseMove( x, y );
}

void onKeyDown(unsigned char key, int x, int y) {
	opgl->onKeyDown( key, x, y );
}

void onResize(int w, int h) {
	opgl->onResize( w, h );
}

void drawScene() {
	opgl->drawScene();
}
