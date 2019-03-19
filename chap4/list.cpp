#include<GL/glut.h>

void f() {
	GLint id = glGenLists(1);
	glNewList(id, GL_COMPILE);
		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_LINES);
			glVertex2i(10, 10);
			glVertex2i(100, 100);
		glEnd();
	glEndList();
	glCallList(id);
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
