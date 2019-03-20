#include<GL/glut.h>
#include<math.h>
#include<iostream>
#include"myTrans.h"
//hiearachical modeling 
GLfloat thetaBody;
GLfloat thetaRA;

/*
	TODO : The transform matrix should be added before body.
*/
void drawBody() {
	glColor3f(0.0, 0.0, 1.0);
	glutSolidCube(3);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(3.00001);
}

void drawHead() {
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1.00001);
	glColor3f(0.0, 1.0, 0.0);
	glutSolidCube(1.0);
}
void drawArm() {
	glPushMatrix();
	glScalef(1.0, 3.0, 1.0);
	glColor3f(1.0, 0.0, 0.0);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1.00001);
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
	//body local cs
	glPushMatrix(); //to head edge
		glTranslatef(0.0, 2.0, 0.0); //from head coordinate to body coordinate
		//head local cs
		drawHead();
	glPopMatrix(); //from head node
	glPushMatrix(); //to right shoulder joint
		glTranslatef(1.5, 1.5, 0.0); //tranlate
		glRotatef(-1 * getAlpha(-1.0, 0.0, 0.0, 1), 1, 0, 0); 
		glRotatef(-1 * getBeta(-1.0, 0.0, 0.0, 1), 0, 1, 0); //part for transform right joint -> body
		//right shoulder local cs
		glRotaet
		glPushMatrix(); //to right arm
			glTranslatef(0.0, -1.5, -0.5);
			glRotatef(-1 * getAlpha(0, 0, 1, 0), 1, 0, 0);
			glRotatef(-1 * getBeta(0, 0, 1, 0), 0, 1, 0);
			//right arm cs
			drawArm();
		glPopMatrix(); //from  right arm
	glPopMatrix(); // from right shoulder joint
	glutSwapBuffers();
}

/*
when parent to child
T * Rx(-alpha) * Ry(-beta) is multiplied.
when coordinates system doesn't change, the vector values (0, 0, 1).
About animation : 
It's possible manipulating arbitary axis rotation. rotation axis will become new z axis
*/

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
