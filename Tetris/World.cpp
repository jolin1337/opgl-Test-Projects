#include "World.h"
#include <GL/glut.h>
#include <stdio.h>

World::World():rotation(0,0), _updates(0), drop_speed(2.5f){
	Vector poses[4];
	poses[0] = Vector(-1, 0);
	poses[1] = Vector(0, 0);
	poses[2] = Vector(1, 0);
	poses[3] = Vector(0, 1);
	
	BlockSet4 block(poses);
	blocksets.push_back(block);
	activeBlockSet = &blocksets[0];
}
void World::render(){
	glPushMatrix();
		glRotatef(rotation.x, 0.0f, 1.0f, 0.0f);
		glRotatef(rotation.y, 1.0f, 0.0f, 0.0f);
		glPushMatrix();
			glScalef(0.1f, 1.0f, 0.1f);
			
			float color[] = {1.0f,0.0f,0.0f};
			glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
			glutSolidCube(4.0f);
		glPopMatrix();
		float s = 1.5f;
		glTranslatef(0.0f, -Block::BLOCK_SIZE/2, 0.0f);
		glNormal3f(0.0f,1.0f,0.0f);
		glBegin(GL_TRIANGLES);
			glVertex3f(s,0.0f,s);
			glVertex3f(s,0.0f,-s);
			glVertex3f(-s,0.0f,-s);

			glVertex3f(s,0.0f,s);
			glVertex3f(-s,0.0f,s);
			glVertex3f(-s,0.0f,-s);
		glEnd();
		glTranslatef(0.0f, Block::BLOCK_SIZE/2, 0.0f);

		for (BlockSets::iterator q = blocksets.begin(); q != blocksets.end(); ++q) 
			q->render();

	glPopMatrix();
}
void World::update(){
	float time_ = glutGet(GLUT_ELAPSED_TIME);

	if((time_ - _updates)*drop_speed > 1900)
		_updates = time_;

	if(_updates == time_){
		for (BlockSets::iterator q = blocksets.begin(); q != blocksets.end(); ++q) {
			if(!q->freezed ){
				q->moveDown();
			}
		}

		for (BlockSets::iterator q1 = blocksets.begin(); q1 != blocksets.end(); ++q1) {
			for (BlockSets::iterator q2 = blocksets.begin(); q2 != q1; ++q2) {
				if(q1->collidesWith(*q2)){
					if(!q1->freezed){
						q1->reposition();
						q1->freezed = true;
					}
					if(!q2->freezed){
						q2->reposition();
						q2->freezed = true;
					}
				}
			}
			if(!q1->freezed && q1->collideFloor()){
				q1->freezed = true;
				q1->reposition();
			}
		}
		if(activeBlockSet && activeBlockSet->freezed){
			Vector poses[4];
			poses[0] = Vector(-1, 1);
			poses[1] = Vector(0, 1);
			poses[2] = Vector(1, 1);
			poses[3] = Vector(0, 2);
			
			activeBlockSet = 0;	// TODO: add new blockset!!!
			BlockSet4 block(poses);
			blocksets.push_back(block);
			activeBlockSet = &(blocksets[blocksets.size()-1]);
			for (BlockSets::iterator q = blocksets.begin(); q != blocksets.end(); ++q)
				if(q->freezed && activeBlockSet->collidesWith(*q))
					exit(0);// lost here!
		}
		_updates--;
	}
}

void World::onKeyEvent(int key, bool down){
	switch(key){
		case ' ':
			if(!down)
				activeBlockSet->rotate();
			break;
		case 's':
		case 'S':
			if(!down){
				BlockSet4 &tmp = *activeBlockSet;
				while(!tmp.freezed){
					_updates = glutGet(GLUT_ELAPSED_TIME);
					update();
				}
			}
			break;
	}
}
void World::rotateX(float x){
	rotation.x += x;
	if(rotation.x >= 360.0f)
		rotation.x = 0;
	if(rotation.x < 0.0f)
		rotation.x = 360.0f;
	if(activeBlockSet){
		activeBlockSet->setHori((rotation.x));
		for (BlockSets::iterator q = blocksets.begin(); q != blocksets.end(); ++q) {
			if(q->freezed && activeBlockSet->collidesWith(*q)){
				rotation.x -= x;
				activeBlockSet->setHori((rotation.x));
			}
		}
	}
}