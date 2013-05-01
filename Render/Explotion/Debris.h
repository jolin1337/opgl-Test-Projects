#ifndef DEBRIS_H
#define DEBRIS_H

#include "Particle.h"
#include "Image/Image.h"

class Debris : public Particle {
public:
	Image img;
	Vector3 rotation,
			rotationSpeed,
			scale;
	Debris():rotation(0,0,0) {
		color = Vector3(0.7,0.7,0.7);

		scale.x = (2.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
		scale.y = (2.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
		scale.z = (2.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
		newRotationSpeed();
		newSpeed();
	}
	Debris(float r, float g, float b):rotation(0,0,0) {
		color = Vector3(r,g,b);

		scale.x = (2.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
		scale.y = (2.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
		scale.z = (2.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
		newRotationSpeed();
		newSpeed();
	}

	void addEnergy(float e){
		energy += e;
	}
	void newRotationSpeed(){
		newSpeed(rotationSpeed);
		if(wantNormalize)
			speed.normalize();
	}
	void update(){
		Particle::update();
		/*position.x += speed.x * 0.1;
		position.y += speed.y * 0.1;
		position.z += speed.z * 0.1;
		energy -= 0.005;
		if(energy < 0.0)
			energy = 0.0;
		*/
		rotation.x += rotationSpeed.x * 10;
		rotation.y += rotationSpeed.y * 10;
		rotation.z += rotationSpeed.z * 10;

	}
	void render(){
		glColor4f(color.x, color.y, color.z, energy);

		glPushMatrix();
			
		glTranslatef(position.x,
			position.y,
			position.z);

		glRotatef(rotation.x, 1.0, 0.0, 0.0);
		glRotatef(rotation.y, 0.0, 1.0, 0.0);
		glRotatef(rotation.z, 0.0, 0.0, 1.0);

		glScalef(scale.x,
				scale.y,
				scale.z);

		glBegin(GL_TRIANGLES);
		glVertex3f(0.0, 0.5, 0.0);
		glVertex3f(-0.25, 0.0, 0.0);
		glVertex3f(0.25, 0.0, 0.0);
		glEnd();	  
		
		glPopMatrix();
	}
	
};

#endif