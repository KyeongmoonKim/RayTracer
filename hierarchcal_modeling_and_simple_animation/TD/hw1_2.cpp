#include<GL/glut.h>
#include<math.h>
#include<iostream>
#include"myTrans.h"
#define PI 3.14159265
//hiearachical modeling 

double lleg1Move;
double lleg2Move;
double rleg1Move;
double rleg2Move;
int counter;
double bodyMove;
double ll1Move;
double ll2Move;
double rl1Move;
double rl2Move;
/*
	STATE : body and arm detach. rotating joint
	TODO : The transform matrix should be added before body.
*/

void myCube(GLfloat sx, GLfloat sy, GLfloat sz, GLfloat r, GLfloat g, GLfloat b, GLfloat len) {
	glPushMatrix();
	{
		glScalef(sx, sy, sz);
		glColor3f(r, g, b);
		glutSolidCube(len);
		glColor3f(0.0, 0.0, 0.0);
		glutWireCube(len + 0.00001);
	}
	glPopMatrix();
}

void joint(GLfloat rad) {
	glPushMatrix();
	{
		glColor3f(1.0, 1.0, 0.0);
		glutSolidSphere(rad, 10, 10);
	}
	glPopMatrix();
}

void mySphere(GLfloat sx, GLfloat sy, GLfloat sz, GLfloat r, GLfloat g, GLfloat b, GLfloat len) {
	glPushMatrix();
	glScalef(sx, sy, sz);
	glColor3f(r, g, b);
	glutSolidSphere(len, 10, 10);
	glPopMatrix();
}



void human() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(45.0, 45.0, 45.0, 0,0,0, 0,1,0);
	//start making world coordinate
	glColor3f(0.0, 0.0, 0.0);
	glPushMatrix();
	{
		glBegin(GL_LINES);
			glVertex3f(0, 0, 0);
			glVertex3f(0, 0, 500);
			glVertex3f(0, 0, 0);
			glVertex3f(0, 500, 0);
			glVertex3f(0, 0, 0);
			glVertex3f(500, 0, 0);
		glEnd();
		glBegin(GL_POLYGON);
			glColor3f(0.7, 0.7, 0.7);
			glVertex3f(500, -0.0001, 0);
			glVertex3f(0, -0.0001, 0);
			glVertex3f(0, -0.0001, 500);
		glEnd();
	}
	glPopMatrix();
	glPushMatrix();
	{//human
		glTranslatef(10.0, 11.0+bodyMove, 10.0);
		myCube(1.0, 1.0, 0.5, 0.0, 0.0, 1.0, 2.0);
		//body local cs
		glPushMatrix(); 
		{//neck
			glTranslatef(0.0, 1.5, 0.0);
			joint(0.5);
			glPushMatrix();
			{//head
				glTranslatef(0.0, 1.0, 0.0); //from head coordinate to body coordinate
				myCube(1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0);
				glTranslatef(0.0, 0.0, 0.5001);
				glPushMatrix();
				{//eye space
					glBegin(GL_QUADS);
						glColor3f(0.0, 0.0, 0.0);
						glVertex3f(0.4, 0.1, 0.0);
						glVertex3f(0.4, 0.4, 0.0);
						glVertex3f(0.1, 0.4, 0.0);
						glVertex3f(0.1, 0.1, 0.0);
						glVertex3f(-0.1, 0.1, 0.0);
						glVertex3f(-0.1, 0.4, 0.0);
						glVertex3f(-0.4, 0.4, 0.0);
						glVertex3f(-0.4, 0.1, 0.0);
					glEnd();
				}
				glPopMatrix();
			}
			glPopMatrix();
		}
		glPopMatrix(); //from head node
		glPushMatrix(); //to right shoulder joint
		{//left shoulder 
			glTranslatef(1.5, 0.5, 0.0);
			joint(0.5);
			glPushMatrix();
			{//for arm1
				glRotatef(ll1Move, 0.0, 0.0, 1.0); //arm1 up-down rotation will be happen
				glTranslatef(0.0, -1.25, 0.0);
				//glRotatef(90.0, 0.0, 0.0, 1.0); //transform coordinates
				myCube(0.5, 1.5, 0.5, 1.0, 0.0, 0.0, 1.0); 
				glPushMatrix();
				{//for arm1-2 joint
					glTranslatef(0.0, -1.25, 0.0);
					joint(0.5);
					glPushMatrix();
					{//for arm2
						glRotatef(ll2Move, 0, 0, 1);
						glTranslatef(0.0, -1.25, 0.0);
						myCube(0.5, 1.5, 0.5, 0.0, 1.0, 0.0, 1.0);
					}
					glPopMatrix();
				}
				glPopMatrix();
			}
			glPopMatrix();

		}
		glPopMatrix(); // from right shoulder joint
		glPushMatrix();
		{//right shoulder
			glTranslatef(-1.5, 0.5, 0.0);
			joint(0.5);
			glPushMatrix();
			{//for leftArm1
				glRotatef(-1*rl1Move, 0.0, 0.0, 1.0);
				glTranslatef(0.0, -1.25, 0.0);
				//glRotatef(90.0, 0.0, 0.0, 1.0);//tranform coordinate
				myCube(0.5, 1.5, 0.5, 1.0, 0.0, 0.0, 1.0);
				glPushMatrix();
				{//Left arm 1-2 joint
					glTranslatef(0.0, -1.25, 0.0);
					joint(0.5);
					glPushMatrix();
					{
						glRotatef(-1*rl2Move, 0, 0, 1);
						glTranslatef(0.0, -1.25, 0.0);
						myCube(0.5, 1.5, 0.5, 0.0, 1.0, 0.0, 1.0);
					}
					glPopMatrix();
				}
				glPopMatrix();
			}
			glPopMatrix();
		}
		glPopMatrix();
		glPushMatrix();
		{//gobanjoint
			glTranslatef(0.0, -1.5, 0.0);
			joint(0.5);
			glPushMatrix();
			{//golban
				glTranslatef(0.0, -1.0, 0.0);
				myCube(2.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0);
				glPushMatrix();
				{//left-leg1-joint
					glTranslatef(0.5, -1.0, 0.0);
					joint(0.5);
					glRotatef(-1 * lleg1Move, 1, 0, 0);
					glPushMatrix();
					{//leg1
						glTranslatef(0.0, -2.0, 0.0);
						myCube(0.5, 3.0, 0.5, 1.0, 0.0, 0.0, 1.0);
						glPushMatrix();
						{//leg2 joint
							glTranslatef(0.0, -2.0, 0.0);
							joint(0.5);
							glRotatef(-1 * lleg2Move, 1, 0, 0);
							glPushMatrix();
							{//leg2
								glTranslatef(0.0, -2.0, 0.0);
								myCube(0.5, 3.0, 0.5, 0.0, 1.0, 0.0, 1.0);
							}
							glPopMatrix();
						}
						glPopMatrix();
					}
					glPopMatrix();
				}
				glPopMatrix();
				glPushMatrix();
				{//right-leg1-joint
					glTranslatef(-0.5, -1.0, 0.0);
					joint(0.5);
					glRotatef(-1 * rleg1Move, 1, 0, 0);
					glPushMatrix();
					{//leg1
						glTranslatef(0.0, -2.0, 0.0);
						myCube(0.5, 3.0, 0.5, 1.0, 0.0, 0.0, 1.0);
						glPushMatrix();
						{//leg2 joint
							glTranslatef(0.0, -2.0, 0.0);
							joint(0.5);
							glRotatef(-1*rleg2Move, 1, 0, 0);
							glPushMatrix();
							{//leg2
								glTranslatef(0.0, -2.0, 0.0);
								myCube(0.5, 3.0, 0.5, 0.0, 1.0, 0.0, 1.0);
							}
							glPopMatrix();
						}
						glPopMatrix();
					}
					glPopMatrix();
				}
				glPopMatrix();
			}
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();
	glutSwapBuffers();
}

void legAni(int counter) {
	if(counter <= 50) {
		lleg1Move += 1.8;
	} else if(counter <= 100) {
		lleg1Move -= 1.8;
	} else if(counter <= 150) {
		rleg1Move += 1.8; 
	} else if(counter <= 200) {
		rleg1Move -= 1.8;
	}
}
void bodyAni(int counter) {
}
void armAni(int counter) {
	if(counter <= 50) {
		ll1Move += 2.61;
		ll2Move += 0.4;
		rl1Move += 2.61;
		rl2Move += 0.4;
	} else if(counter <= 100) {
		ll1Move -= 2.61;
		ll2Move -= 0.4;
		rl1Move -= 2.61;
		rl2Move -= 0.4;
	} else if(counter <= 150) {
		ll1Move += 2.61;
		ll2Move += 0.4;
		rl1Move += 2.61;
		rl2Move += 0.4;
	} else if(counter <= 200) {
		ll1Move -= 2.61;
		ll2Move -= 0.4;
		rl1Move -= 2.61;
		rl2Move -= 0.4;
	} else if(counter <= 250) {
		ll1Move += 3.6;
		ll2Move += acos(25.0/30.0)*180/(50.0*PI);
		rl1Move += 3.6;
		rl2Move += acos(25.0/30.0)*180/(50.0*PI);
	} else if(counter <= 300) {
		ll1Move -= 3.6;
		ll2Move -= acos(25.0/30.0)*180/(50.0*PI);
		rl1Move -= 3.6;
		rl2Move -= acos(25.0/30.0)*180/(50.0*PI);
	}
}
unsigned timeStep = 20;
void timer(int unUsed) {
	legAni(counter);
	bodyAni(counter);
	armAni(counter);
	counter++;
	glutPostRedisplay();
	glutTimerFunc(timeStep, timer, 0);
}

void reShape(int newWeight, int newHeight) {
	glViewport(0, 0, newWeight, newHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)newWeight/newHeight, 0.1, 400.0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(700, 700);
	glutCreateWindow("An Example");

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClearDepth(1.0);
	counter = 1;
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glMatrixMode(GL_PROJECTION);
	//glFrustum(-3, 3, -3, 3, 1, 500);
	gluPerspective(45.0, 1.0, 0.1, 400.0);
	glutDisplayFunc(human);
	glutReshapeFunc(reShape);
	glutTimerFunc(timeStep, timer, 0);
	glutMainLoop();
}
