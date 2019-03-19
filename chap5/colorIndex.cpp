#include<GL/glut.h>

void f() {
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_COLOR_TABLE);
	glutSetColor(196, 1.0, 1.0, 0.0);
	glIndexi(196);
	glBegin(GL_LINES);
		glVertex2i(30, 30);
		glVertex2i(100, 100);
	glEnd();
	glDisable(GL_COLOR_TABLE);
	glFlush();
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(400, 300);
	glutCreateWindow("An Example");

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);

	glutDisplayFunc(f);
	glutMainLoop();
}
