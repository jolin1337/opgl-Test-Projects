// preformance:
	GLint fps = 0, r_fps = 0;
	bool showPreformance = true;
	extern GLint screen_width;
	extern std::string title;


void PreformanceCheck(int value);
void threadFunc(int value){
	PreformanceCheck(value);
	glutTimerFunc(250, threadFunc, 4);
}
void PreformanceCheck(int value){
	if(!showPreformance)
		return;
	if (value) {
		char *TempString = new char[title.length() + 12];
 		r_fps = fps * 4;
		sprintf(
			TempString,
			"%s, %d fps",
			title.c_str(),
			r_fps
		);
		glutSetWindowTitle(TempString);
		delete[] TempString;
		TempString = 0;
	} 
	fps = 0;
	 
}
void PreformanceIRL(int value){
	if(!showPreformance)
		return;
	if (value) {
		GLfloat offset = 0.0f;
		GLfloat charDist = 12.0f;
 		glColor3f(0,0,0);

 		offset += charDist;
 		glRasterPos2f(offset/screen_width - 1.0f,-1.0f);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, r_fps/10 + 48);

 		offset += charDist;
 		glRasterPos2f(offset/screen_width - 1.0f,-1.0f);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, r_fps%10 + 48);

	} 
	 
}