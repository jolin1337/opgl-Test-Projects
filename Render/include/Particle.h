#ifndef PARTICLE_H
#define PARTICLE_H

#include "utils/Vector3.h"
#include <GL/glut.h>

class Particle {
public:
	bool wantNormalize;
	Vector3 position,
			speed,
			color;
	GLfloat energy;
	Particle():energy(0),wantNormalize(false),position(0,0,0),speed(0,0,0),color(0,0,0){newSpeed();}
	Particle(float r, float g, float b):energy(0),wantNormalize(true),position(0,0,0),speed(0,0,0),color(r,g,b){newSpeed();}
	bool hasEnergy(){
		return energy;
	}
	void addEnergy(float e){
		energy+= e;
	}
	void newSpeed(){
		newSpeed(speed);
		if(wantNormalize)
			speed.normalize();
	}
	void update();
	void render();

	static void newSpeed (Vector3 &dest);
};

#endif