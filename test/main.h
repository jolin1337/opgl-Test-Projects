
#include "OPGL.h"
OPGL *opgl=new OPGL();

void init();
void onKeyDown(unsigned char key, int x, int y);
void onResize(int w, int h);
void drawScene();

void init() {
	opgl->init();
}

//Called every 25 milliseconds
void update(int value) {
	opgl->loop(value);
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
