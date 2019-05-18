#include<stdlib.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include<fstream>
#include<algorithm>
#include<iostream>
#include"vect.hpp"
#include"quat.hpp"
#define PI 3.14159265
#define H 250
#define W 250

using namespace std;
double refP[] = {10.0, 10.0, 10.0};
double windowCenter[] = {5.0, 5.0, 5.0};
double pixels[H][W];
double* pixelD(int row, int col);
double color(double* o, double* v); //o : reference point, v : vector
void setObject(string str); //str is file name

	
typedef struct lig {
	double center[3];
	double amb[3];
	double dif[3];
	double spe[3];
} Light;
typedef struct sph {
	double center[3];
	double r;
	double amb[3];
	double dif[3];
	double spe[3];
	//materials after
} Sphere;
typedef struct pol {
	int n; //number of vertex
	double** vertexs;
	double amb[3];
	double dif[3];
	double spe[3];
	//materials after
} Plane;

Light* lights;
Sphere* spheres;
Plane* planes;
int lightNum;
int sphereNum;
int planeNum;

int main(int argc, char** argv) {
	setObject("test");
	for(int i = 0; i < H; i++) {
		for(int j = 0; j < W; j++) {
			double* v = pixelD(i, j);
			pixels[i][j] = color(refP, v);
			free(v);
		}
	}
}

double* pixelD(int row, int col) {
	double dy = (double)row - (double)H/2;
	double dx = (double)col - (double)W/2;
	double* ret = (double *)malloc(sizeof(double)*3);
	double zAxis[3];
	for(int i = 0; i < 3; i++) {
		zAxis[i] = windowCenter[i] - refP[i];
		ret[i] = zAxis[i];
	}
	double d = length(zAxis, 3);
	for(int i = 0; i < 3; i++) zAxis[i] = zAxis[i]/d;
	double viewUp[3];
	viewUp[0] = 0.0; viewUp[1] = 1.0; viewUp[2] = 0.0;
	double* xAxis = crossProduct(viewUp, zAxis);
	double* yAxis = crossProduct(zAxis, xAxis);
	for(int i = 0; i < 3; i++) ret[i] += xAxis[i] * dx * -1.0 + yAxis[i] * dy * -1.0;
	free(xAxis);
	free(yAxis);
	return ret;
} //vector for reference point to pixel.

double color(double* o, double* v) {
}

void setObject(string str) {
	sphereNum = 1;
	planeNum = 1;
	lightNum = 1;
	spheres = (Sphere*)malloc(sizeof(Sphere)*1);
	spheres[0].r = 5.0;
	spheres[0].center[0] = -10.0;
	spheres[0].center[1] = 10.0;
	spheres[0].center[2] = 0.0;
	spheres[0].amb[0] = 0.0; spheres[0].amb[1] = 0.0; spheres[0].amb[2] = 0.0;
	planes = (Plane*)malloc(sizeof(Plane)*1);
	lights = (Light*)malloc(sizeof(Light)*1);
	//part for test.
}
