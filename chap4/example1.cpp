#include<GL/glut.h>

void f() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 1.0);
	GLint pt[12][2] = {{50, 50}, {150, 100}, {176, 150}, {24, 200},
			   {450, 250}, {371, 300}, {382, 350}, {111, 400},
			   {154, 450}, {550, 500}, {483, 550}, {580, 600}
			};
	glBegin(GL_LINE_STRIP);
		GLint i;
		for(i = 0; i < 12; i++) {
			glVertex2i(pt[i][1], 100+pt[i][0]);
		}
	glEnd(); //line

	glColor3f(1.0, 0.0, 0.0);
	for(i = 0; i < 12; i++) {
		glRasterPos2i(46 + 50 * i, pt[i][0]+96);
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '*');
	}

	glColor3f(0.0, 0.0, 0.0);
	// GLint i; glBeigin End aren't for statement.
	GLint x = 42;
	GLint y = 50;
	GLint a[36] = {'J', 'a', 'n', 'F', 'e', 'B', 'M', 'a', 'r', 'A', 'p', 'r',
		       'M', 'a', 'y', 'J', 'u', 'n', 'J', 'u', 'l', 'A', 'u', 'g',
		       'S', 'e', 'p', 'O', 'c', 't', 'N', 'o', 'v', 'D', 'e', 'c'};
	for(i = 0; i < 12; i++) {
		glRasterPos2i(x+i*50, y);
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, a[i*3 + 0]);
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, a[i*3 + 1]);
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, a[i*3 + 2]);
	}
	glFlush();
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(1000, 800);
	glutCreateWindow("An Example");

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 800.0, 0.0, 1000.0);

	glutDisplayFunc(f);
	glutMainLoop();
}
