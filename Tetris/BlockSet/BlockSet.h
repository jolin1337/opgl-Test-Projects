#ifndef BLOCK_SET_H
#define BLOCK_SET_H 

#include "Block/Block.h"
#include "../utils/Vector.h"

class BlockSet4 {
	static const int TOP;
	static const float ANGLE;
	Block blocks[4];
	Vector position;
	int rotation;
public:
	bool freezed;
	BlockSet4():position(0,TOP),rotation(0), freezed(false){}
	BlockSet4(Vector poses[]):position(0,TOP),rotation(0), freezed(false){
		blocks[0] = Block(poses[0]);
		blocks[1] = Block(poses[1]);
		blocks[2] = Block(poses[2]);
		blocks[3] = Block(poses[3]);
	}

	void render();
	bool collidesWith(BlockSet4 &b);
	bool collideFloor();

	void rotate();
	void reposition();
	float getX();
	void setHori(float x);
	void moveLeft();
	void moveRight();
	void moveDown();

private:
};

#endif