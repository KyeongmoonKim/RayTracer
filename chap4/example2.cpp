#include<GL/glut.h>
#include<iostream>

/*class Temp {
	private:
	int a;
	int b;
	public:
	Temp() {
		a = -1;
		b = -1;
	}
	Temp(int _a, int _b) {
		a = _a;
		b = _b;
	}
	void setA(int in) {
		a = in;
	}
	void setB(int in) {
		b = in;
	}
	int getA() {
		return a;
	}
	int getB() {
		return b;
	}
	
};*/
void f() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 1.0);
	GLint pt[12][2] = {{50, 150}, {100, 250}, {150, 276}, {200, 124},
			   {250, 550}, {300, 471}, {350, 482}, {400, 211},
			   {450, 254}, {500, 650}, {550, 583}, {600, 680}
			};
	
	GLint i;
	glBegin(GL_QUADS);
		for(i = 0; i < 12; i++) {
			glVertex2i(pt[i][0]-10, 70);
			glVertex2i(pt[i][0]+10, 70);
			glVertex2i(pt[i][0]+10, 70+pt[i][1]);
			glVertex2i(pt[i][0]-10, 70+pt[i][1]);
		}
	glEnd();
	glColor3f(0.0, 0.0, 0.0);
	GLint x = 42;
	GLint y = 50;
	GLint a[36] = {'J', 'a', 'n', 'F', 'e', 'b', 'M', 'a', 'r', 'A', 'p', 'r',
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
	/*Temp t = Temp(3, 4);
	t.setB(1);
	t.setA(1);
	std::cout << t.getA() << t.getB() << std::endl;*/
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
