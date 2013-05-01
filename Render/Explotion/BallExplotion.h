#include "utils/Vector3.h"
class BallExplotion{
public:
	Vector3 position;
	float life;
	BallExplotion():life(0.01f), position(0,0,0){}
	void render(){
		glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		glutSolidSphere(life, 15, 15);

		glPopMatrix();
	}

	void update(){
		life *= 1.06;
		if(life >= 0.5f)
			life = 0.01f;
	}
};