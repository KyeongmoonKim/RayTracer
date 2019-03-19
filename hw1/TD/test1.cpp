#include<GL/glut.h>
#include<math.h>

//It doesn't display anything because the objtect is so far away
void f() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	//glRotatef(-45.0, 0.0, 0.0, 1.0);
	glRotatef(-45.0, 0.0, 1.0, 0.0);
	glTranslatef(-150, -150, -150);
	GLint pt[8][3] = {{100, 100, 100}, {200, 100, 100}, {200, 200, 100}, {100, 200, 100},
			      {100, 100, 200}, {200, 100, 200}, {200, 200, 200}, {100, 200, 200}};
	glBegin(GL_QUADS);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3iv(pt[3]);
		glVertex3iv(pt[2]);
		glVertex3iv(pt[1]);
		glVertex3iv(pt[0]);
		glColor3f(0.0, 1.0, 0.0);
		glVertex3iv(pt[5]);
		glVertex3iv(pt[1]);
		glVertex3iv(pt[2]);
		glVertex3iv(pt[6]);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3iv(pt[6]);
		glVertex3iv(pt[2]);
		glVertex3iv(pt[3]);
		glVertex3iv(pt[7]);
		glColor3f(1.0, 1.0, 0.0);
		glVertex3iv(pt[4]);
		glVertex3iv(pt[5]);
		glVertex3iv(pt[6]);
		glVertex3iv(pt[7]);
		glColor3f(1.0, 0.0, 1.0);
		glVertex3iv(pt[7]);
		glVertex3iv(pt[4]);
		glVertex3iv(pt[0]);
		glVertex3iv(pt[3]);
		glColor3f(0.0, 1.0, 1.0);
		glVertex3iv(pt[1]);
		glVertex3iv(pt[5]);
		glVertex3iv(pt[4]);
		glVertex3iv(pt[0]);
	glEnd();
	glFlush();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(700, 700);
	glutCreateWindow("An Example");

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	gluLookAt(0.0, 0.0, 200.0, 0.0, 0.0, 199.0, 0.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 600/600, 60.0, 100.0); 
	glutDisplayFunc(f);
	glutMainLoop();
}
