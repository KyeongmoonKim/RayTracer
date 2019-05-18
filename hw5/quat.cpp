#include<stdlib.h>
#include"vect.hpp"
#include<math.h>

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
} //4-d vector

double* rotate(double* point, double* center, float* rotV) {
	double qCP[4]; //quaternion of vector c->p
	qCP[0] = 0.0;
	for(int i = 1; i < 4; i++) qCP[i] = point[i-1] - center[i-1];
	double* q = Qexp(rotV);
	double* qI = Qinverse(q);
	double* temp = Qmulti(qCP, qI);
	double* Qnew = Qmulti(q, temp);
	double* ret = (double *)malloc(sizeof(double) * 3);
	for(int i = 0; i < 3; i++) ret[i] = Qnew[i+1] + center[i];
	free(q);
	free(qI);
	free(temp);
	free(Qnew);
	return ret;
	
}//3-d point, 3-d center, 4-d rotation vector (theta, x, y, z)
