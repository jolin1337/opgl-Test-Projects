#ifndef BLOCK_H
#define BLOCK_H
#include "../../utils/Vector3.h"
#include <stdlib.h>
const float inc = 0.02f;
class Block {
	Vector3 color;
	Vector position;
public:
	Block(Vector position=Vector(0.0f,0.0f), 
			Vector3 color=Vector3(rand()/(float)RAND_MAX,rand()/(float)RAND_MAX,rand()/(float)RAND_MAX)):
		position(position), color(color){}
	void render();
	int getY() const;
	void setY(int y);
	int getX() const;
	void setX(int x);
	static const float BLOCK_SIZE;
	static const float ANGLE;
};

#endif