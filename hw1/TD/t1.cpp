#include<GL/glut.h>
#include<math.h>


void f() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
		glVertex3f(1.0, 0.0, 0.0);
		glVertex3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, 0.0, 1.0);
	glEnd();
	glutSwapBuffers();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(700, 700);
	glutCreateWindow("An Example");

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(1, 1, 1, 0, 0, 0, 0, 1, 0);
	glMatrixMode(GL_PROJECTION);
	glFrustum(-10, 10, -10, 10, 1, 20);
	glutDisplayFunc(f);
	glutMainLoop();
}
