#include<stdlib.h>
#include<math.h>

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
