#include<GL/glut.h>
#include<math.h>
#define PI 3.14159265

double lleg1Move;
double lleg2Move;
double rleg1Move;
double rleg2Move;
int counter;
double bodyMovex;
double bodyMovez;
double ll1Move;
double ll2Move;
double rl1Move;
double rl2Move;
double wheelMove;
double bodyAngle=-90.0;

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

void makeWheel() {
	glPushMatrix();
	{
		glColor3f(1.0, 0.0, 0.0);
		GLfloat currZ = 2.0;
		GLfloat currY = 0.0;
		GLfloat theta = 2 * PI / 8;
		for(int i = 0; i < 11; i++) {
			GLfloat nextZ = 2.0 * cos(theta*i);
			GLfloat nextY = 2.0 * sin(theta*i);
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
	lleg1Move = 0.0;
	lleg2Move = 0.0;
	rleg1Move = 0.0;
	rleg2Move = 0.0;
	ll1Move = 0.0;
	ll2Move = 0.0;
	rl1Move = 0.0;
	rl2Move = 0.0;
}

void initDiff() {
	counter = 0;
	wheelMove = 0.0;
	bodyMovex = 0.0;
	bodyMovez = 0.0;
	bodyAngle = -90.0;
}
void human() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(50.0, 30.0, 50.0, 0,0,0, 0,1,0);
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
	}
	glPopMatrix();
	glPushMatrix();
	{//human
		glTranslatef(10.4719755+bodyMovex, 14.0, 10.4719755+bodyMovez);
		glRotatef(bodyAngle, 0.0, 1.0, 0.0);
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
			{//for leftarm1
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
		glPopMatrix();
		glPushMatrix();
		{//right shoulder
			glTranslatef(-1.5, 0.5, 0.0);
			joint(0.5);
			glPushMatrix();
			{//for rightArm1
				glRotatef(-1*rl1Move, 0.0, 0.0, 1.0);
				glTranslatef(0.0, -1.25, 0.0);
				//glRotatef(90.0, 0.0, 0.0, 1.0);//tranform coordinate
				myCube(0.5, 1.5, 0.5, 1.0, 0.0, 0.0, 1.0);
				glPushMatrix();
				{//Right rm 1-2 joint
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
		glPushMatrix();
		{//car
			glTranslatef(0.0, -12.0, 0.0);
			myCube(2.0, 1.0, 5.0, 1.0, 1.0, 0.0, 2.0);
			glPushMatrix();
			{//left front wheel
				glTranslatef(2.25, -1.0, 3.5);
				glPushMatrix();
				{
					glRotatef(wheelMove, 1.0, 0.0, 0.0);
					makeWheel();
				}
				glPopMatrix();
			}
			glPopMatrix();
			glPushMatrix();
			{//left back wheel
				glTranslatef(2.25, -1.5, -3.5);
				glPushMatrix();
				{
					glRotatef(wheelMove, 1.0, 0.0, 0.0);
					makeWheel();
				}
				glPopMatrix();
			}
			glPopMatrix();
			glPushMatrix();
			{//right front wheel
				glTranslatef(-2.25, -1.0, 3.5);
				glPushMatrix();
				{
					glRotatef(wheelMove, 1.0, 0.0, 0.0);
					glRotatef(180, 0.0, 1.0, 0.0);
					makeWheel();
				}
				glPopMatrix();
			}
			glPopMatrix();
			glPushMatrix();
			{//right back wheel
				glTranslatef(-2.25, -1.0, -3.5);
				glPushMatrix();
				{
					glRotatef(wheelMove, 1.0, 0.0, 0.0);
					glRotatef(180, 0.0, 1.0, 0.0);
					makeWheel();
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
	if(counter <= 49) {
		lleg1Move += 1.8;
	} else if(counter <= 99) {
		lleg1Move -= 1.8;
	} else if(counter <= 149) {
		rleg1Move += 1.8; 
	} else if(counter <= 199) {
		rleg1Move -= 1.8;
	} 
}
void bodyAni(int counter) {
	if(counter <= 299)
		bodyMovex -= 1.0 * 4.0 * PI / 180; //l = r*theta, move per frame
	else if(counter <= 399)
		bodyAngle -= 0.9;
	else if(counter <= 699)
		bodyMovez -= 1.0 * 4.0 * PI / 180;
	else if(counter <= 799)
		bodyAngle -= 0.9;
	else if(counter <= 1099)
		bodyMovex += 1.0 * 4.0 * PI / 180;
	else if(counter <= 1199)
		bodyAngle -=0.9;
	else if(counter <= 1499)
		bodyMovez += 1.0 * 4.0 * PI / 180;
	else if(counter <= 1599)
		bodyAngle -=0.9;
}
void armAni(int counter) {
	if(counter <= 49) {
		ll1Move += 2.61;
		ll2Move += 0.4;
		rl1Move += 2.61;
		rl2Move += 0.4;
	} else if(counter <= 99) {
		ll1Move -= 2.61;
		ll2Move -= 0.4;
		rl1Move -= 2.61;
		rl2Move -= 0.4;
	} else if(counter <= 149) {
		ll1Move += 2.61;
		ll2Move += 0.4;
		rl1Move += 2.61;
		rl2Move += 0.4;
	} else if(counter <= 199) {
		ll1Move -= 2.61;
		ll2Move -= 0.4;
		rl1Move -= 2.61;
		rl2Move -= 0.4;
	} else if(counter <= 249) {
		ll1Move += 3.6;
		ll2Move += acos(25.0/30.0)*180/(50.0*PI);
		rl1Move += 3.6;
		rl2Move += acos(25.0/30.0)*180/(50.0*PI);
	} else if(counter <= 299) {
		ll1Move -= 3.6;
		ll2Move -= acos(25.0/30.0)*180/(50.0*PI);
		rl1Move -= 3.6;
		rl2Move -= acos(25.0/30.0)*180/(50.0*PI);
	}
}
void whAni(int counter) {
	if(counter<=300) wheelMove += 4.0;
}
unsigned timeStep = 15;
void timer(int unUsed) {
	legAni(counter%400);
	bodyAni(counter);
	armAni(counter%400);
	whAni(counter%400);
	counter++;
	if(counter==1600) initDiff();
	if(counter%400==0) initTemp();
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
