#include<GL/glut.h>
#include<math.h>

void f() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);

	GLint i;
	GLdouble pi2 = 6.2831853;
	glBegin(GL_POLYGON);
		GLdouble theta;
		for(i = 0; i < 6; i++) {
			theta = pi2 * i / 6.0;
			GLint x = 200 + 150 * cos(theta);
			GLint y = 200 + 150 * sin(theta);
			glVertex2i(x, y);
		}
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
	gluOrtho2D(0.0, 400.0, 0.0, 400.0);

	glutDisplayFunc(f);
	glutMainLoop();
}
