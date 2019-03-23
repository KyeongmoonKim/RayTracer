#include<GL/glut.h>
#include<math.h>
#include<iostream>
#include"myTrans.h"
#define PI 3.14159265
//hiearachical modeling 

double leg1Move;
double leg2Move;
int counter;
double bodyMove;
double bodyMove2;
double bodyMove3;
double ll1Move;
double ll2Move;
double rl1Move;
double rl2Move;
GLfloat ll1v1[3];
GLfloat rl1v1[3];
/*
	STATE : body and arm detach. rotating joint
	TODO : The transform matrix should be added before body.
*/

void myCube(GLfloat sx, GLfloat sy, GLfloat sz, GLfloat r, GLfloat g, GLfloat b, GLfloat len) {
	glPushMatrix();
	glScalef(sx, sy, sz);
	glColor3f(r, g, b);
	glutSolidCube(len);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(len + 0.00001);
	glPopMatrix();
}

void mySphere(GLfloat sx, GLfloat sy, GLfloat sz, GLfloat r, GLfloat g, GLfloat b, GLfloat len) {
	glPushMatrix();
	glScalef(sx, sy, sz);
	glColor3f(r, g, b);
	glutSolidSphere(len, 10, 10);
	glPopMatrix();
}


void diffInit() {
	counter = 1;
	bodyMove = 0.0;
	bodyMove2 = 0.0;
	bodyMove3 = 0.0;
	ll1Move = 0.0;
	rl1Move = 0.0;
}
void human() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(8, 20, 8, 0,20,0, 0,1,0);
	//start making world coordinate
	glColor3f(0.5, 0.5, 0.5);
	glPushMatrix();
	{
		glBegin(GL_POLYGON);
			glVertex3f(500, -8, 500);
			glVertex3f(500, -8, -500);
			glVertex3f(-500, -8, -500);
			glVertex3f(-500, -8, 500);
		glEnd();
		glColor3f(0.88235, 0.94118, 0.85490);
		glutSolidCube(500);
	}
	glPopMatrix();
	glPushMatrix();
	{//human
		glTranslatef(0.0, bodyMove+bodyMove2-bodyMove3, 0.0);
		myCube(1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 2.0);
		//body local cs
		glPushMatrix(); //to head edge
		{
			glTranslatef(0.0, 1.5, 0.0); //from head coordinate to body coordinate
			//head local cs
			myCube(1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0);
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
		glPopMatrix(); //from head node
		glPushMatrix(); //to right shoulder joint
		{//left shoulder 
			glTranslatef(1.0, 0.75, 0.25);
			glPushMatrix();
			{//for arm1
				glRotatef(-1*ll1Move, ll1v1[0], ll1v1[1], ll1v1[2]); //arm1 up-down rotation will be happen
				glTranslatef(0.75, 0.0, -0.25);
				glRotatef(90.0, 0.0, 0.0, 1.0); //transform coordinates
				myCube(0.5, 1.5, 0.5, 1.0, 0.0, 0.0, 1.0); 
				glPushMatrix();
				{//for arm1-2 joint
					glTranslatef(0.0, -0.75, 0.25);
					glPushMatrix();
					{//for arm2
						//glRotatef(90, 0, 0, 1);
						//glRotatef(-90, 1, 0, 0);
						glTranslatef(0.0, -0.75, -0.25);
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
			glTranslatef(-1, 0.75, 0.25);
			glPushMatrix();
			{//for leftArm1
				glRotatef(rl1Move, rl1v1[0], rl1v1[1], rl1v1[2]);
				glTranslatef(-0.75, 0.0, -0.25);
				glRotatef(90.0, 0.0, 0.0, 1.0);//tranform coordinate
				myCube(0.5, 1.5, 0.5, 1.0, 0.0, 0.0, 1.0);
				glPushMatrix();
				{//Left arm 1-2 joint
					glTranslatef(0.0, 0.75, 0.25);
					glPushMatrix();
					{
						//glRotatef(90, 1, 0, 0);
						//glRotatef(-90, -1, 0, 0);
						glTranslatef(0.0, 0.75, -0.25);
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
		{//legjoint
			glTranslatef(0.0, -1.0, 0.0);
			glPushMatrix();
			{//golban
				glTranslatef(0.0, -0.5, 0.0);
				myCube(2.0, 1.0, 2.0, 1.0, 1.0, 0.0, 1.0);
				glPushMatrix();
				{//right-leg1-joint
					glTranslatef(0.5, -0.5, 0.5);
					glRotatef(-1 * leg1Move, 1, 0, 0);
					glPushMatrix();
					{//leg1
						glTranslatef(0.0, -1.5, -0.5);
						myCube(0.5, 3.0, 1, 1.0, 0.0, 0.0, 1.0);
						glPushMatrix();
						{//leg2 joint
							glTranslatef(0.0, -1.5, -0.5);
							glRotatef(leg2Move, 1, 0, 0);
							glPushMatrix();
							{//leg2
								glTranslatef(0.0, -1.5, 0.5);
								myCube(0.5, 3.0, 1, 0.0, 1.0, 0.0, 1.0);
							}
							glPopMatrix();
						}
						glPopMatrix();
					}
					glPopMatrix();
				}
				glPopMatrix();
				glPushMatrix();
				{//left-leg1-joint
					glTranslatef(-0.5, -0.5, 0.5);
					glRotatef(-1 * leg1Move, 1, 0, 0);
					glPushMatrix();
					{//leg1
						glTranslatef(0.0, -1.5, -0.5);
						myCube(0.5, 3.0, 1, 1.0, 0.0, 0.0, 1.0);
						glPushMatrix();
						{//leg2 joint
							glTranslatef(0.0, -1.5, -0.5);
							glRotatef(leg2Move, 1, 0, 0);
							glPushMatrix();
							{//leg2
								glTranslatef(0.0, -1.5, 0.5);
								myCube(0.5, 3.0, 1, 0.0, 1.0, 0.0, 1.0);
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
	if(counter <= 100) {
		leg1Move = counter * 60.0 / 100.0;
		leg2Move = counter * 120.0 / 100.0;
	} else if(counter <= 110) {
		leg1Move = 60.0 - (counter-100) * 60.0 / 10.0;
		leg2Move = 120.0 - (counter-100) * 120.0 / 10.0;
	}
}
void bodyAni(int counter) {
	if(counter <= 100) {
		double theta1 = counter * 60.0 / 100.0;
		double theta2 = counter * 120.0 / 100.0;
		bodyMove = 3 * cos(theta1*PI/180)+sin(theta1*PI/180);
		bodyMove += sin((theta2-theta1)*PI/180)+3*cos((theta2-theta1)*PI/180)-6;
	} else if(counter <= 200) {
		double temp = (double)(counter-100);
		double v0 = 100.0/100.0;
		double g = v0/100.0;
		bodyMove2 = (2 * v0 - g * temp) * temp/2;
	} else if(counter <= 300) {
		double temp = (double)(counter-200);
		double v0 = 100.0/100.0;
		double g = v0/100.0;
		bodyMove3 = g * temp * temp/2;
	}
}
void armAni(int counter) {
	if(counter <= 100) {
		ll1v1[0] = 0.0; ll1v1[1] = 1.0; ll1v1[2] = 0.0;
		rl1v1[0] = 0.0; rl1v1[1] = 1.0; rl1v1[2] = 0.0;
		ll1Move = counter * 90.0 / 100.0;
		rl1Move = counter * 90.0 / 100.0;
	} else if(counter <= 110) {
		ll1Move = 90.0;
		rl1Move = 90.0;
	}
}
unsigned timeStep = 20;
void timer(int unUsed) {
	legAni(counter);
	bodyAni(counter);
	armAni(counter);
	if(counter<=300) counter++;
	glutPostRedisplay();
	glutTimerFunc(timeStep, timer, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(700, 700);
	glutCreateWindow("An Example");

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClearDepth(1.0);
	diffInit();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glMatrixMode(GL_PROJECTION);
	glFrustum(-3, 3, -3, 3, 1, 1000);
	glutDisplayFunc(human);
	glutTimerFunc(timeStep, timer, 0);
	glutMainLoop();
}
