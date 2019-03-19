#include<GL/glut.h>
#include<math.h>

GLfloat thetaBody;
GLfloat thetaRS;
//It doesn't display anything because the objtect is so far away
void f() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(-45.0, 1.0, 0.0, 0.0);
	glRotatef(-45.0, 0.0, 1.0, 0.0);
	GLfloat pt[8][3] = {{-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5},
                               {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}};
         glBegin(GL_QUADS);
                glColor3f(1.0, 0.0, 0.0);
                glVertex3fv(pt[3]);
                glVertex3fv(pt[2]);
                glVertex3fv(pt[1]);
                glVertex3fv(pt[0]);
                glColor3f(0.0, 1.0, 0.0);
                glVertex3fv(pt[5]);
                glVertex3fv(pt[1]);
                glVertex3fv(pt[2]);
                glVertex3fv(pt[6]);
                glColor3f(0.0, 0.0, 1.0);
                glVertex3fv(pt[6]);
                glVertex3fv(pt[2]);
                glVertex3fv(pt[3]);
                glVertex3fv(pt[7]);
                glColor3f(1.0, 1.0, 0.0);
                glVertex3fv(pt[4]);
                glVertex3fv(pt[5]);
                glVertex3fv(pt[6]);
                glVertex3fv(pt[7]);
                glColor3f(1.0, 0.0, 1.0);
                glVertex3fv(pt[7]);
                glVertex3fv(pt[4]);
                glVertex3fv(pt[0]);
                glVertex3fv(pt[3]);
                glColor3f(0.0, 1.0, 1.0);
                glVertex3fv(pt[1]);
                glVertex3fv(pt[5]);
                glVertex3fv(pt[4]);
                glVertex3fv(pt[0]);
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
	glutDisplayFunc(f);
	glutMainLoop();
}
