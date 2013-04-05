#include "Particle.h"

void Particle::update(){
	position.x += speed.x * 0.2 * energy;
	position.y += speed.y * 0.2 * energy;
	position.z += speed.z * 0.2 * energy;
	float factor = 0.1 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX);
	
	color.x -= 1.0 / 40.0 - factor*abs(speed.y);
	if (color.x < 0.0)
		color.x = 0.0;
	
	color.y -= 1.0 / 20.0 - factor*abs(speed.y);
	if (color.y < 0.0)
		color.y = 0.0;
		
	color.z -= 1.0 / 15.0 - factor*abs(speed.y);
	if (color.z < 0.0)
		color.z = 0.0;

	energy -= factor;// / ((GLfloat) rand ());
	if(energy < 0.0)
		energy = 0.0;
}
void Particle::render(){
	glColor3f(color.x, color.y, color.z);
	glVertex3f(position.x, position.y, position.z);
}

void Particle::newSpeed (Vector3 &dest) {
	float x;
	float y;
	float z;
	float len;

	x = (2.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
	y = (2.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
	z = (2.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;

	dest.x = x;
	dest.y = y;
	dest.z = z;
}