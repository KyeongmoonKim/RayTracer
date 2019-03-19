#include<GL/glut.h>

void f() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.5, 0.5, 0.5);
	GLint a[4][2] = {{5, 10}, {10, 25}, {8, 29}, {1, 28}};
	glBegin(GL_TRIANGLE_STRIP); //need 1 common edge  
		/*
		glVertex2iv(a[0]);
		glVertex2iv(a[1]);
		glVertex2iv(a[2]);
		glVertex2iv(a[3]);
		it has no common edge, and It overlapped
		*/
		glVertex2iv(a[0]);
		glVertex2iv(a[1]);
		glVertex2iv(a[3]);
		glVertex2iv(a[2]);
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

	glutDisplayFunc(f);
	glutMainLoop();
}
