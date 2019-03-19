#include<GL/glut.h>

void f();
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

void f() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glRasterPos2i(30, 30);	
	GLubyte bitShape[30] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xaa, 0xaa,
				0xaa, 0x11, 0x00, 0x00, 0x00, 0x11, 0x11, 0x11, 0x00,
				0x00, 0x00, 0x11};
	glBitmap(19, 10, 0, 0, 35, 35, bitShape);
	glFlush();
}

