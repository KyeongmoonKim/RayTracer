#include<GL/glut.h>
#include<math.h>

void makeLine(GLint pt1[], GLint pt2[]) {
	glBegin(GL_LINES);
		glVertex2iv(pt1);
		glVertex2iv(pt2);
	glEnd();
}
void f() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);
	const GLdouble two_pi = 6.283185;
	const GLdouble dtheta = 0.01;
	GLdouble theta = dtheta;
	GLint pt[2][2];
	pt[0][0] = 600;
	pt[0][1] = 500;
	const GLint r = 100;
	while(theta < two_pi) {
		pt[1][0] = 2 * r * (cos(theta) - cos(2 * theta)/2) + 500;
		pt[1][1] = 2 * r * (sin(theta) - sin(2 * theta)/2) + 500;
		makeLine(pt[0], pt[1]);
		pt[0][0] = pt[1][0];
		pt[0][1] = pt[1][1];
		theta += dtheta;
	}
	GLint end[2] = {600, 500};
	makeLine(pt[1], end);
	glFlush();
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("An Example");

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 1000.0, 0.0, 1000.0);

	glutDisplayFunc(f);
	glutMainLoop();
}
