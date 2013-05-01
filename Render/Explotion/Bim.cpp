#include "Bim.h"

void Bim::render(){
	glPushMatrix ();

	glTranslatef(position.x, position.y, position.z);

	if (!_hasDetonated) {
		glColor3f(1.0, 1.0, 1.0);
		glutSolidCube (1.0);
	}
	else if (!exploded) {
		glBegin (GL_POINTS);
		for (int i = 0; i < PARTICLES; i++){
		//	if(particles[i].hasEnergy())
		//		particles[i].render();
		}
		glEnd();

		glEnable (GL_LIGHTING); 
		glEnable (GL_LIGHT0); 
		glEnable (GL_DEPTH_TEST);

		glNormal3f (0.0, 0.0, 1.0);
		for (int i = 0; i < DEBS; i++){
			if(debs[i].hasEnergy())
				debs[i].render();
		}
		glDisable(GL_LIGHTING); 
		glDisable(GL_LIGHT0); 
		glDisable(GL_DEPTH_TEST);
	}
	glPopMatrix ();
}
void Bim::update(){
	if (!exploded && _hasDetonated) {
		bool explodeTest = true;
		for (int i = 0; i < PARTICLES; i++) {
			if(particles[i].hasEnergy()){
				particles[i].update();
				explodeTest = false;
			}
		}
		for (int i = 0; i < DEBS; i++) {
			if(debs[i].hasEnergy()){
				debs[i].update();
				explodeTest = false;
			}
		}
		exploded = explodeTest;
	}
}