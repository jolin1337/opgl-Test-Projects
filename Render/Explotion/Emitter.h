#include "Particle.h"

struct Sprite : public Particle {
	Vector3 scale, scaleVelocity, 
			rotation, rotationVelocity;
};

class Emitter {
	Sprite *particles;
public:
	Emitter(size_t count = 1):particles(new Sprite[count]){}
	~Emitter(){
		delete[] particles;
	}
};