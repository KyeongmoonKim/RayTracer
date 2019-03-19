#include<GL/glut.h>
#include<math.h>
#include<iostream>
#include"myTrans.h"
//hiearachical modeling 
GLfloat thetaBody;
GLfloat thetaHS;
//It doesn't display anything because the objtect is so far a

void drawBody() {
	glutWireCube(3);
}

void drawHead() {
	glColor3f(0.0, 1.0, 0.0);
	glutWireCube(1);
}
void drawArm() {
	glPushMatrix();
	glScalef(3.0, 1.0, 1.0);
	glColor3f(0.0, 0.0, 1.0);
	glutWireCube(1);
	glPopMatrix();
}


void human() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(3,3,3,0,0,0,0,1,0);
	//start making world coordinates
	glColor3f(1.0, 0.0, 0.0);
	
	glPushMatrix(); //to body edge
	drawBody(); //body at zero-point
	
	glPushMatrix(); //to head edge
	glTranslatef(0.0, 2.0, 0.0); //from head coordinate to body coordinate
	drawHead();
	glPopMatrix(); //from head node
	glPushMatrix(); //to right shoulder joint
	glTranslatef(1.5, 1.5, 0.0); //tranlate
	glRotatef(-1 * getAlpha(-1/sqrt(2), -1/sqrt(2), 0.0), 1, 0, 0); 
	glRotatef(-1 * getBeta(-1/sqrt(2), -1/sqrt(2), 0.0), 0, 1, 0);
	glBegin(GL_LINES);
		glColor3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 6.0);
	glEnd();
	glPushMatrix(); //to right arm
	glTranslatef(1.5, 0.0, 0.0);
	drawArm();
	glPopMatrix(); //from  right arm
	glPopMatrix(); // from right shoulder joint
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
	glMatrixMode(GL_PROJECTION);
	glFrustum(-3, 3, -3, 3, 1, 20);
	glutDisplayFunc(human);
	glutMainLoop();
}
