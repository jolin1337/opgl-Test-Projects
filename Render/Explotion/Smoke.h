#ifndef SMOKE_H
#define SMOKE_H
#define CLOUDS 10

#include "Particle.h"
#include "Image/Image.h"

#define TEX_INTERNAL GL_RGBA4
#define TEX_FORMAT   GL_RGBA
#define TEX_TYPE     GL_UNSIGNED_SHORT_4_4_4_4

// here's a few hardcoded RGBA color values
#define R 0xf30f
#define W 0xffff
#define X 0x0000
#define G 0x5c6c
#define B 0x111f

// and here's a hardcoded image
unsigned short mushroom_texture [(16 * 16)] = {
	X,X,X,B,B,B,B,B,B,B,B,B,B,X,X,X,
	X,X,B,B,W,W,W,W,W,W,W,W,B,B,X,X,
	X,X,B,W,W,W,W,W,W,W,W,W,W,B,X,X,
	X,B,B,W,W,W,R,W,W,R,W,W,W,B,B,X,
	B,B,B,B,W,W,R,W,W,R,W,W,B,B,B,B,
	B,W,W,B,B,B,B,B,B,B,B,B,B,W,W,B,
	B,W,W,W,W,W,G,G,G,G,W,W,W,W,W,B,
	B,G,G,W,W,G,G,G,G,G,G,W,W,G,G,B,
	B,G,G,G,W,G,G,G,G,G,G,W,G,G,G,B,
	B,G,G,G,W,G,G,G,G,G,G,W,G,G,G,B,
	B,B,G,G,W,W,G,G,G,G,W,W,G,G,B,B,
	X,B,G,W,W,W,W,W,W,W,W,W,W,G,B,X,
	X,B,B,W,G,G,W,W,W,W,G,G,W,B,B,X,
	X,X,B,B,G,G,G,W,W,G,G,G,B,B,X,X,
	X,X,X,B,B,B,G,W,W,G,B,B,B,X,X,X,
	X,X,X,X,X,B,B,B,B,B,B,X,X,X,X,X,
};

enum ELEMENT { 
	FIRE, ICE, WATER, AIR
};
class Cloud : public Particle {
	unsigned char *rawimg;
	ELEMENT type;
public:
	Cloud(){}
};

class Smoke {
	Image image;
	unsigned char *rawimg;
	Cloud clouds[CLOUDS];
	GLuint imgID;
	float height, radius;
	ELEMENT smokeType;
public:
	Smoke():imgID(0), height(1.05f), radius(4.0f) {
	}
	Smoke(float radius):imgID(0), height(1.05f), radius(radius){
	}
	Smoke(std::string src):imgID(0), height(1.05f), radius(4.0f){
		loadSmoke(src);
	}
	Smoke(std::string src, ELEMENT e):imgID(0), height(1.05f), smokeType(e), radius(4.0f){
		loadSmoke(src);
	}
	Smoke(std::string src, ELEMENT e, float radius):imgID(0), height(1.05f), smokeType(e), radius(radius){
		loadSmoke(src);
	}
	Smoke(std::string src, float radius):imgID(0), height(1.05f), radius(radius){
		loadSmoke(src);
	}
	void loadSmoke(std::string src){
		image.readImage(src);
		for (int i = 0; i < CLOUDS; i++){
			refresh(i);
			clouds[i].addEnergy(2.5);
		}
		if(image.isOpen()){
			rawimg = image.getPixlArray();
			for (int i = 0; i < image.getHeight(); i++) {
				for (int j = 0; j < image.getWidth(); j++) {
					unsigned char &r = rawimg[4*i*image.getWidth() + 4*j],
								  &g = rawimg[4*i*image.getWidth() + 4*j + 1],
								  &b = rawimg[4*i*image.getWidth() + 4*j + 2],
								  &a = rawimg[4*i*image.getWidth() + 4*j + 3];
					switch(smokeType){
						case FIRE:
							setFireColor(r, g, b, a);
							//rawimg[4*i*image.getWidth() + 4*j + 2] = b*0.8;
							break;
						case ICE:
							break;
						case WATER:
							break;
						case AIR:
							setAirColor(r, g, b, a);
							break;
						default:
							i = image.getHeight();
							j = image.getWidth();
					}
				}
			}
			//glGenTextures(1, &imgID);
			
			//glBindTexture(GL_TEXTURE_2D, imgID);
			
		}
	}
	void update(){
		for (int i = 0; i < CLOUDS; i++) {
			clouds[i].update();
			if(!clouds[i].hasEnergy()){
				refresh(i);
			}
		}
	}
	void reset(){
		for(int i=0; i< CLOUDS; i++)
			refresh(i);
	}
	void refresh(int i){
		clouds[i].position = Vector3(0,0,0);
		clouds[i].newSpeed();
		if(clouds[i].speed.y > height)
			clouds[i].speed.y = height;
		else if(clouds[i].speed.y < -height)
			clouds[i].speed.y = -height;
		clouds[i].speed.div(25);
		if(rand()%10 < 5)
			clouds[i].addEnergy(radius + (rand()%6 - (radius > 3.5f ? 3.5f : radius)));
	}
	void renderPrep(){
		if(!image.isOpen())return;
		glTexImage2D(GL_TEXTURE_2D, 
				0, 
				GL_RGBA4, 
				image.getWidth(), 
				image.getHeight(), 
				0, 
				GL_RGBA, 
				GL_UNSIGNED_BYTE, 
				rawimg
			);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		
		//glTranslatef(camRot.x, camRot.y, camRot.z);
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0, 0.0f, 1.0f);
	}
	void render(Vector3 camRot){
		if(!image.isOpen()) 
			return;
		glPushMatrix();
		glScalef(0.3,0.3,0.3);
		
		/*
		glTexImage2D(GL_TEXTURE_2D, 
			0, 
			TEX_INTERNAL, 
			16, 
			16, 
			0, 
			TEX_FORMAT, 
			TEX_TYPE, 
			mushroom_texture
		);*/
		
		glEnable(GL_TEXTURE_2D);
		for (int i = 0; i < CLOUDS; i++) {
			glPushMatrix();
			glTranslatef(clouds[i].position.x,clouds[i].position.y,clouds[i].position.z);
			glRotatef(camRot.x, 1.0f, 0.0f, 0.0f);
			glRotatef(camRot.y, 0.0f, 1.0f, 0.0f);
			glRotatef(camRot.z, 0.0f, 0.0f, 1.0f);

			/*// Ring of fire:
			glBegin(GL_TRIANGLES);
			// top tri
				glTexCoord2f(0.75, 0.5);	glVertex3f(1.0f,1.0f,0.0f);
				glTexCoord2f(0.75, 0.8);	glVertex3f(1.0f,-1.0f,0.0f);
				glTexCoord2f(1,    0.5);	glVertex3f(-1.0f,1.0f,0.0f);

			// bottom tri
				glTexCoord2f(1,    0.8);	glVertex3f(-1.0f,-1.0f,0.0f);
				glTexCoord2f(1,    0.5);	glVertex3f(-1.0f,1.0f,0.0f);
				glTexCoord2f(0.75, 0.8);	glVertex3f(1.0f,-1.0f,0.0f);
			glEnd();*/
			if(clouds[i].getEnergy() > 4.0f){// explosion1
				glBegin(GL_TRIANGLES);
				// top tri
					glTexCoord2f(0.0 , 0.0 );		glVertex3f(1.0f,1.0f, 0.0f);
					glTexCoord2f(0.0 , 0.25);		glVertex3f(1.0f,-1.0f, 0.0f);
					glTexCoord2f(0.25, 0.0 );		glVertex3f(-1.0f,1.0f, 0.0f);

				// bottom tri
					glTexCoord2f(0.25, 0.25);		glVertex3f(-1.0f,-1.0f, 0.0f);
					glTexCoord2f(0.25, 0.0 );		glVertex3f(-1.0f,1.0f, 0.0f);
					glTexCoord2f(0.0 , 0.25);		glVertex3f(1.0f,-1.0f, 0.0f);
				glEnd();
			}
			else if(clouds[i].getEnergy() > 2.0f){
				// explosion2
				glBegin(GL_TRIANGLES);
				// top tri
					glTexCoord2f(0.25, 0.25);		glVertex2f(1.0f,1.0f);
					glTexCoord2f(0.25, 0.50);		glVertex2f(1.0f,-1.0f);
					glTexCoord2f(0.50, 0.25);		glVertex2f(-1.0f,1.0f);

				// bottom tri
					glTexCoord2f(0.50, 0.50 );		glVertex2f(-1.0f,-1.0f);
					glTexCoord2f(0.50, 0.25);		glVertex2f(-1.0f,1.0f);
					glTexCoord2f(0.25, 0.50 );		glVertex2f(1.0f,-1.0f);
				glEnd();
			}
			else if(clouds[i].getEnergy() > 1.0f){
				// explosion3
				glBegin(GL_TRIANGLES);
				// top tri
					glTexCoord2f(0.25 , 0.0);		glVertex2f(1.0f,1.0f);
					glTexCoord2f(0.25 , 0.25);		glVertex2f(1.0f,-1.0f);
					glTexCoord2f(0.50, 0.0);		glVertex2f(-1.0f,1.0f);

				// bottom tri
					glTexCoord2f(0.50, 0.25 );		glVertex2f(-1.0f,-1.0f);
					glTexCoord2f(0.50, 0.0);		glVertex2f(-1.0f,1.0f);
					glTexCoord2f(0.25 , 0.25 );		glVertex2f(1.0f,-1.0f);
				glEnd();
			}
			else{
				// explosion4
				glBegin(GL_TRIANGLES);
				// top tri
					glTexCoord2f(0.0 , 0.25);		glVertex2f(1.0f,1.0f);
					glTexCoord2f(0.0 , 0.5);		glVertex2f(1.0f,-1.0f);
					glTexCoord2f(0.25, 0.25);		glVertex2f(-1.0f,1.0f);

				// bottom tri
					glTexCoord2f(0.25, 0.5 );		glVertex2f(-1.0f,-1.0f);
					glTexCoord2f(0.25, 0.25);		glVertex2f(-1.0f,1.0f);
					glTexCoord2f(0.0 , 0.5 );		glVertex2f(1.0f,-1.0f);
				glEnd();
			}/*
			// Orig
			glBegin(GL_TRIANGLES);
			// top tri
				glTexCoord2f(0.0 , 0.0 );		glVertex2f(1.0f,1.0f);
				glTexCoord2f(0.0 , 1.0);		glVertex2f(1.0f,-1.0f);
				glTexCoord2f(1.0 , 0.0 );		glVertex2f(-1.0f,1.0f);

			// bottom tri
				glTexCoord2f(1.0 , 1.0);		glVertex2f(-1.0f,-1.0f);
				glTexCoord2f(1.0 , 0.0 );		glVertex2f(-1.0f,1.0f);
				glTexCoord2f(0.0 , 1.0);		glVertex2f(1.0f,-1.0f);
			glEnd();*/

			
			glPopMatrix();
		}
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	void setFireColor(unsigned char &r, unsigned char &g, unsigned char &b, unsigned char &a){
		r = r*3> 255?255:r*3;
		g = g*1.3 > 255?255:g*1.3;
		a = ((r + g + b)/8.0f);
		a = a > 255?255:(a<0?0:a);
	}
	void setAirColor(unsigned char &r, unsigned char &g, unsigned char &b, unsigned char &a){
		a = ((r + g + b)/10.0f);
		a = a > 255?255:(a<0?0:a);
	}
};

#endif