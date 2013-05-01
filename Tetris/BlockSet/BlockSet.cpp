#include "BlockSet.h"
#include <GL/glut.h>
#include <math.h>
#include <iostream>


const int BlockSet4::TOP = 10;
void BlockSet4::render(){
	float ab = position.y;
	if(ab < 0.0f)
		ab *= -1;
	if(ab-inc >= (int)(ab) && !freezed){
		position.y = (float)position.y - inc;
	}
	if(collideFloor()){
		freezed = true;
		reposition();
	}
	//printf("res = %f, p = %f, i = %i\n", position.y, (int)position.y);

	int posDeg = 100;
	glPushMatrix();
		glRotatef(-position.x, 0.0f, 1.0f, 0.0f);
		glTranslatef(0.0f, position.y*Block::BLOCK_SIZE, 0.0f);
		//glRotatef(rotation*M_PI/2,1,0,0);
		for (int i = 0; i < 4; i++) 
			blocks[i].render();

	glPopMatrix();
}

bool BlockSet4::collidesWith(BlockSet4 &b){
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			float blockAX = position.x + blocks[i].getX()*Block::ANGLE,
				  blockBX = b.position.x + b.blocks[j].getX()*Block::ANGLE,
				  blockAY = position.y+inc + blocks[i].getY(),
				  blockBY = b.position.y+inc + b.blocks[j].getY();
			if(blockAX >= 360.0f)
				blockAX -= 360.0f;
			if(blockBX >= 360.0f)
				blockBX -= 360.0f;
			if(blockAX < 0)
				blockAX += 360.0f;
			if(blockBX < 0)
				blockBX += 360.0f;

			float rotLim = blockAX - blockBX;
			float yLim = blockAY - blockBY;
			//if((yLim*(1+inc) <= 1.0f + inc && yLim*(1+inc) >= -1.0f - inc) &&
			bool ycon = yLim < 1.0f   && yLim > -0.9f,
				 xcon = rotLim > -Block::ANGLE && rotLim < Block::ANGLE;
			if(ycon && xcon){
				return true;
			}
		}
	}
	return false;
}
bool BlockSet4::collideFloor(){
	for (int i = 0; i < 4; i++) {
		if((position.y + blocks[i].getY()) <= 0)
			return true;
	}
	return false;
}
void BlockSet4::rotate(){
	Vector centerPoint(0,1);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 2; j++) {
			if(blocks[i].getX() > centerPoint.x && blocks[i].getY() <= centerPoint.y)
				blocks[i].setY(blocks[i].getY()+1);
			else if(blocks[i].getY() > centerPoint.y && blocks[i].getX() >= centerPoint.x)
				blocks[i].setX(blocks[i].getX()-1);
			else if(blocks[i].getX() < centerPoint.x && blocks[i].getY() >= centerPoint.y)
				blocks[i].setY(blocks[i].getY()-1);
			else if(blocks[i].getY() < centerPoint.y && blocks[i].getX() <= centerPoint.x)
				blocks[i].setX(blocks[i].getX()+1);
		}
/*
		if(blocks[i].getX() == centerPoint.x || blocks[i].getY() == centerPoint.y){

		}
		else {
			if(blocks[i].getY() < centerPoint.y && blocks[i].getX() < centerPoint.x ){
				// go up 2*(block[i].getY() - centerPoint.y)
			}
			else if((blocks[i].getY() < centerPoint.y && blocks[i].getX() < centerPoint.x ){

			}
		}*/
	}

	/*for(int i=0; i < 4; i++){
		blocks[i].
	}*/
}

void BlockSet4::reposition(){
	/*int sign = 1;
	if(position.x < 0)
		sign *= -1;
	int n = sign*position.x /Block::ANGLE; // times angle goes in position?
	float dA = sign*position.x - n*Block::ANGLE;		  // 
	//float dA = ((int)(sign*position.x)%((int)(Block::ANGLE)));
	if( dA < Block::ANGLE/2)
		position.x = position.x - sign*dA;
	else
		position.x = position.x + sign*(Block::ANGLE - dA);
	if(position.x >= 360.0f)
		position.x = 0;*/
}

float BlockSet4::getX(){
	return position.x;
}
void BlockSet4::setHori(float x){
	position.x = x;
	reposition();
}
void BlockSet4::moveLeft(){
	position.x++;
}
void BlockSet4::moveRight(){
	position.x--;
}
void BlockSet4::moveDown(){
	if(!freezed)
		position.y-=inc;
}