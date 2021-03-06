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
double diff[3];
double ptClick[3];
double factor = 2.0; //using normalize v1
double zoomAngle = 45.0;
double scale = 1.0;
int axisView = 0;
//viewing functions
void moveCameraX(int check);
void moveCameraY(int check);
void moveCameraZ(int check);
double* crossProduct(double *v1, double *v2);
double dotProduct(double *v1, double *v2);
void trackBallXY();
void trackBallZ(int check);
void moveTbCenter(int check);
double length(double *v, int n); //length of n-dimension vector
double* Qmulti(double *q1, double *q2);


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
	if(axisView == 1) {
		double tV[3];
		for(int i = 0; i < 3; i++) tV[i] = p0[i] - pref[i];
		double d = -1.0 * length(tV, 3);
		glBegin(GL_LINES);
			glColor3f(1.0, 0.0, 1.0);
			glVertex3f(0, 0, d);
			glVertex3f(0, 3, d);
			glVertex3f(0, 0, d);
			glVertex3f(3, 0, d); 
		glEnd();
	}
	gluLookAt(p0[0], p0[1], p0[2], pref[0], pref[1], pref[2],  viewUp[0], viewUp[1], viewUp[2]);
	//gluLookAt(p0[0], p0[1], p0[2], pref[0], pref[1], pref[2], 0, 1, 0);
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

void ttemp(double theta) {
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
		ttemp(bodyTheta);
	} 
	if(counter >= 250 && counter < 300) bodyTheta = 20;
	if(counter >= 300 && counter < 480) ttemp(bodyTheta);
	if(counter >= 480 && counter < 510) bodyTheta = 15;
	if(counter >= 510 && counter < 680) ttemp(bodyTheta);
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
	if(counter < 710) glutTimerFunc(timeStep, timer, 0);
}

void reShape(int newWeight, int newHeight) {
	glViewport(0, 0, newWeight, newHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(zoomAngle, (GLfloat)newWeight/newHeight, 0.1, 400.0);
}

void myKeyboard(unsigned char key, int x, int y) {
	switch(key) {
		case 'a': //camera left move
		moveCameraX(-1);
		break;
		case 'w': //camera up move
		moveCameraY(1);
		break;
		case 's': //camera down move
		moveCameraY(-1);
		break;
		case 'd': //camera right move
		moveCameraX(1);
		break;
		case 'j': //camera dolly out
		moveCameraZ(1);
		break;
		case 'k': //camera dolly in
		moveCameraZ(-1);
		break;
		case 'u': //camera zoom in
		if(zoomAngle < 179) zoomAngle += 1.0;
		std::cout<<"zoomAngle : "<<zoomAngle<<std::endl;
		break;
		case 'i': //camera zoom out
		if(zoomAngle > 1) zoomAngle -= 1.0;
		std::cout<<"zoomAngle : "<<zoomAngle<<std::endl;
		break;
		case 'o': //trackball z-axis
		trackBallZ(-1);
		break;
		case 'p': //trackball z-axis
		trackBallZ(1);
		break;
		case 'v':
		if(axisView == 1) axisView = 0;
		else axisView = 1;
		break;
		case 'n':
		moveTbCenter(-1);
		break;
		case 'm':
		moveTbCenter(1);
		break;
		case 'g':
		if(scale > 0.01) scale /= 10.0;
		std::cout<<"scale : "<<scale<<std::endl;
		break;
		case 'h':
		if(scale < 100.0) scale *= 10.0;
		std::cout<<"scale : "<<scale<<std::endl;
		break;
		defalut:
		break;
	}
	glutPostRedisplay();

}

void myDrag(int x, int y) {
	diff[0] = ((double)x - ptClick[0])/factor;
	diff[1] = ((double)y - ptClick[1])/factor;
	diff[2] = 0.0;
	ptClick[0] = (double)x;
	ptClick[1] = (double)y;
	ptClick[2] = 0.0;
	if(diff[0] !=0.0 || diff[1] != 0.0 || diff[2] != 0.0) {
		trackBallXY();
	}
	glutPostRedisplay();
}
void myMouse(int button, int state, int x, int y) {
	switch(button) {
		case GLUT_LEFT_BUTTON:
		if(state==GLUT_DOWN) {
			ptClick[0] = (double)x;
			ptClick[1] = (double)y;
			ptClick[2] = 0.0;
		}
		break;
		case GLUT_RIGHT_BUTTON:
		break;
		default:
		break;
	}
	glutPostRedisplay();
}

void moveCameraX(int check) {
	double vZ[3]; // zaxis in view coordinate
	for(int i = 0; i < 3; i++) vZ[i] = p0[i] - pref[i];
	double *temp = crossProduct(viewUp, vZ); //get x-axis in view coordinate
	double d = length(temp, 3);
	for(int i = 0; i < 3; i++) temp[i] = scale * temp[i] / d;
	for(int i = 0; i < 3; i++) {
		p0[i] += (double)check * temp[i];
		pref[i] += (double)check *temp[i];
	} //translation
}

void moveCameraY(int check) {
	double vZ[3];
	for(int i = 0; i < 3; i++) vZ[i] = p0[i] - pref[i];
	double* vX = crossProduct(viewUp, vZ);
	double dx = length(vX, 3);
	for(int i = 0; i < 3; i++) vX[i] = vX[i] / dx;
	double *temp = crossProduct(vZ, vX); //get y-axis in veiw coordinate
	double d = length(temp, 3);
	for(int i = 0; i < 3; i++) temp[i] = scale * temp[i] / d;
	for(int i = 0; i < 3; i++) {
		p0[i] += (double)check * temp[i];
		pref[i] += (double)check * temp[i];
	}
}

void moveCameraZ(int check) {
	double temp[3];
	for(int i = 0; i < 3; i++) temp[i] = p0[i] - pref[i]; //Z-axis in viewing coordinate
	double d = length(temp, 3); //get distance
	for(int i = 0; i < 3; i++) {
		p0[i] += (double)check * scale * temp[i] / d;
		pref[i] += (double)check * scale *temp[i] / d;
	} //z - axis translation
}

double* crossProduct(double *v1, double *v2) {
	double* ret = (double *)malloc(sizeof(double)*3); 
	ret[0] = v1[1]*v2[2] - v1[2]*v2[1];
	ret[1] = v1[2]*v2[0] - v1[0]*v2[2];
	ret[2] = v1[0]*v2[1] - v1[1]*v2[0];
	return ret;
} //v1 is second finger, v2 is third finger

double dotProduct(double *v1, double *v2) {
	double ret = v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
	return ret;
}

double length(double *v, int n) {
	double ret = 0.0;
	for(int i = 0; i < n; i++) ret += v[i]*v[i];
	return sqrt(ret);
}
void moveTbCenter(int check) {
	double v[3];
	for(int i = 0; i < 3; i++) v[i] = p0[i] - pref[i];
	double d = length(v, 3);
	if(check == -1 && fabs(d - 1.0) < 0.001) return;
	for(int i = 0; i < 3; i++) pref[i] += (double)check * v[i] / d;
}

void trackBallZ(int check) {
	double rotAxis[3]; //axis of the rotation
	double trans[3]; //vector which moves pref to zero-point
	for(int i = 0; i < 3; i++) trans[i] = 0.0 - pref[i];
	double rotAngle = 1.0 * (double)check * PI / 180.0; //the unit of the rotation
	for(int i = 0; i < 3; i++) rotAxis[i] = p0[i]-pref[i];
	double d = length(rotAxis, 3);
	for(int i = 0; i < 3; i++) rotAxis[i] = rotAxis[i]/d; //normalization
	double* xAxis = crossProduct(viewUp, rotAxis);
	d = length(xAxis, 3);
	for(int i = 0; i < 3; i++) xAxis[i] = xAxis[i]/d;
	double* yAxis = crossProduct(rotAxis, xAxis); //the Y-axis of the camera coordinate system
	d = length(yAxis, 3);
	for(int i = 0; i < 3; i++) yAxis[i] = yAxis[i]/d;
	double qQ[4]; //the quaternion of the point that will be rotated.
	qQ[0] = 0.0;
	for(int i = 1; i < 4; i++) qQ[i] = p0[i-1] + yAxis[i-1] + trans[i-1]; //get the point that will be rotated
	//trans vector is added because of the rotation center
	double qRot[4]; //quaternion of the rotation
	double qRotI[4]; //inverse of the rotation quaternion
	qRot[0] = cos(rotAngle / 2.0);
	for(int i = 1; i < 4; i++) qRot[i] = rotAxis[i-1] * sin(rotAngle/2.0);
	d = length(qRot, 4);
	qRotI[0] = qRot[0] / d;
	for(int i = 1; i < 4; i++) qRotI[i] = -1.0 * qRot[i] / d;
	double* qQnew = Qmulti(qRot, Qmulti(qQ, qRotI));
	for(int i = 0; i < 3; i++) viewUp[i] = (qQnew[i+1]-trans[i]) - p0[i]; //qQnew-trans is the point rotated.
	d = length(viewUp, 3);
	for(int i = 0; i < 3; i++) viewUp[i] = viewUp[i] / d;
}

void trackBallXY() {
	double vZ[3]; //Z-axis of the camera
	double trans[3]; //vector that pref to zero-point
	for(int i=0; i<3; i++) vZ[i] = p0[i] - pref[i];
	for(int i=0; i<3; i++) trans[i] = 0.0 - pref[i];
	double *xAxis = crossProduct(viewUp, vZ);
	double dx = length(xAxis, 3);
	for(int i = 0; i < 3; i++) xAxis[i] = xAxis[i]/dx;
	double *yAxis = crossProduct(vZ, xAxis);
	double dy = length(yAxis, 3);
	for(int i = 0; i < 3; i++) yAxis[i] = yAxis[i]/dy;
	double dragedP[3]; //point which moved by dragging
	for(int i = 0; i < 3; i++) {
		dragedP[i] = p0[i] + xAxis[i] * diff[0]  - yAxis[i] * diff[1];
		//yAxis[i] diff be substracted because of the window coordinate system.
	}
	double dragedV[3]; //vector pref to dragedP
	for(int i = 0; i < 3; i++) dragedV[i] = dragedP[i] - pref[i];
	
	//upper section is codes for making dragged point and vector
	double* rotAxis = crossProduct(vZ, dragedV); //rotAxis, and using reference point pref.
	double cs = dotProduct(vZ, dragedV) / (length(vZ,3) * length(dragedV, 3)); //cosin value of the rotation angle
	double sn = length(rotAxis, 3) / (length(vZ, 3) * length(dragedV, 3)); //sin value of the rotation angle
	double rotAngle = -1.0 * atan2(sn, cs);
	double rotLen = length(rotAxis, 3);
	for(int i = 0; i < 3; i++) rotAxis[i] = rotAxis[i] / rotLen; //normalization

	//quaternian part start
	double qRot[4]; //quaternian for rotAxis and rotAngle
	double qP0[4]; //quaternian for p0 //must be changed to unit quaternian.
	double qPviewUp[4]; //quaternian for p0 + viewUp
	double qRotI[4]; //imverse of qRot
	qRot[0] = cos(rotAngle / 2);
	qP0[0] = 0.0;
	qPviewUp[0] = 0.0;
	for(int i = 1; i < 4; i++) {
		qRot[i] = rotAxis[i-1] * sin(rotAngle/2);
		qP0[i] = p0[i-1]+trans[i-1]; //trans vector for the rotation center
		qPviewUp[i] = p0[i-1] + viewUp[i-1]+trans[i-1]; //trans vector for the rotation center
	}
	qRotI[0] = qRot[0] / length(qRot, 4);
	for(int i = 1; i< 4; i++) qRotI[i] = -1.0 * qRot[i] / length(qRot, 4);
	double* q0new = Qmulti(qRot, Qmulti(qP0, qRotI));
	double* qViewUp = Qmulti(qRot, Qmulti(qPviewUp, qRotI));
	for(int i = 0; i < 3; i++) {
		p0[i] = q0new[i+1]-trans[i]; //new p0
		viewUp[i] = qViewUp[i+1] - q0new[i+1]; //vector doesn't need the transvector
	}
	double d = length(viewUp, 3);
	for(int i = 0; i<3; i++) viewUp[i] = viewUp[i]/d;
} //trackball for x-axis and y-axis

double* Qmulti(double *q1, double *q2) {
	double *ret = (double *)malloc(sizeof(double) * 4);
	ret[0] = q1[0]*q2[0] - q1[1]*q2[1] -q1[2]*q2[2] - q1[3]*q2[3];
	ret[1] = q1[0]*q2[1] + q1[1]*q2[0] +q1[2]*q2[3] - q1[3]*q2[2];
	ret[2] = q1[0]*q2[2] + q1[2]*q2[0] +q1[3]*q2[1] - q1[1]*q2[3];
	ret[3] = q1[0]*q2[3] + q1[3]*q2[0] +q1[1]*q2[2] - q1[2]*q2[1];
	return ret;
}


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
	setView(30.0, 30.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	gluPerspective(45.0, 1.0, 0.1, 400.0);
	glutDisplayFunc(human);
	glutReshapeFunc(reShape);
	glutKeyboardFunc(myKeyboard);
	glutMouseFunc(myMouse);
	glutMotionFunc(myDrag);
	glutTimerFunc(timeStep, timer, 0);
	glutMainLoop();
}
