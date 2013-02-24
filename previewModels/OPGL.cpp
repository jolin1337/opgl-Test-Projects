#include "OPGL.h" 
#include <math.h>
bool fullscreen = true;
float mousePos[2] = { 0, 0 };
GLvoid ResetWindowSize();

void OPGL::onMouseClick( int button, int state, int x, int y ) {
	if( state == GLUT_DOWN ) {
		mousePos[0] = x + _angle[0];
		mousePos[1] = y + _angle[1];
		//cout<<"Mouse Down, "<<"X:"<<x<<", Y:"<<y<<endl;
	}
	else if( state == GLUT_UP ) {
		//cout<<"Mouse Up, "<<"X:"<<x<<", Y:"<<y<<endl;
	}
}

void OPGL::onMouseMove(int x, int y){
	_angle[0] = mousePos[0] - x;
	if( !((mousePos[1] - y)/4 >= 90 || (mousePos[1] - y)/4 <= -90) )
		_angle[1] = mousePos[1] - y;
	//cout<<"Mouse Move, "<<"X:"<<x<<", Y:"<<y<<endl;
}

void OPGL::onKeyDown(unsigned char key, int x, int y) {
	//cout<<"Key-Code:"<<(int)key<<", X:"<<x<<", Y:"<<y<<endl;
	switch (key) {
		case 'f':
			if(fullscreen) {
				glutReshapeWindow(800, 600);
				glutPositionWindow(0,0);
				fullscreen=false;
			}
			else {
				glutFullScreen();
				fullscreen=true;
			}
			break;
		case 27: //Escape key
			//glutFullScreenToggle();
			exit(0);
	} 
} 

void OPGL::onResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
	glMatrixMode(GL_MODELVIEW);
}

void OPGL::init() { 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING); 
	glEnable(GL_LIGHT0);
	//glEnable(GL_NORMALIZE);
	//glEnable(GL_COLOR_MATERIAL);
	//glShadeModel(GL_FLAT);
	//glClearColor( 0.7f, 0.9f, 1.0f, 1.0f );
	glutFullScreen();
	Image* image = loadBMP("vtr.bmp");
	_textureId = this->loadTexture(image);
	delete image;

	object=OB::loadFromFile("obj.o");

	/*object=new OB();
	
	object->Faces.push_back(new Vector(-BOX_SIZE / 2, BOX_SIZE / 2, -BOX_SIZE / 2));
	object->Faces.push_back(new Vector(-BOX_SIZE / 2, BOX_SIZE / 2, BOX_SIZE / 2));
	object->Faces.push_back(new Vector(BOX_SIZE / 2, BOX_SIZE / 2, BOX_SIZE / 2));
	object->Faces.push_back(new Vector(BOX_SIZE / 2, BOX_SIZE / 2, -BOX_SIZE / 2));

	//Bottom face
	object->Faces.push_back(new Vector(-BOX_SIZE / 2, -BOX_SIZE / 2, -BOX_SIZE / 2));
	object->Faces.push_back(new Vector(BOX_SIZE / 2, -BOX_SIZE / 2, -BOX_SIZE / 2));
	object->Faces.push_back(new Vector(BOX_SIZE / 2, -BOX_SIZE / 2, BOX_SIZE / 2));
	object->Faces.push_back(new Vector(-BOX_SIZE / 2, -BOX_SIZE / 2, BOX_SIZE / 2));

	//Left face
	object->Faces.push_back(object->Faces[4]);
	object->Faces.push_back(object->Faces[7]);
	object->Faces.push_back(object->Faces[1]);
	object->Faces.push_back(object->Faces[0]);

	//Right face
	object->Faces.push_back(object->Faces[5]);
	object->Faces.push_back(object->Faces[3]);
	object->Faces.push_back(object->Faces[2]);
	object->Faces.push_back(object->Faces[6]);

	glEnd();

	//Front face
	object->Faces.push_back(object->Faces[7]);
	object->Faces.push_back(object->Faces[6]);
	object->Faces.push_back(object->Faces[2]);
	object->Faces.push_back(object->Faces[1]);

	//Back face
	object->Faces.push_back(object->Faces[4]);
	object->Faces.push_back(object->Faces[0]);
	object->Faces.push_back(object->Faces[3]);
	object->Faces.push_back(object->Faces[5]);
	*/
}
OPGL::~OPGL(){
	for(unsigned int i=0; i<object->Faces.size(); i++){
		if(object->Faces[i])
			delete object->Faces[i];
		for(unsigned int j = 0; j < object->Faces.size(); j++)
			if(j != i && object->Faces[j] == object->Faces[i])
				object->Faces[j] = 0;
		object->Faces[i] = 0;
	}
	delete object;
}

void OPGL::loop(int value) {
	//_angle += 1.0f;
	//if (_angle > 360) {
	//	_angle -= 360;
	//}
	glutPostRedisplay();
	glutTimerFunc(25, update, 0);
}

GLuint OPGL::loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_RGB,
				 image->width, image->height,
				 0,
				 GL_RGB,
				 GL_UNSIGNED_BYTE,
				 image->pixels);
	return textureId;
}

void OPGL::drawScene() {
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -20.0f);
	glScalef(1.9f, 1.9f, 1.9f);

	glRotatef(-_angle[1]/4, 1.0f, 0.0f, 0.0f);
	glRotatef(-_angle[0]/4, 0.0f, 1.0f, 0.0f);

	/*GLfloat lightColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat lightPos[] = {2 * BOX_SIZE * cos(_angle[0] * M_PI/180), 2*BOX_SIZE * sin(_angle[0] * M_PI/180), 4 * BOX_SIZE, 1.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);*/

	glBegin(GL_TRIANGLES);
	for(unsigned int i=0; i < object->Faces.size();i++){
		if(i%3){
			glColor3f(1.0f, 0.0f, 0.0f);
			if(i + 2 < object->Faces.size()){
				Vector v1 = *(object->Faces[i + 2]),
				       v2 = *(object->Faces[i + 0]),
				       v3 = *(object->Faces[i + 1]);
				v2.sub(v1);
				v3.sub(v1);
				Vector n = v2.cross(v3);
				n.normalize();
				glNormal3f(n.x, n.y, n.z);
			}
		}
		Vector *tmp=object->Faces[i];
		glVertex3f( tmp->x, tmp->y, tmp->z );
	}
	glEnd();
	
	/*//Top face
	glColor3f(1.0f, 1.0f, 0.0f);
	glNormal3f(0.0, 1.0f, 0.0f);
	glVertex3f(-BOX_SIZE / 2, BOX_SIZE / 2, -BOX_SIZE / 2);
	glVertex3f(-BOX_SIZE / 2, BOX_SIZE / 2, BOX_SIZE / 2);
	glVertex3f(BOX_SIZE / 2, BOX_SIZE / 2, BOX_SIZE / 2);
	glVertex3f(BOX_SIZE / 2, BOX_SIZE / 2, -BOX_SIZE / 2);

	//Bottom face
	glColor3f(1.0f, 0.0f, 1.0f);
	glNormal3f(0.0, -1.0f, 0.0f);
	glVertex3f(-BOX_SIZE / 2, -BOX_SIZE / 2, -BOX_SIZE / 2);
	glVertex3f(BOX_SIZE / 2, -BOX_SIZE / 2, -BOX_SIZE / 2);
	glVertex3f(BOX_SIZE / 2, -BOX_SIZE / 2, BOX_SIZE / 2);
	glVertex3f(-BOX_SIZE / 2, -BOX_SIZE / 2, BOX_SIZE / 2);

	//Left face
	glNormal3f(-1.0, 0.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(-BOX_SIZE / 2, -BOX_SIZE / 2, -BOX_SIZE / 2);
	glVertex3f(-BOX_SIZE / 2, -BOX_SIZE / 2, BOX_SIZE / 2);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-BOX_SIZE / 2, BOX_SIZE / 2, BOX_SIZE / 2);
	glVertex3f(-BOX_SIZE / 2, BOX_SIZE / 2, -BOX_SIZE / 2);

	//Right face
	glNormal3f(1.0, 0.0f, 0.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(BOX_SIZE / 2, -BOX_SIZE / 2, -BOX_SIZE / 2);
	glVertex3f(BOX_SIZE / 2, BOX_SIZE / 2, -BOX_SIZE / 2);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(BOX_SIZE / 2, BOX_SIZE / 2, BOX_SIZE / 2);
	glVertex3f(BOX_SIZE / 2, -BOX_SIZE / 2, BOX_SIZE / 2);

	glEnd();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);

	//Front face
	glNormal3f(0.0, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-BOX_SIZE / 2, -BOX_SIZE / 2, BOX_SIZE / 2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(BOX_SIZE / 2, -BOX_SIZE / 2, BOX_SIZE / 2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(BOX_SIZE / 2, BOX_SIZE / 2, BOX_SIZE / 2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-BOX_SIZE / 2, BOX_SIZE / 2, BOX_SIZE / 2);

	//Back face
	glNormal3f(0.0, 0.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-BOX_SIZE / 2, -BOX_SIZE / 2, -BOX_SIZE / 2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-BOX_SIZE / 2, BOX_SIZE / 2, -BOX_SIZE / 2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(BOX_SIZE / 2, BOX_SIZE / 2, -BOX_SIZE / 2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(BOX_SIZE / 2, -BOX_SIZE / 2, -BOX_SIZE / 2);

	glEnd();
	glDisable(GL_TEXTURE_2D);*/

	glutSwapBuffers();
}
