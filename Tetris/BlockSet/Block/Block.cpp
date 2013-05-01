#include "Block.h"
#include "../../Data.h"
#include <GL/glut.h>

float const Block::BLOCK_SIZE = 0.2f;
float const Block::ANGLE = atan(Block::BLOCK_SIZE)*180/M_PI;


void Block::render(){
	glPushMatrix();
		int sign = 1;
		if(position.y < 0)
			sign *= -1;
		glRotatef(position.x * ANGLE, 0.0f, 1.0f, 0.0f);
		glTranslatef(0.0f, (position.y+sign*inc)*(BLOCK_SIZE), 1.0f);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, (float*)(&color));
		//glColor3f(color.x, color.y, color.z);
		glutSolidCube(BLOCK_SIZE);
	glPopMatrix();
}

int Block::getY() const{
	return position.y;
}
void Block::setY(int y){
	position.y = y;
}
int Block::getX() const{
	return position.x;
}
void Block::setX(int x){
	position.x = x;
}