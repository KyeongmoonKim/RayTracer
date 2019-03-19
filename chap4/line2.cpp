#include<GL/glut.h>

void f() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);
	
	glBegin(GL_LINE_STRIP);
		glVertex2i(5, 10);
		glVertex2i(20, 20);
		glVertex2i(30, 30);
		glVertex2i(40, 40);
	glEnd();
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
	f();
	//glutDisplayFunc(f);
	glutMainLoop();
}
