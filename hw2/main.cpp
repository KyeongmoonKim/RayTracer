#include<GL/glut.h>
#include<math.h>
#include<iostream>
#include<stdlib.h>
#define PI 3.14159265

//animation variables
int counter;
double bodyMovex;
double bodyMovez;
double rotX;
double rotZ;
double wheelMove;
double wheelMove2;
double bodyAngle=0.0;
double bodyTheta;
double neckMove;
double handleMove;

//viewing variables
double p0[3]; //gluLookAt zero-point
double pref[3]; //gluLookAt ref-point
double viewUp[3]; //viewUpvector
double v1[3];
double rAxis[3];
double rAngle;
double factor = 70.0; //using normalize v1
double zoomAngle = 45.0;
void moveCameraX(int check);
void moveCameraY(int check);
void moveCameraZ(int check);
double* crossProduct(double *v1, double *v2);

//when camera moves, the ratio of the object change
//when angle changes, the ratio of the objectdoesn't change
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



void initDiff() {
	counter = 0;
	wheelMove = 0.0;
	bodyMovex = 0.0;
	bodyMovez = 0.0;
	bodyAngle = 0.0;
	bodyTheta = 0.0;
}
void human() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(zoomAngle, 1.0, 0.1, 400); //temp zoom in
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(p0[0], p0[1], p0[2], pref[0], pref[1], pref[2],  viewUp[0], viewUp[1], viewUp[2]);
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
		myCube(1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 5.0); //for test
	}
	glPopMatrix();
	glPushMatrix();
	{//human
		glTranslatef(10.0+bodyMovex+rotX, 12.5, 10.0+bodyMovez+rotZ);
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
				glRotatef(30.0, 0.0, 1.0, 0.0); //for handling
				glRotatef(-45.0, 1.0, 0.0, 0.0); //for handling
				glTranslatef(0.0, -1.0, 0.0);
				//glRotatef(90.0, 0.0, 0.0, 1.0); //transform coordinates
				myCube(0.5, 1.0, 0.5, 1.0, 0.0, 0.0, 1.0); 
				glPushMatrix();
				{//for arm1-2 joint
					glTranslatef(0.0, -1.0, 0.0);
					joint(0.5);
					glPushMatrix();
					{//for arm2
						glRotatef(-60.0, 0.0, 1.0, 0.0);//for handling
						glRotatef(-90.0, 1.0, 0.0, 0.0);//for handling
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
				glRotatef(-30.0, 0.0, 1.0, 0.0); //for handle
				glRotatef(-45.0, 1.0, 0.0, 0.0); //for handle
				glTranslatef(0.0, -1.0, 0.0);
				//glRotatef(90.0, 0.0, 0.0, 1.0);//tranform coordinate
				myCube(0.5, 1.0, 0.5, 1.0, 0.0, 0.0, 1.0);
				glPushMatrix();
				{//Right rm 1-2 joint
					glTranslatef(0.0, -1.0, 0.0);
					joint(0.5);
					glPushMatrix();
					{
						glRotatef(60.0, 0.0, 1.0, 0.0);//for handle
						glRotatef(-90.0, 1.0, 0.0, 0.0);//for handle
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
			{//chair
				glTranslatef(0.0, 2.5, 0.0);
				myCube(0.5, 1.0, 0.5, 0.0, 1.0, 1.0, 3);
			}
			glPopMatrix();
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

void temp(double theta) {
	double d = 2.0 * 4.0 * PI / 180.0;
	double currD = sqrt(rotX*rotX + rotZ*rotZ);
	double temp1 = bodyAngle * PI / 180.0;
	double temp2 = bodyAngle * PI / 180.0 + theta * PI / 180.0;
	rotX = currD * sin(temp1) + d * sin(temp2);
	rotZ = currD * cos(temp1) + d * cos(temp2);
	bodyAngle = atan2(rotX, rotZ) * 180 / PI;
}

void bodyAni(int counter) {
	if(counter <= 149) {
		bodyMovez += 2.0 * 4.0 * PI / 180; //l = r*theta, move per frame
	}
	if(counter>=180 && counter < 250) {
		if(counter%5==0) bodyTheta += 1.0; 
		temp(bodyTheta);
	} 
	if(counter >= 250 && counter < 300) bodyTheta = 20;
	if(counter >= 300 && counter < 480) temp(bodyTheta);
	if(counter >= 480 && counter < 510) bodyTheta = 15;
	if(counter >= 510 && counter < 680) temp(bodyTheta);
}

void neckAni(int counter) {
	if(counter >= 150 && counter < 180) neckMove+=2;
	if(counter >= 680 && counter < 710) neckMove-=2;
}
void haAni(int counter) {
	if(counter >= 180 && counter < 250) handleMove -= 2;
	if(counter >= 250 && counter < 300) handleMove -= 3;
	if(counter >= 480 &&counter < 510) handleMove += 3;
}
void whAni(int counter) {
	if(counter <= 149) wheelMove += 4.0;
	if(counter >= 180 && counter < 250) {
		wheelMove2 += 0.2;
		wheelMove += 4.0;
	}
	if(counter >= 250 && counter < 300) wheelMove2 += 0.4;
	if(counter >= 300 && counter < 480) wheelMove += 4.0;
	if(counter >= 480 && counter < 510) wheelMove2 -= 0.3;
	if(counter >= 510 && counter < 680) wheelMove += 4.0;
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
	gluPerspective(zoomAngle, (GLfloat)newWeight/newHeight, 0.1, 400.0);
	factor = newWeight / 10.0;
}

void myKeyboard(unsigned char key, int x, int y) {
	switch(key) {
		case 'a':
		moveCameraX(-1);
		break;
		case 'w':
		moveCameraY(1);
		break;
		case 's':
		moveCameraY(-1);
		break;
		case 'd':
		moveCameraX(1);
		break;
		case 'j':
		moveCameraZ(1);
		break;
		case 'k':
		moveCameraZ(-1);
		break;
		case 'u':
		if(zoomAngle < 179) zoomAngle += 1.0;
		break;
		case 'i':
		if(zoomAngle > 1) zoomAngle -= 1.0;
		break;
		defalut:
		break;
	}

}

void myMouse(int button, int state, int x, int y) {
	switch(button) {
		case GLUT_LEFT_BUTTON:
		if(state==GLUT_DOWN) {
			std::cout<<x <<" "<<y <<std::endl;
			v1[0] = (double)x;
			v1[1] = (double)y;
			v1[2] = 0.0;
		}
		else if(state==GLUT_UP) 
		{
			std::cout<<x << " "<<y<<std::endl;
			v1[0] = ((double)x - v1[0]) / factor;
			v1[1] = ((double)y - v1[1]) / factor;
			v1[2] = 0.0;
			std::cout<<v1[0]<<", "<<v1[1]<<", "<<v1[2]<<std::endl;
		}
		break;
		case GLUT_RIGHT_BUTTON:
		break;
		default:
		break;
	}
}

void moveCameraX(int check) {
	double vZ[3]; // zaxis in view coordinate
	for(int i = 0; i < 3; i++) vZ[i] = p0[i] - pref[i];
	double *temp = crossProduct(viewUp, vZ); //get x-axis in view coordinate
	double d = sqrt(temp[0] * temp[0] + temp[1] * temp[1] + temp[2] * temp[2]);
	for(int i = 0; i < 3; i++) {
		p0[i] += check * temp[i] / d;
		pref[i] += check *temp[i] / d;
	} //translation
}

void moveCameraY(int check) {
	double vZ[3];
	for(int i = 0; i < 3; i++) vZ[i] = p0[i] - pref[i];
	double *temp = crossProduct(vZ, crossProduct(viewUp, vZ)); //get y-axis in veiw coordinate
	double d = sqrt(temp[0] * temp[0] + temp[1] * temp[1] + temp[2] * temp[2]);
	for(int i = 0; i < 3; i++) {
		p0[i] += check * temp[i] / d;
		pref[i] += check * temp[i] /d;
	}
}

void moveCameraZ(int check) {
	double temp[3];
	for(int i = 0; i < 3; i++) temp[i] = p0[i] - pref[i]; //Z-axis in viewing coordinate
	double d = sqrt(temp[0] * temp[0] + temp[1] * temp[1] + temp[2] * temp[2]); //get distance
	for(int i = 0; i < 3; i++) {
		p0[i] += check * temp[i] / d;
		pref[i] += check * temp[i] / d;
	} //z - axis translation
}

double* crossProduct(double *v1, double *v2) {
	double* ret = (double *)malloc(sizeof(double)*3); 
	ret[0] = v1[1]*v2[2] - v1[2]*v2[1];
	ret[1] = v1[2]*v2[0] - v1[0]*v2[2];
	ret[2] = v1[0]*v2[1] - v1[1]*v2[0];
	return ret;
} //v1 is second finger, v2 is third finger
void setView(double x0, double y0, double z0, double xref, double yref, double zref, double x, double y, double z) {
	p0[0] = x0;
	p0[1] = y0;
	p0[2] = z0;
	pref[0] = xref;
	pref[1] = yref;
	pref[2] = zref;
	viewUp[0] = x;
	viewUp[1] = y;
	viewUp[2] = z;
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
	setView(100.0, 35.0, 100.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	gluPerspective(45.0, 1.0, 0.1, 400.0);
	glutDisplayFunc(human);
	glutReshapeFunc(reShape);
	glutKeyboardFunc(myKeyboard);
	glutMouseFunc(myMouse);
	glutTimerFunc(timeStep, timer, 0);
	glutMainLoop();
}
