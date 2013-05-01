#ifndef PARTICLE_H
#define PARTICLE_H

#include "utils/Vector3.h"
#include <GL/glut.h>

class Particle {
public:
	bool wantNormalize;
	Vector3 position,
			speed,
			force,
			color;
	GLfloat energy, 
			fadeSpeed,
			mass;
	Particle(float r=0, float g=0, float b=0):mass(0.01f),energy(0),fadeSpeed(((GLfloat)rand())/((GLfloat)RAND_MAX)*0.02f+0.03f),wantNormalize(true),position(0,0,0),speed(0,0,0),color(r,g,b){newSpeed();}
	bool hasEnergy(){
		return energy > 0;
	}
	void addEnergy(float e){
		energy+= e;
	}
	void setForce(Vector3 v){
		force = v;
	}
	GLfloat getEnergy() const{
		return energy;
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