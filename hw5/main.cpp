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
#define H 250 //init 250.
#define W 250 //init 250.

using namespace std;
double refP[] = {100.0, 100.0, 100.0};
double windowCenter[] = {30.0, 30.0, 30.0};
double pixels[H][W][3];
double* pixelD(int row, int col);
int color(double* o, double* v, double* rgb); //o : reference point, v : vector, return : rgb vector
void setObject(string str); //str is file name
double calculD(double* vertex, double* normal); //normal is normal vector of plane, vertex is on the plane.

	
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
	double D;
	//materials after
} Plane;
double interSphere(double* o, double* u, Sphere* s); //return value is s, because u is normal vector, distance is s
double interPlane(double* o, double* u, Plane* p);
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
			for(int k = 0; k < 3; k++) pixels[i][j][k] = 0.0;
		}
	}
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
	
	ofstream img ("test.ppm");
	img << "P3" << endl;
	img << W << " "<< H <<endl;
	img << "255" << endl;
	for(int i = 0; i < H; i++) {
		for(int j = 0; j < W; j++) {
			int r = pixels[i][j][0] * 255;
			int g = pixels[i][j][1] * 255;
			int b = pixels[i][j][2] * 255;
			img <<r<<" "<<g<<" "<<b<<endl;
		}
	}
	return 0;
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
	int type = 0;//0 is sphere, 1 is polygon.
	for(int i = 0; i < sphereNum; i++) {
		double tempS = interSphere(o, v, &(spheres[i]));
		if(tempS < s) {
			s = tempS; 
			currIdx = i;
		}
	}
	/*
	for(int i = 0; i < planeNum; i++) {
		double tempS = interPlane(o, v, &(planes[i]));
		if(tempS < s) {
			s = tempS;
			currIdx = i;
			type = 1;
		}
	}
	*/

	if(s == 20000.0) return -1; //no intersection :  basecase 


	//intersection exists.
	double pInter[3]; //intersected point.
	double shadowRay[lightNum][3]; //lightsource to intersecting point vector.
	for(int i = 0; i < 3; i++) pInter[i] = o[i] + s * v[i];

	//color by direct light source.
	double tempRgb[3]; //rgb by direct lightsource
	double N[3]; //normal vector of intersecting point
	double* amb;
	double* dif;
	double* spe;
	double shi;
	if(type == 0) {
		amb = spheres[currIdx].amb;
		dif = spheres[currIdx].dif;
		spe = spheres[currIdx].spe;
		shi = spheres[currIdx].shi;
		for(int i = 0; i < 3; i++) N[i] = (pInter[i] - spheres[currIdx].center[i])/spheres[currIdx].r;
	} else {
		amb = planes[currIdx].amb;
		dif = planes[currIdx].dif;
		spe = spheres[currIdx].spe;
		shi = spheres[currIdx].shi;
		for(int i = 0; i < 3; i++) N[i] = planes[currIdx].normal[i];
	} //informaton allocation
	double V[3];
	for(int i = 0; i < 3; i++) V[i] = -1.0 * v[i]; //reverse vector of v.
	double R[lightNum][3];
	double L[lightNum][3];
	for(int i = 0; i < lightNum; i++) {
		for(int j = 0; j < 3; j++) {
			L[i][j] = lights[i].center[j] - pInter[j];
		} //intersecting point to light source vector
		double d = length(L[i], 3);
		for(int j = 0; j < 3; j++) {
			L[i][j] = L[i][j] / d;
		}//normalization.
		for(int j = 0; j < 3; j++) {
			R[i][j] = 2.0 * dotProduct(N, L[i]) * N[j] - L[i][j];
		}
	} //L, R allocation.
	//coloring start
	for(int i = 0; i < 3; i++) tempRgb[i] = 0.0;
	for(int i = 0; i < lightNum; i++) {
		if(dotProduct(N, L[i]) > 0.0) {//lights is in the direction.
			double tS = 20000.0; //base
			for(int j = 0; j < sphereNum; j++) {
				if(j != currIdx || type==1) {
					double ttS = interSphere(pInter, L[i], &(spheres[i]));
					if(ttS < tS) tS = ttS;
				}
			}
			/*
			for(int j = 0; j < planeNum; j++) {
				if(j != currIdx || type==0) {
					double ttS = interPlane(pInter, L[i], &(planes[i]));
					if(ttS < ts) tS = ttS;
				}
			}
			*/
			if(tS != 20000.0) break; //light isn't seen.
			//cout<<"light is seeing"<<endl;
			for(int j = 0; j < 3; j++) {
				tempRgb[j] += lights[i].amb[j]*amb[j]; //ambient caculation
				tempRgb[j] += lights[i].dif[j]*dif[j]*dotProduct(N, L[i]); //diffuse caculation
				tempRgb[j] += lights[i].spe[j]*spe[j]*pow(dotProduct(R[i], V), shi);
			}
		} //light hit the point. corloring.
	}//rgb is summation. light 0 only has amb
	
	/*
		part for reflection. 
	*/
	/* Vnew is rotated by rotAxis = crossProduct(V, N).
		double* rgbNew = (double *)malloc(sizeof(double)*3);
		int reflection = color(pInter, vNew, rgbNew);
		//coloring by reflection. and in this case, L = Vnew. R = V.
	*/
	for(int i = 0; i < 3; i++) rgb[i] = tempRgb[i];
	
	return 1;
}

double interSphere(double* o, double* u, Sphere* s) {
	double deltaP[3];
	for(int i = 0; i < 3; i++) deltaP[i] = s->center[i] - o[i];
	double b = -2.0 * dotProduct(u, deltaP);
	double c = length(deltaP, 3) * length(deltaP, 3) - (s->r) * (s->r);
	double temp = b*b - 4.0 * c;
	if(temp < 0.00000001) { // no intersection.
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

double interPlane(double* o, double* u, Plane* p) {
	double s = -1.0*(p->D + dotProduct(p->normal, o)) / dotProduct(p->normal, u);
	if(s < 0.0) return 20000.0;
	double pInter[3];
	for(int i = 0; i < 3; i++) pInter[i] = o[i] + s * u[i];
	//inside-outside test.
}
void setObject(string str) {
	sphereNum = 1;
	planeNum = 1;
	lightNum = 1;
	spheres = (Sphere*)malloc(sizeof(Sphere)*1);
	spheres[0].r = 30.0;
	spheres[0].center[0] = 0.0;
	spheres[0].center[1] = 0.0;
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
	planes[0].vertex[0][0] = 50.0; planes[0].vertex[0][1] = -30.0; planes[0].vertex[0][2] = 50.0;
	planes[0].vertex[1][0] = 50.0; planes[0].vertex[1][1] = -30.0; planes[0].vertex[1][2] = -50.0;
	planes[0].vertex[2][0] = -50.0; planes[0].vertex[2][1] = -30.0; planes[0].vertex[2][2] = -50.0;
	planes[0].vertex[3][0] = -50.0; planes[0].vertex[3][1] = 0.0; planes[0].vertex[3][2] = 50.0;
	planes[0].amb[0] = 0.2125; planes[0].amb[1] = 0.1275; planes[0].amb[2] = 0.054;
	planes[0].dif[0] = 0.714; planes[0].dif[1] = 0.4284; planes[0].dif[2] = 0.18144;
	planes[0].spe[0] = 0.393548; planes[0].spe[1] = 0.271906; planes[0].spe[2] = 0.166721;
	planes[0].normal[0] = 0.0; planes[0].normal[1] = 1.0; planes[0].normal[2] = 0.0;
	planes[0].D = calculD(planes[0].vertex[0], planes[0].normal);
	cout<<planes[0].D<<endl;
	planes[0].shi = 25.6;
	//test part for polygon
	lights = (Light*)malloc(sizeof(Light)*1);
	lights[0].center[0] = 30.0; lights[0].center[1] = 30.0; lights[0].center[2] = 30.0;
	for(int i = 0; i < 3; i++) {
		lights[0].amb[i] = 0.1;
		lights[0].dif[i] = 1.0;
		lights[0].spe[i] = 0.5;
	}
	//test part for light	
	//part for test.
}

double calculD(double* vertex, double* normal) {
	return -1.0 * dotProduct(vertex, normal);
}
