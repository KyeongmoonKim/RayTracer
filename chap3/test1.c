#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h>

void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 150.0, 0.0, 250.0);
}

void lineSegment() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
		glVertex2i(149, 249);
		glVertex2i(0, 0);
	glEnd();
	glFlush();
}
void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(30, 30);
	glutInitWindowSize(150, 250);
	glutCreateWindow("An Example OpenGL Program");

	init();
	glutDisplayFunc(lineSegment);
	glutMainLoop();
}
