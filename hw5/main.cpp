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
double pixels[H][W][3];
double* pixelD(int row, int col);
int color(double* o, double* v, double* rgb); //o : reference point, v : vector, return : rgb vector
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
	double shi;
	//materials after
} Sphere;
typedef struct pol {
	int n; //number of vertex
	double** vertex;
	double amb[3];
	double dif[3];
	double spe[3];
	double shi;
	double normal[3];
	//materials after
} Plane;
double interSphere(double* o, double* u, Sphere* s); //return value is s, because u is normal vector, distance is s
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
			double* rgb = (double *)malloc(sizeof(double)*3);
			int ret = color(refP, v, rgb);
			if(ret == -1) { //background color is black.
				pixels[i][j][0] = 0.0;
				pixels[i][j][1] = 0.0;
				pixels[i][j][2] = 0.0;
			} else {
				for(int k = 0; k < 3; k++) pixels[i][j][k] = rgb[k];
			}
			free(v);
			free(rgb);
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
	d = length(ret, 3);
	for(int i = 0; i < 3; i++) ret[i] = ret[i]/d;
	return ret;
} //normal vector for reference point to pixel.

int color(double* o, double* v, double* rgb) {
	double s = 20000.0;
	int currIdx;
	int type;//0 is sphere, 1 is polygon.
	for(int i = 0; i < sphereNum; i++) {
		double tempS = interSphere(o, v, &(spheres[i]));
		if(tempS < s) {
			s = tempS; 
			currIdx = i;
		}
	}
	/*
		part for polygon.
	*/
	double pInter[3]; //intersected point.
	double shadowRay[lightNum][3]; //lightsource to intersecting point vector.
	for(int i = 0; i < 3; i++) pInter[i] = o[i] + s * v[i];
	for(int i = 0; i < lightNum; i++) {
		for(int j = 0; j < 3; j++) {
			shadowRay[i][j] = pInter[j]-lights[i].center[j];
		}
		double d = length(shadowRay[i], 3);
		for(int j = 0; j < 3; j++) shadowRay[i][j] = shadowRay[i][j] / d;
	}//shadow ray allocation.
	double tempRgb[3]; //rgb by direct lightsource
	double normal[3]; //normal vector of intersecting point
	if(type == 0) {
		for(int i = 0; i < 3; i++) normal[i] = (pInter[i] - spheres[currIdx].center[i])/spheres[i].r;
	} else {
		for(int i = 0; i < 3; i++) normal[i] = planes[currIdx].normal[i];
	}
	for(int i = 0; i < 3; i++) tempRgb[i] = 0.0;
	for(int i = 0; i < lightNum; i++) {
		if(dotProduct(v, shadowRay[i]) > 0.0) {
			for(int j = 0; j < 3; j++) {
				//tempRgb[j] = 
			}
		} //light hit the point. corloring.
	}
}

double interSphere(double* o, double* u, Sphere* s) {
	double deltaP[3];
	for(int i = 0; i < 3; i++) deltaP[i] = s->center[i] - o[i];
	double b = -2.0 * dotProduct(u, deltaP);
	double c = length(deltaP, 3) * length(deltaP, 3) - (s->r) * (s->r);
	double temp = b*b - 4.0 * c;
	if(temp < 0.0) { // no intersection.
		return 20000.0;
	} else if(temp > 0.00000001) { //2 point intersection
		double s1 = (-1.0 * b - sqrt(temp)) / 2.0;
		double s2 = (-1.0 * b + sqrt(temp)) / 2.0;
		double ret = min(s1, s2);
		if(ret < 0.0) return 20000.0; //because o is out of sphere, s1 and s2 both are minus value.
		else return ret; //because o is out of sphere, min value has less distance.
	} else {//1 point intersection.
		double ret = -1.0 * b / 2.0;
		if(ret < 0.0) return 20000.0; //because spehre is behind of o.
		else return ret;
	}
}//caculate intersect point Pi. Let, point Pi = o + s*u, return value is s. hyphothesis : o is out of sphere.
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
	spheres[0].dif[0] = 0.1; spheres[0].dif[1] = 0.35; spheres[0].dif[2] = 0.1;
	spheres[0].spe[0] = 0.45; spheres[0].spe[1] = 0.55; spheres[0].dif[2] = 0.45;
	spheres[0].shi = 41.0;
	//test part for sphere
	planes = (Plane*)malloc(sizeof(Plane)*1);
	planes[0].n = 4;
	planes[0].vertex = (double **)malloc(sizeof(double*)*planes[0].n);
	for(int i = 0; i < planes[0].n; i++) 
		planes[0].vertex[i] = (double *)malloc(sizeof(double)*3);
	planes[0].vertex[0][0] = 50.0; planes[0].vertex[0][1] = 0.0; planes[0].vertex[0][2] = 50.0;
	planes[0].vertex[1][0] = 50.0; planes[0].vertex[1][1] = 0.0; planes[0].vertex[1][2] = -50.0;
	planes[0].vertex[2][0] = -50.0; planes[0].vertex[2][1] = 0.0; planes[0].vertex[2][2] = -50.0;
	planes[0].vertex[3][0] = -50.0; planes[0].vertex[3][1] = 0.0; planes[0].vertex[3][2] = 50.0;
	planes[0].amb[0] = 0.2125; planes[0].amb[1] = 0.1275; planes[0].amb[2] = 0.054;
	planes[0].dif[0] = 0.714; planes[0].dif[1] = 0.4284; planes[0].dif[2] = 0.18144;
	planes[0].spe[0] = 0.393548; planes[0].spe[1] = 0.271906; planes[0].spe[2] = 0.166721;
	planes[0].normal[0] = 0.0; planes[0].normal[1] = 1.0; planes[0].normal[2] = 0.0;
	planes[0].shi = 25.6;
	//test part for polygon
	lights = (Light*)malloc(sizeof(Light)*1);
	lights[0].center[0] = 0.0; lights[0].center[1] = 20.0; lights[0].center[2] = 0.0;
	for(int i = 0; i < 3; i++) {
		lights[0].amb[i] = 0.1;
		lights[0].dif[i] = 1.0;
		lights[0].spe[i] = 0.5;
	}
	//test part for light	
	//part for test.
}
