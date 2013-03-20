#include "Data.h"
#include <math.h>

enum DIR { X, Y, Z };

class Character : public GL_Object {
	float angle[2];
	Vector3 position;
	static float rmax, rmin;
public:
	Character():position(4.0, 0.0, 0.0){angle[X] = angle[Y] = 0.0f;}
	~Character(){}
	float getRotationX() const{
		return angle[X];
	}
	float getRotationY() const{
		return angle[Y];
	}
	Vector3 getPosition() const{
		return position;
	}
	void moveLeft(float x){
		position.x += x*sin(angle[X]);
		position.z += x*cos(angle[X]);
	}
	void moveForward(float x){
		position.x -= x*cos(angle[X]);
		position.z += x*sin(angle[X]);
	}
	void rotateX(float a){
		if(angle[X] > 2*M_PI){
			int iAng = angle[X]*100;
			angle[X] = (iAng%((int)(2*M_PI*100)))/100.0f; // make it a cycle
		}
		angle[X] += a;
	}
	void rotateY(float a){
		if((angle[Y] > rmax && a > 0) || (angle[Y] < rmin && a < 0)) return;
		angle[Y] += a;
	}
};
float Character::rmax = 70*M_PI/180;
float Character::rmin = -70*M_PI/180;