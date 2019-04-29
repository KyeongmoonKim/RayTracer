#include<GL/glut.h>
#include<math.h>
#include<iostream>
#include<stdlib.h>
#include<string>
#include<fstream>
#include<algorithm>
#include<string.h>
#define PI 3.14159265

using namespace std;

int type; //the type of section
int sectNum = -1; //the number of section
int contNum = -1; //the number of control point
float*** points;
float* scalas;
float** rotats;
float** posits;
//hw3
void parser(string str);
float cPoint(float t, float p0, float p1, float p2, float p3); //catmullRom point
float bPoint(float t, float p0, float p1, float p2, float p3); //bspline point
float* newRv(float t, float* r0, float* r1, float* r2, float* r3);
void bDraw(float** cts, int time);
void cDraw(float** cts, int time);
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
float* Qlog(double *q);
double* Qexp(float *v);
double* Qinverse(double *q);
float** movePv(float** pv, float scala, float* rv, float *tv, int n);

void myDraw() {
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
	glColor3f(0.0, 0.0, 0.0);
	glPushMatrix();
	{//start drawing
		int time = 3;
		float d = 1.0 / (float)time;
		float tv[3];
		float* rv = (float*)malloc(sizeof(float)*4);
		float sv;
		float **pv;
		float **before;
		float **after;
		float **temp;
		int distNum = 5;
		int polyNum = contNum * distNum;
		float dd = 1.0f / (float)distNum;
		float cd = 1.0f / (float)polyNum;
		temp = (float**)malloc(sizeof(float*)*polyNum);
		for(int i = 0; i < polyNum; i++) temp[i] = (float*)malloc(sizeof(float)*3);
		for(int i = 0; i < polyNum; i++) {
			for(int j = 0; j < 3; j++) {
				if(type==0) temp[i][j] = cPoint(dd*(float)(i%distNum), points[1][i/distNum][j], points[1][(i/distNum+1)%contNum][j], points[1][(i/distNum+2)%contNum][j], points[1][(i/distNum+3)%contNum][j]);
				else temp[i][j] = bPoint(dd*(float)(i%distNum), points[1][i/distNum][j], points[1][(i/distNum+1)%contNum][j], points[1][(        i/distNum+2)%contNum][j], points[1][(i/distNum+3)%contNum][j]);
			}
		}
		before = movePv(temp, scalas[1], rotats[1], posits[1], polyNum);
		pv = (float**)malloc(sizeof(float*)*contNum);
		for(int i = 0; i < contNum; i++) pv[i] = (float*)malloc(sizeof(float)*3);
		for(int i = 0; i < sectNum-3; i++) {
			float t = 0.0f;
			for(int j = 0; j <= time; j++) {
				if(j==time) t = 1.0f;
				for(int k=0; k < 3; k++) {
					tv[k] = cPoint(t, posits[i][k], posits[i+1][k], posits[i+2][k], posits[i+3][k]);
				}
				rv = newRv(t, rotats[i], rotats[i+1], rotats[i+2], rotats[i+3]);
				sv = cPoint(t, scalas[i], scalas[i+1], scalas[i+2], scalas[i+3]);
				for(int k=0; k < contNum; k++) {
					for(int l = 0; l < 3; l++) {
						pv[k][l] = cPoint(t, points[i][k][l], points[i+1][k][l], points[i+2][k][l], points[i+3][k][l]);
					}
				}
				for(int k = 0; k < polyNum; k++) {
					for(int l = 0; l < 3; l++) {
						if(type==0) temp[k][l] = cPoint(dd*(float)(k%distNum), pv[k/distNum][l], pv[(k/distNum+1)%contNum][l], pv[(k/distNum+2)%contNum][l], pv[(k/distNum+3)%contNum][l]);
						else temp[k][l] = bPoint(dd*(float)(k%distNum), pv[k/distNum][l], pv[(k/distNum+1)%contNum][l], pv[(k/distNum+2)%contNum][l], pv[(k/distNum+3)%contNum][l]);
					}
				}
				after = movePv(temp, sv, rv, tv, polyNum);
				for(int k = 0; k < polyNum; k++) {
					glColor3f(0.0, 0.0+cd*k, 1.0-cd*k);
					glBegin(GL_TRIANGLES);
						glVertex3fv(before[k]);
						glVertex3fv(before[(k+1)%polyNum]);
						glVertex3fv(after[k]);
					glEnd();
					glBegin(GL_TRIANGLES);
						glVertex3fv(after[(k+1)%polyNum]);
						glVertex3fv(after[k]);
						glVertex3fv(before[(k+1)%polyNum]);
					glEnd();
				}
				/*for(int k = 0; k < polyNum; k++) {
					glColor3f(0.0, 0.0, 0.0);
					glBegin(GL_LINES);
						glVertex3fv(before[k]);
						glVertex3fv(after[k]);
					glEnd();
				}*/
				before = after;
				glPushMatrix();
				{
					glColor3f(0.0, 0.0, 0.0);
					glTranslatef(tv[0], tv[1], tv[2]);
					glRotatef(rv[0]*180.0f / PI, rv[1], rv[2], rv[3]);
					glScalef(sv, sv, sv);
					if(type==0) cDraw(pv, 5);
					else bDraw(pv, 5);
				}
				glPopMatrix();
				t+=d;
			}
		}
	}
	glPopMatrix();
	glutSwapBuffers();
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

float* Qlog(double *q) {
	double sn = sqrt(q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
	//double theta = atan2(sn, q[0]);
	double theta = atan2(sn, q[0]) * 2;
	float* ret = (float*)malloc(sizeof(float)*4);
	if(sn > 0.001) {
		ret[0] = theta;
		for(int i = 1; i < 4; i++) ret[i] = q[i]/sn;
	}
	else for(int i = 0; i < 4; i++) ret[i] = 0.0;
	return ret;
} //4-d vector

double* Qexp(float *v) {
	double theta = v[0];
	double* ret = (double*)malloc(sizeof(double)*4);
	if(theta < 0.001) {
		ret[0] = 1.0;ret[1] = 0.0; ret[2] = 0.0; ret[3] = 0.0;
		return ret;
	}
	else {
		//double sn = sin(theta);
		double sn = sin(theta/2);
		//ret[0] = cos(theta);
		ret[0] = cos(theta/2);
		for(int i = 1; i < 4; i++) ret[i] = sn*(double)v[i];
		return ret;
	}
} //4-d vector

double* Qinverse(double *q) {
	double d = length(q, 4);
	double* ret = (double *)malloc(sizeof(double)*4);
	ret[0] = q[0]/d;
	for(int i = 1; i < 4; i++) ret[i] = -1.0 * q[i]/d;
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


float bPoint(float t, float p0, float p1, float p2, float p3) {
	float t2 = t*t;
	float t3 = t*t*t;
	float b0 = (-1.0f * p0 + 3.0f * p1 - 3.0f * p2 + p3) / 6.0f;
	float b1 = (3.0f * p0 - 6.0f * p1 + 3.0f * p2) / 6.0f;
	float b2 = (-3.0f * p0 + 3.0f * p2) / 6.0f;
	float b3 = (p0 + 4.0f * p1 + p2) / 6.0f;

	return (b3 + t * b2 + t2 * b1 + t3 * b0);
}
float cPoint(float t, float p0, float p1, float p2, float p3) {
	float t2 = t*t;
	float t3 = t*t*t;
	float b0 = p1;
	float b1 = -0.5f*p0 + 0.5f*p2;
	float b2 = p0 - 2.5f*p1 + 2.0f*p2 -0.5f*p3;
	float b3 = -0.5f*p0 + 1.5f*p1 - 1.5f*p2 + 0.5f*p3;
	return (b3 * t3 + b2*t2 + b1*t + b0);
} //bezier + catmullrom

float bezierPoint(float t, float p0, float p1, float p2, float p3) {
	float t2 = t*t;
	float t3 = t*t*t;
	float b0 = -1.0f * p0 + 3.0f * p1 - 3.0f * p2 + p3;
	float b1 = 3.0f * p0 - 6.0f * p1 + 3.0f * p2;
	float b2 = -3.0f * p0 + 3.0f * p1;
	float b3 = p0;
	return (t3 * b0 + t2 * b1 + t* b2 + b3);
}
double* Qcont1(double* q0, double* q1, double *q2) {
	double* Qtemp = Qmulti(Qinverse(q0), q2);
	double* a = Qinverse(q0);
	//cout<<"Qinverse(q0) : "<<a[0]<<", "<<a[1]<<", "<<a[2]<<", "<<a[3]<<endl;
	//cout<<"Qmulti(Qinverse(q0), q2) : "<<Qtemp[0]<<", "<<Qtemp[1]<<", "<<Qtemp[2]<<", "<<Qtemp[3]<<endl;
	float* Vtemp = Qlog(Qtemp);
	//cout<<"Vtemp : "<<Vtemp[0]<<", "<<Vtemp[1]<<", "<<Vtemp[2]<<", "<<Vtemp[3]<<endl;
	Vtemp[0] = Vtemp[0]/6.0f;
	//cout<<Vtemp[0]<<endl;
	Qtemp = Qexp(Vtemp);
	double* ret = Qmulti(q1, Qtemp);
	return ret;
}

double* Qcont2(double* q1, double* q2, double* q3) {
	double* Qtemp = Qmulti(Qinverse(q1), q3);
	float* Vtemp = Qlog(Qtemp);
	Vtemp[0] = Vtemp[0]/6.0f;
	Qtemp = Qexp(Vtemp);
	double* ret = Qmulti(q2, Qinverse(Qtemp));
	return ret;
}

float* newRv(float t, float* r0, float* r1, float* r2, float* r3) {
	float ret;
	double *q0 = Qexp(r0);
	double *q1 = Qexp(r1);
	double *q2 = Qexp(r2);
	double *q3 = Qexp(r3);
	double *cq1 = Qcont1(q0, q1, q2);
	//cout<<"cq1 : "<< cq1[0] <<", "<<cq1[1]<<", "<<cq1[2]<<", "<<cq1[3]<<endl;
	double *cq2 = Qcont2(q1, q2, q3);
	//cout<<"cq2 : "<< cq2[0] <<", "<<cq2[1]<<", "<<cq2[2]<<", "<<cq2[3]<<endl;
	double *cq0 = q1;
	double *cq3 = q2;
	//cout<<"cq0 : "<<cq0[0]<<", "<<cq0[1]<<", "<<cq0[2]<<", "<<cq0[3]<<endl;
	double *qP = (double*)malloc(sizeof(double)*4);
	for(int i = 0; i < 4; i++) qP[i] = bezierPoint(t, cq0[i], cq1[i], cq2[i], cq3[i]);
	//cout<<"bezier quaternion point when "<<t<<" : "<<qP[0]<<", "<<qP[1]<<", "<<qP[2]<<", "<<qP[3]<<", "<<endl;
	return Qlog(qP);
} 
void cDraw(float **cts, int time) {
	float d = (1.0f / (float)time);
	glBegin(GL_LINE_STRIP);
	for(int i = 0; i < contNum; i++) {
		glBegin(GL_LINE_STRIP);
		float t = 0.0f;
		for(int j = 0; j <= time; j++) {
			if(j==time) t= 1.0;
			float x = cPoint(t, cts[i][0], cts[(i+1)%contNum][0], cts[(i+2)%contNum][0], cts[(i+3)%contNum][0]);
			float y = cPoint(t, cts[i][1], cts[(i+1)%contNum][1], cts[(i+2)%contNum][1], cts[(i+3)%contNum][1]);
			float z = cPoint(t, cts[i][2], cts[(i+1)%contNum][2], cts[(i+2)%contNum][2], cts[(i+3)%contNum][2]);
			glVertex3f(x, y, z);
			t+=d;
		}
		glVertex3f(cts[(i+2)%contNum][0], cts[(i+2)%contNum][1], cts[(i+2)%contNum][2]);
		glEnd();
	}
}
void bDraw(float **cts, int time) {
	float d = (1.0f / (float)time);
	for(int i = 0; i < contNum; i++) {
		glBegin(GL_LINE_STRIP);
		float t = 0.0f;
		for(int j = 0; j <=time; j++) {
			if(j== time) t = 1.0;
			float x = bPoint(t, cts[i][0], cts[(i+1)%contNum][0], cts[(i+2)%contNum][0], cts[(i+3)%contNum][0]);
			float y = bPoint(t, cts[i][1], cts[(i+1)%contNum][1], cts[(i+2)%contNum][1], cts[(i+3)%contNum][1]);
			float z = bPoint(t, cts[i][2], cts[(i+1)%contNum][2], cts[(i+2)%contNum][2], cts[(i+3)%contNum][2]);
			glVertex3f(x, y, z);
			t+=d;
		}
		glEnd();
	}
}

float** movePv(float** pv, float scala, float *rv, float *tv, int n) {
	float** ret = (float **)malloc(sizeof(float*)*n);
	for(int i = 0; i < n; i++) ret[i] = (float *)malloc(sizeof(float)*3);
	for(int i = 0; i < n; i++) for(int j = 0; j < 3; j++) ret[i][j] = pv[i][j] * scala;
	double* rq = Qexp(rv);
	double* rqi = Qinverse(rq);
	double* pq = (double *)malloc(sizeof(double)*4);
	for(int i = 0; i < n; i++) {
		pq[0] = 0.0;
		for(int j =1; j < 4; j++) {
			pq[j] = (double)ret[i][j-1];
		}
		double* newQ = Qmulti(rq, Qmulti(pq, rqi));
		for(int j =0; j< 3; j++) {
			ret[i][j] = newQ[j+1] + tv[j];
		}
	}
	return ret;
}
void parser(string str) {
	ifstream inputFile(str.data());
	if(inputFile.fail()) {
		cout << "the file doesn't exist" << endl;
		return;
	}
	while(!inputFile.eof()) {
		string line;
		getline(inputFile, line);
		line.erase(remove(line.begin(), line.end(), ' '), line.end()); //space remove
		line.erase(remove(line.begin(), line.end(), '\t'), line.end()); //tab remove
		line.erase(remove(line.begin(), line.end(), 13), line.end());
		if(line.length() == 0) continue;
		else if(line.at(0) == '#') continue;
		else {
			size_t found = line.find_first_of('#');
			if(found != -1) line = line.substr(0, found);
			if(line.compare("BSPLINE") == 0) type = 0;
			else if(line.compare("CATMULL_ROM") == 0) type = 1;
			else {
				cout<<"spline type error"<<endl;
				return;
			}
			break;
		}
	} //type check
	while(!inputFile.eof()) {
		string line;
		getline(inputFile, line);
		line.erase(remove(line.begin(), line.end(), ' '), line.end());
		line.erase(remove(line.begin(), line.end(), '\t'), line.end());
		line.erase(remove(line.begin(), line.end(), 13), line.end());
		if(line.length() == 0) continue;
		else if(line.at(0) == '#') continue;
		else {
			size_t found = line.find_first_of('#');
			if(found != -1) line = line.substr(0, found);
			if(sectNum == -1) sectNum = stoi(line);
			else if(contNum == -1) contNum = stoi(line);
		}
		if(sectNum != -1 && contNum != -1) break;
	}//sectNum, contNum

	/*
		global variables allocation part will be added
	*/
	points = (float ***)malloc(sizeof(float *)*sectNum);
	for(int i = 0; i < sectNum ; i++) points[i] = (float **)malloc(sizeof(float*)*contNum);
	scalas = (float *)malloc(sizeof(float)*sectNum);
	rotats = (float **)malloc(sizeof(float*)*sectNum);
	posits = (float **)malloc(sizeof(float*)*sectNum);
	for(int i = 0; i < sectNum; i++) {
		for(int j = 0; j < contNum; j++) points[i][j] = (float*)malloc(sizeof(float)*3);
	}
	for(int i = 0; i < sectNum; i++) {
		rotats[i] = (float*)malloc(sizeof(float)*4);
		posits[i] = (float*)malloc(sizeof(float)*3);
	}
	int contCheck = 0;
	int sectCheck = 0;
	while(!inputFile.eof()) {
		string line;
		getline(inputFile, line);
		string lineCp;
		lineCp = line;
		line.erase(remove(line.begin(), line.end(), ' '), line.end());
		line.erase(remove(line.begin(), line.end(), '\t'), line.end());
		line.erase(remove(line.begin(), line.end(), 13), line.end());
		if(line.length() == 0) continue;
		else if(line.at(0) == '#') continue;
		else {
			line = lineCp;
			if(contCheck < contNum) {//contPoint
				size_t found = line.find_first_of(' ');
				float x = stof(line.substr(0, found));
				line = line.substr(found, line.length());
				line.erase(remove(line.begin(), line.end(), ' '), line.end());
				line.erase(remove(line.begin(), line.end(), '\t'), line.end());
				found = line.find_first_of("#");
				if(found == -1) found = line.length();
				float z = stof(line.substr(0, found));
				points[sectCheck][contCheck][0] = x;
				points[sectCheck][contCheck][1] = 0.0f;
				points[sectCheck][contCheck][2] = z;
				contCheck++;
			} else if(contCheck == contNum) {//scala
				line.erase(remove(line.begin(), line.end(), ' '), line.end());
				line.erase(remove(line.begin(), line.end(), '\t'), line.end());
				size_t found = line.find_first_of("#");
				if(found == -1) found = line.length();
				float scala = stof(line.substr(0, found));
				scalas[sectCheck] = scala;
				contCheck++;
			} else if(contCheck == contNum + 1) {//rotation
				for(int i = 0; i < 3; i++) {
					size_t found = line.find_first_of(' ');
					float value = stof(line.substr(0, found));
					line = line.substr(found, line.length());
					rotats[sectCheck][i] = value;
					found = line.find_first_not_of("\t ");
					line = line.substr(found, line.length());
				}
				line.erase(remove(line.begin(), line.end(), ' '), line.end());
				line.erase(remove(line.begin(), line.end(), '\t'), line.end());
				size_t found = line.find_first_of("#");
				if(found == -1) found = line.length();
				rotats[sectCheck][3] = stof(line.substr(0, found));
				contCheck++;
			} else {//position
				for(int i = 0; i < 2; i++) {
					size_t found = line.find_first_of(' ');
					float value = stof(line.substr(0, found));
					line = line.substr(found, line.length());
					posits[sectCheck][i] = value;
					found = line.find_first_not_of("\t ");
					line = line.substr(found, line.length());
				}
				line.erase(remove(line.begin(), line.end(), ' '), line.end());
				line.erase(remove(line.begin(), line.end(), '\t'), line.end());
				size_t found = line.find_first_of("#");
				if(found == -1) found = line.length();
				posits[sectCheck][2] = stof(line.substr(0, found));
				contCheck = 0;
				sectCheck++;
			}
		}
	} //point, scala, rotation, trans
	inputFile.close();
	cout<<"file closed!"<<endl;
	cout<<type<<" "<<sectNum<<" "<<contNum<<endl;
}

int main(int argc, char** argv) {
	cout<<"please enter your file name : ";
	string str;
	cin >> str;
	parser(str);
	cout << type << endl;
	for(int i = 0; i < sectNum; i++) {
		cout << "section #" << i  <<endl;
		for(int j = 0; j < contNum; j++) cout << "point" << j << ": (" << points[i][j][0] <<", "<<points[i][j][1] <<", "<<points[i][j][2] <<")"<<endl;
		cout << "scala : " << scalas[i] <<endl;
		cout << "angle : " << rotats[i][0] << ", axis : ("<<rotats[i][1]<<", "<<rotats[i][2]<<", "<<rotats[i][3]<<")"<<endl;
		cout << "position : ("<<posits[i][0] <<", "<< posits[i][1] <<", "<<posits[i][2]<<")"<<endl;
	}
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
	setView(30.0, 30.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	gluPerspective(45.0, 1.0, 0.1, 400.0);
	glutDisplayFunc(myDraw);
	glutReshapeFunc(reShape);
	glutKeyboardFunc(myKeyboard);
	glutMouseFunc(myMouse);
	glutMotionFunc(myDrag);
	glutMainLoop();
}
