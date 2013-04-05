#ifndef BIM_H
#define BIM_H 


#include "Particle.h"
#include "Debris.h"
#include <math.h>
#define PARTICLES 10000
#define DEBS 100

class Bim {
	Particle particles[PARTICLES];
	Debris debs[DEBS];
	bool _hasDetonated,
		exploded;
public:
	Vector3 position;
	Bim():exploded(false),_hasDetonated(false) {
		for (int i = 0; i < PARTICLES; i++){
			particles[i] = Particle(1.0,1.0,0.5);
		}
	}
	void render();
	void update();
	bool hasDetonated(){return _hasDetonated;}
	void detonate(){
		for (int i = 0; i < PARTICLES; i++){
			particles[i].addEnergy(((GLfloat) rand ()) / ((GLfloat) RAND_MAX));
		}
		for (int i = 0; i < DEBS; i++){
			debs[i].addEnergy(((GLfloat) rand ()) / ((GLfloat) RAND_MAX));
		}
		_hasDetonated=true;
	}
};

#endif