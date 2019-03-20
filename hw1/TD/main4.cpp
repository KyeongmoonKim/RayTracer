#include<GL/glut.h>
#include<math.h>
#include<iostream>
#include"myTrans.h"
//hiearachical modeling 
GLfloat thetaBody;
GLfloat thetaRA;

/*
	STATE : body and arm attach
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
void drawArm2() {
	glPushMatrix();
	glScalef(1.0, 3.0, 1.0);
	glColor3f(0.0, 1.0, 0.0);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1.00001);
	glPopMatrix();
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
		/*
			part for rotation using axis prefered defined in parents using quadtunian 
		*/
		glTranslatef(1.5, 1.5, 0.0); //tranlate
		//In parent local system, parent want child's z-axis becomes axis defined parents
		
		//body-rightarm1 joint  local cs

		glPushMatrix(); //to right arm1
			glTranslatef(0.5, -1.5, -0.5);
			//right arm cs
			drawArm();
			glPushMatrix();
				glTranslatef(0.0, -1.5, 0.0);
				//rightarm1-arm2 joint
				glPushMatrix();
					glTranslatef(0.0, -1.5, 0.0);
					//right arm2 cs
					drawArm2();
				glPopMatrix();
			glPopMatrix();
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
