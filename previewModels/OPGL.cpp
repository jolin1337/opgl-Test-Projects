#include "OPGL.h" 
#include <math.h>
bool fullscreen = false;
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
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glLineWidth(3.0);

	glMatrixMode(GL_PROJECTION);
	gluPerspective( 45.0,	// fov
					1.0,	// aspect ratio
					20.0, 	// z near
					100.0 	// z far
	);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0.0, 3.0, 6.0,	// eye 
		0.0, 0.0, 0.0,			// center 
		0.0, 1.0, 0.0);			// up direction 

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.1);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	//glEnable(GL_NORMALIZE);
	//glEnable(GL_COLOR_MATERIAL);
	//glShadeModel(GL_FLAT);
	//glClearColor( 0.7f, 0.9f, 1.0f, 1.0f );
	if(fullscreen)
		glutFullScreen();
	//Image* image = loadBMP("resources/images/vtr.bmp");
	//_textureId = this->loadTexture(image);
	//delete image;

	object=OB::loadFromFile("resources/rect.o");

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
	_angle[0] += 2.0f;
	_angle[0] += 1.0f;
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

static GLfloat floorVertices[4][3] = {
	{ -10.0, 0.0, 10.0 },
	{ 10.0, 0.0, 10.0 },
	{ 10.0, 0.0, -10.0 },
	{ -10.0, 0.0, -10.0 },
};

void OPGL::drawFloor(void) {
	glEnable(GL_LIGHTING);

	glBegin(GL_TRIANGLES);
		glNormal3f(0.0,-1.0,0.0);
		glVertex3fv(floorVertices[0]);
		glVertex3fv(floorVertices[1]);
		glVertex3fv(floorVertices[2]);
		glNormal3f(0.0,-1.0,0.0);
		glVertex3fv(floorVertices[0]);
		glVertex3fv(floorVertices[2]);
		glVertex3fv(floorVertices[3]);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	
}
void OPGL::drawScene() {

	//glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glPushMatrix();
		glRotatef(-_angle[1]/4, 1.0f, 0.0f, 0.0f);
		glRotatef(-_angle[0]/4, 0.0f, 1.0f, 0.0f);

		GLfloat lightColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
		GLfloat lightPos[] = {3.0,-3.0,3.0,0.3};//2 * BOX_SIZE * cos(_angle[0] * M_PI/180), 2*BOX_SIZE * sin(_angle[0] * M_PI/180), 4 * BOX_SIZE, 1.0f};
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

		// Show Light sphere
		glPushMatrix();
			glDisable(GL_LIGHTING);
			glColor3f(1.0, 1.0, 0.0);
			/* Draw a yellow ball at the light source. */
			glTranslatef(-lightPos[0], -lightPos[1], -lightPos[2]);
			glutSolidSphere(0.2, 5, 5);
			glEnable(GL_LIGHTING);
		glPopMatrix();

		glFrontFace(GL_CW);  /* Switch face orientation.*/ 
		glPushMatrix();
			glTranslatef(0.0, -0.92, 0.0);
			drawFloor();
		
			glFrontFace(GL_CCW);  /* Switch face orientation.*/ 
			drawFloor();
		glPopMatrix();
		glBegin(GL_TRIANGLES);
		for(unsigned int i=0; i < object->Faces.size();i+=3){
			//if(i%3){
				glColor3f(5.0f, 0.0f, 0.0f);
				if(i + 2 < object->Faces.size()){
					Vector v1 = *(object->Faces[i + 0]),
					       v2 = *(object->Faces[i + 1]),
					       v3 = *(object->Faces[i + 2]);
					v2.sub(v1);
					v3.sub(v1);
					Vector n = v3.cross(v2);
					n.normalize();
					glNormal3f(n.x, n.y, n.z);
				}
			//}
			for(int j = 0; j < 3;j++){
				Vector *tmp=object->Faces[j+i];
				//glTexCoord2f(tx[i], ty[i]);
				glVertex3f( tmp->x, tmp->y, tmp->z );
			}
		}
		glEnd();
	glPopMatrix();
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
