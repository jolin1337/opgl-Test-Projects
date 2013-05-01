#ifndef WORLD_H
#define WORLD_H 
#include "BlockSet/BlockSet.h"
#include <vector>

typedef std::vector<BlockSet4> BlockSets;

class World {
	BlockSets blocksets;
	BlockSet4 *activeBlockSet;
	Vector rotation;
	int _updates;
	float drop_speed;
public:
	World();
	void render();
	void update();
	void onKeyEvent(int key, bool down = true);
	void rotateX(float x);
};


#endif