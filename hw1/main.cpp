#include<GL/glut.h>
#include<math.h>
#include"myTrans.h"
#define PI 3.14159265


int counter;
double bodyMovex;
double bodyMovez;
double ll1MoveX;
double ll1MoveY;
double ll2MoveX;
double ll2MoveY;
double rl1Move;
double rl2Move;
double wheelMove;
double wheelMove2;
double bodyAngle=0.0;
double neckMove;
double handleMove;

void myCube(GLfloat sx, GLfloat sy, GLfloat sz, GLfloat r, GLfloat g, GLfloat b, GLfloat len) {
	glPushMatrix();
	{
		glScalef(sx, sy, sz);
		glColor3f(r, g, b);
		glutSolidCube(len);
		glColor3f(0.0, 0.0, 0.0);
		glutWireCube(len + 0.03);
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

void makeWheel(GLfloat rad) {
	glPushMatrix();
	{
		glColor3f(0.0, 0.0, 0.0);
		GLfloat currZ = rad;
		GLfloat currY = 0.0;
		GLfloat theta = 2 * PI / 8;
		for(int i = 0; i < 11; i++) {
			GLfloat nextZ = rad * cos(theta*i);
			GLfloat nextY = rad * sin(theta*i);
			glBegin(GL_LINES);
				glVertex3f(0.25, currY, currZ);
				glVertex3f(0.25, nextY, nextZ);
				glVertex3f(-0.25, currY, currZ);
				glVertex3f(-0.25, nextY, nextZ);
				glVertex3f(0.25, nextY, nextZ);
				glVertex3f(-0.25, nextY, nextZ);
				glVertex3f(0.25, 0.0, 0.0);
				glVertex3f(0.25, nextZ, nextY);
			glEnd();
			currZ = nextZ;
			currY = nextY;
		}
	}
	glPopMatrix();
}



void initTemp() {
	ll1MoveX = 0.0;
	ll1MoveY = 0.0;
	rl1Move = 0.0;
	rl2Move = 0.0;
}

void initDiff() {
	counter = 0;
	wheelMove = 0.0;
	bodyMovex = 0.0;
	bodyMovez = 0.0;
	bodyAngle = 0.0;
}
void human() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(90.0, 35.0, 90.0, 0.0, 0.0, 0.0,  0,1,0);
	//start making world coordinate
	glColor3f(0.0, 0.0, 0.0);
	glPushMatrix();
	{//for background
		glBegin(GL_LINES);
			glVertex3f(0, 0, 0);
			glVertex3f(0, 0, 500);
			glVertex3f(0, 0, 0);
			glVertex3f(0, 500, 0);
			glVertex3f(0, 0, 0);
			glVertex3f(500, 0, 0);
		glEnd();
	}
	glPopMatrix();
	glPushMatrix();
	{//human
		glTranslatef(30.0+bodyMovex, 12.5, 30.0+bodyMovez);
		glRotatef(bodyAngle, 0.0, 1.0, 0.0);
		myCube(1.0, 1.0, 0.5, 0.0, 0.0, 1.0, 2.0);
		//body local cs
		glPushMatrix(); 
		{//neck
			glTranslatef(0.0, 1.5, 0.0);
			glRotatef(neckMove, 0.0, 1.0, 0.0);
			joint(0.5);
			glPushMatrix();
			{//head
				glTranslatef(0.0, 1.0, 0.0); //from head coordinate to body coordinate
				myCube(1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0);
				glTranslatef(0.0, 0.0, 0.501);
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
			{//for leftarm1
				glRotatef(30.0+ll1MoveY, 0.0, 1.0, 0.0); //for handling, initValue:30.0
				glRotatef(-45.0+ll1MoveX, 1.0, 0.0, 0.0); //for handling, initValue:-45.0
				glTranslatef(0.0, -1.0, 0.0);
				//glRotatef(90.0, 0.0, 0.0, 1.0); //transform coordinates
				myCube(0.5, 1.0, 0.5, 1.0, 0.0, 0.0, 1.0); 
				glPushMatrix();
				{//for arm1-2 joint
					glTranslatef(0.0, -1.0, 0.0);
					joint(0.5);
					glPushMatrix();
					{//for arm2
						glRotatef(-60.0+ll2MoveY, 0.0, 1.0, 0.0);//for handling, initValue:-60.0
						glRotatef(-90.0+ll2MoveX, 1.0, 0.0, 0.0);//for handling, initValue:-90.0
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
		{//right shoulder
			glTranslatef(-1.5, 0.5, 0.0);
			joint(0.5);
			glPushMatrix();
			{//for rightArm1
				glRotatef(-30.0, 0.0, 1.0, 0.0); //for handle, initValue:-30.0
				glRotatef(-45.0, 1.0, 0.0, 0.0); //for handle, initValue:-45.0
				glTranslatef(0.0, -1.0, 0.0);
				//glRotatef(90.0, 0.0, 0.0, 1.0);//tranform coordinate
				myCube(0.5, 1.0, 0.5, 1.0, 0.0, 0.0, 1.0);
				glPushMatrix();
				{//Right rm 1-2 joint
					glTranslatef(0.0, -1.0, 0.0);
					joint(0.5);
					glPushMatrix();
					{
						glRotatef(60.0, 0.0, 1.0, 0.0);//for handle,initValue:60.0
						glRotatef(-90.0, 1.0, 0.0, 0.0);//for handle,initValue:-90.0
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
		{//golbanjoint
			glTranslatef(0.0, -1.5, 0.0);
			joint(0.5);
			glPushMatrix();
			{//golban
				glTranslatef(0.0, -1.0, 0.0);
				glRotatef(-90.0, 1.0, 0.0, 0.0);
				myCube(2.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0);
				glPushMatrix();
				{//left-leg1-joint
					glTranslatef(0.5, -1.0, 0.0);
					joint(0.5);
					glRotatef(45.0, 0.0, 0.0, 1.0);
					glPushMatrix();
					{//leg1
						glTranslatef(0.0, -1.5, 0.0);
						myCube(0.5, 2.0, 0.5, 1.0, 0.0, 0.0, 1.0);
						glPushMatrix();
						{//leg2 joint
							glTranslatef(0.0, -1.5, 0.0);
							joint(0.5);
							glRotatef(90.0, 1, 0, 0);
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
					glRotatef(-45.0, 0.0, 0.0, 1.0);
					glPushMatrix();
					{//leg1
						glTranslatef(0.0, -1.5, 0.0);
						myCube(0.5, 2.0, 0.5, 1.0, 0.0, 0.0, 1.0);
						glPushMatrix();
						{//leg2 joint
							glTranslatef(0.0, -1.5, 0.0);
							joint(0.5);
							glRotatef(90.0, 1, 0, 0);
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
		glPushMatrix();
		{//car
			glTranslatef(0.0, -7.0, 0.0);
			myCube(4.0, 1.0, 5.0, 1.0, 1.0, 0.0, 2.0);
			glPushMatrix();
			{//handle pillar
				glTranslatef(0.0, 4.0, 3.0);
				myCube(0.5, 6.0, 0.5, 0.0, 1.0, 0.0, 1.0);
				glPushMatrix();
				{//handle joint
					glTranslatef(0.0, 3.5, 0.0);
					joint(0.5);
					glRotatef(handleMove, 0.0, 0.0, 1.0);
					glPushMatrix();
					{
						glTranslatef(0.0, 0.0, -0.5);
						glRotatef(90.0, 0.0, 1.0, 0.0);
						makeWheel(1.0);
					}
					glPopMatrix();
				}
				glPopMatrix();
			}
			glPopMatrix();
			glPushMatrix();
			{//front pillar
				glTranslatef(0.0, -1.25, 3.5);
				myCube(12.0, 0.5, 0.5, 1.0, 0.0, 0.0, 1.0);
				glPushMatrix();
				{//left front wheel joint
					glTranslatef(6.5, 0.0, 0.0);
					joint(0.5);
					glPushMatrix();
					{//left front wheel
						glTranslatef(0.75, 0.0, 0.0);
						glRotatef(wheelMove2, 0.0, 1.0, 0.0);
						glPushMatrix();
						{
							glRotatef(wheelMove, 1.0, 0.0, 0.0);
							makeWheel(2.0);
						}
						glPopMatrix();
					}
					glPopMatrix();
				}
				glPopMatrix();
				glPushMatrix();
				{//right front wheel joint
					glTranslatef(-6.5, 0.0, 0.0);
					joint(0.5);
					glPushMatrix();
					{//right front wheel
						glTranslatef(-0.75, 0.0, 0.0);
						glRotatef(wheelMove2, 0.0, 1.0, 0.0);
						glPushMatrix();
						{
							glRotatef(wheelMove, 1.0, 0.0, 0.0);
							glRotatef(180, 0.0, 1.0, 0.0);
							makeWheel(2.0);
						}
						glPopMatrix();
					}
					glPopMatrix();
				}
				glPopMatrix();
			}
			glPopMatrix();
			glPushMatrix();
			{//back pillar
				glTranslatef(0.0, -1.25, -3.5);
				myCube(12.0, 0.5, 0.5, 1.0, 0.0, 0.0, 1.0);
				glPushMatrix();
				{//left back wheel
					glTranslatef(6.25, 0.0, 0.0);
					glPushMatrix();
					{
						glRotatef(wheelMove, 1.0, 0.0, 0.0);
						makeWheel(2.0);
					}
					glPopMatrix();
				}
				glPopMatrix();
				glPushMatrix();
				{//right back wheel
					glTranslatef(-6.25, 0.0, 0.5);
					glPushMatrix();
					{
						glRotatef(wheelMove, 1.0, 0.0, 0.0);
						glRotatef(180, 0.0, 1.0, 0.0);
						makeWheel(2.0);
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


void bodyAni(int counter) {
	if(counter <= 149)
		bodyMovez += 1.0 * 4.0 * PI / 180; //l = r*theta, move per frame
	//else if(counter <= 399)
	//	bodyAngle -= 0.9;
}
void armAni(int counter) {
	if(counter <= 50&& counter >= 0) {
		ll1MoveX -= 0.5;
		ll1MoveY -= 0.5;
		ll2MoveX -= 0.15;
		ll2MoveY += 0.3;
	}
}
void neckAni(int counter) {
	if(counter >= 150 && counter <= 179) {
		neckMove+=3;
	}
}
void haAni(int counter) {
	if(counter >= 180 && counter <= 249) {
		handleMove -= 3;
	}
}
void whAni(int counter) {
	if(counter <= 149) wheelMove += 4.0;
	if(counter >= 180 && counter <= 249) wheelMove2 += 0.75; 
}
unsigned timeStep = 15;
void timer(int unUsed) {
	bodyAni(counter);
	whAni(counter);
	neckAni(counter);
	haAni(counter);
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
	initDiff();
	initTemp();
	gluPerspective(45.0, 1.0, 0.1, 400.0);
	glutDisplayFunc(human);
	glutReshapeFunc(reShape);
	glutTimerFunc(timeStep, timer, 0);
	glutMainLoop();
}
