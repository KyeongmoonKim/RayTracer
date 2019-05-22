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
#define H 500 //init 250.
#define W 500 //init 250.
#define l 50 // used when calculating light decay.
/*
	TOKNOW : for debugging, sphereNum is zero
	TODO : 1. when coding reflection, dist parameter need and return value is vector. when dist is on the preset constant, return values is zero vector.
	
*/
using namespace std;
double refP[] = {0.0, 40.0, 200.0}; //when refP is away from window, zoom : in, reverse is zoom out
double windowCenter[] = {0.0, 10.0, 50.0};
double pixels[H][W][3];
double* pixelD(int row, int col);
double color(double* o, double* v, double* rgb, double dist, int bType, int bIdx); //o : reference point, v : vector, return : rgb vector
void setObject(string str); //str is file name
double calculD(double* vertex, double* normal); //normal is normal vector of plane, vertex is on the plane.
int parallel(double* v1, double* v2);
	
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
double interPlane(double* o, double* u, Plane* p); //the same as interSphere.
int crossVect(double* pInter, double* u, double* p0, double* p1, double* normal); //return value : 0(no cross), 1(cross exists).
//crossVect is function for inside-outside test. find crosspoints exists with edge between p0 and p1, and half-line u
Light* lights;
Sphere* spheres;
Plane* planes;
int lightNum;
int sphereNum;
int planeNum;

//for debugging
int currRow;
int currCol;

int main(int argc, char** argv) {
	setObject("test");
	for(int i = 0; i < H; i++) {
		for(int j = 0; j < W; j++) {
			for(int k = 0; k < 3; k++) pixels[i][j][k] = 0.0;
		}
	}
	for(int i = 0; i < H; i++) {
		for(int j = 0; j < W; j++) {
			currRow = i;
			currCol = j;
			double* v = pixelD(i, j);
			double* rgb = (double *)malloc(sizeof(double)*3);
			double ret = color(refP, v, rgb, 0.0, -1, -1);
			if(ret < 0.0) { //background color is black.
				pixels[i][j][0] = 1.0;
				pixels[i][j][1] = 1.0;
				pixels[i][j][2] = 1.0;
			} else {
				for(int k = 0; k < 3; k++) pixels[i][j][k] = rgb[k];// * l / ret;
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
			int r = (int)(pixels[i][j][0] * 255.0);
			int g = (int)(pixels[i][j][1] * 255.0);
			int b = (int)(pixels[i][j][2] * 255.0);
			if(r<0 || g<0 || b<0) {
				cout<<"row, col : " <<i <<", "<<j<<endl;
				cout<<pixels[i][j][0]<<", "<<pixels[i][j][1]<<", "<<pixels[i][j][2]<<endl;
				cout<<r<<" "<<g<<" "<<b<<endl;
				img<<"0 0 0"<<endl;
			} else {
				img <<r<<" "<<g<<" "<<b<<endl;
			}
		}
	}
	return 0;
}

double* pixelD(int row, int col) {
	/*double dy = (double)row - (double)H/2;
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
	return ret;*/
	double* ret = (double*)malloc(sizeof(double)*3);
	double zAxis[3];
	double viewUp[] = {0.0, 1.0, 0.0};
	for(int i = 0; i < 3; i++) {
		ret[i] = windowCenter[i] - refP[i];
		zAxis[i] = refP[i] - windowCenter[i];
	}
	double d = length(zAxis, 3);
	for(int i = 0; i < 3; i++) zAxis[i] = zAxis[i]/d;
	double* xAxis = crossProduct(viewUp, zAxis);
	double* yAxis = crossProduct(zAxis, xAxis);
	double dy = (double)H/2 - row;
	double dx = (double)col - W/2;
	for(int i = 0; i < 3 ; i++) ret[i] += xAxis[i]*dx + yAxis[i] * dy;
	d = length(ret, 3);
	for(int i = 0; i < 3; i++) ret[i] = ret[i]/d;
	free(xAxis);
	free(yAxis);
	return ret;
} //normal vector for reference point to pixel.

double color(double* o, double* v, double* rgb, double dist, int bType, int bIdx) {
	if(dist > 500.0) return -1.0; //so far away.
	double s = 20000.0;
	int currIdx;
	int type = 0;//0 is sphere, 1 is polygon.
	for(int i = 0; i < sphereNum; i++) {
		if(bType==0 && bIdx == i) continue; //pass the reflexed surface
		double tempS = interSphere(o, v, &(spheres[i]));
		if(tempS < s) {
			s = tempS; 
			currIdx = i;
		}
	}
	
	for(int i = 0; i < planeNum; i++) {
		if(bType==1 && bIdx == i) continue; //pass the reflexed surface
		double tempS = interPlane(o, v, &(planes[i]));
		//cout<<"tempS : " << tempS<<endl;
		if(tempS < s) {
			s = tempS;
			currIdx = i;
			type = 1;
		}
	}
	

	if(s == 20000.0) return -1.0; //no intersection :  basecase 
	//if(s < 1000) cout<<"s in the color : "<<s<<endl;

	//intersection exists.
	double pInter[3]; //intersected point.

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
		spe = planes[currIdx].spe;
		shi = planes[currIdx].shi;
		for(int i = 0; i < 3; i++) N[i] = planes[currIdx].normal[i];
	} //informaton allocation
	double V[3];
	for(int i = 0; i < 3; i++) V[i] = -1.0 * v[i]; //reverse vector of v.
	if(dotProduct(N, V) < 0) {//when seeing the back-face.
		cout<<"backface"<<endl;
		for(int i = 0; i < 3; i++) N[i] = -N[i];
	}
	double R[lightNum][3];
	double L[lightNum][3];
	double d[lightNum];
	for(int i = 0; i < lightNum; i++) {
		for(int j = 0; j < 3; j++) {
			L[i][j] = lights[i].center[j] - pInter[j];
		} //intersecting point to light source vector
		d[i] = length(L[i], 3);
		for(int j = 0; j < 3; j++) {
			L[i][j] = L[i][j] / d[i];
		}//normalization.
		for(int j = 0; j < 3; j++) {
			R[i][j] = 2.0 * dotProduct(N, L[i]) * N[j] - L[i][j];
		}
	} //L, R allocation.
	//coloring start
	//if(type==1) cout<<"type 1!"<<endl;
	for(int i = 0; i < 3; i++) tempRgb[i] = 0.0;
	for(int i = 0; i < lightNum; i++) {
		if(dotProduct(N, L[i]) > 0.0) {//lights is in the direction.
			double tS = 20000.0; //base
			for(int j = 0; j < sphereNum; j++) {
				if(j != currIdx || type==1) {
					double ttS = interSphere(pInter, L[i], &(spheres[j]));
					if(ttS < tS && ttS < d[i]) tS = ttS;
				}
			}
			
			for(int j = 0; j < planeNum; j++) {
				if(j != currIdx || type==0) {
					double ttS = interPlane(pInter, L[i], &(planes[j]));
					if(ttS < tS && ttS < d[i]) tS = ttS;
				}
			}
			
			if(tS != 20000.0) continue; //light isn't seen.
			//cout<<"light is seeing"<<endl;
			for(int j = 0; j < 3; j++) {
				double wow = d[i]/l;
				if(wow < 1.0)  wow = 1.0;
				tempRgb[j] += lights[i].amb[j]*amb[j]; //ambient caculation
				tempRgb[j] += lights[i].dif[j]*dif[j]*dotProduct(N, L[i]) / wow;//diffuse caculation
				if(dotProduct(R[i], V) > 0.0) {//after thinking
					double temp = lights[i].spe[j]*spe[j]*pow(dotProduct(R[i], V), shi) / wow;
					tempRgb[j] += temp;
				}
			}
		
		} //light hit the point. corloring.
	}//rgb is summation. light 0 only has amb
	
	//below part is reflection.
	double* reflRgb = (double*)malloc(sizeof(double)*3);
	double vNew[3];
	for(int i = 0; i < 3; i++) vNew[i] = 2.0 * dotProduct(N, V) * N[i] - V[i]; //reflected v vector.
	double reflS = color(pInter, vNew, reflRgb, dist+s, type, currIdx); //reflRgb is light from vNew.
	if(reflS > 0.0) { //reflection occured.
		for(int i = 0; i < 3; i++) {
			double temp = reflS/l;
			if(temp < 1.0) temp = 1.0;
			//tempRgb[i] += reflRgb[i] * dif[i] * dotProduct(N, V) / temp;
			tempRgb[i] += reflRgb[i] * spe[i] / temp;
		}
	}
	free(reflRgb);
	for(int i = 0; i < 3; i++) rgb[i] = tempRgb[i];
	return s;
}

double interSphere(double* o, double* u, Sphere* s) {
	double deltaP[3];
	for(int i = 0; i < 3; i++) deltaP[i] = s->center[i] - o[i];
	double b = -2.0 * dotProduct(u, deltaP);
	double c = length(deltaP, 3) * length(deltaP, 3) - (s->r) * (s->r);
	double temp = b*b - 4.0 * c;
	if(temp < 0.00001) { // no intersection.
		return 20000.0;
	} else if(temp > 0.00001) { //2 point intersection
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
}//caculate intersect point Pi. Let, point Pi = o + s*u, return value is s. hyphothesis : o is out of the sphere.

double interPlane(double* o, double* u, Plane* p) {
	double s = -1.0*(p->D + dotProduct(p->normal, o)) / dotProduct(p->normal, u);
	if(s < 0.0) {
		return 20000.0; //no intersection.
	}
	//cout<<"s in the inter plane : "<< s<<endl;
	double pInter[3];
	for(int i = 0; i < 3; i++) pInter[i] = o[i] + s * u[i];
	//inside-outside test.
	//cout<<"pInter : " <<pInter[0]<<", "<<pInter[1]<<", "<<pInter[2]<<endl;
	int crossNum = 0;
	double uPlane[3];
	for(int i = 0; i < 3; i++) {
		uPlane[i] = (p->vertex[1][i] + p->vertex[0][i])/2;
		uPlane[i] -= pInter[i];
	}//uPlane : pInter -> mid(vertex[0], vertex[1]) vector.
	double d = length(uPlane, 3);
	for(int i = 0; i < 3; i++) uPlane[i] = uPlane[i]/d;
	for(int i = 0; i < p->n; i++) {//find existence of cross point.
		//cout<<"i before crossVect in interPlane : "<<i<<endl; 
		crossNum += crossVect(pInter, uPlane, p->vertex[i], p->vertex[(i+1)%(p->n)], p->normal);
	}
	if(crossNum%2==0) {//even is outside. so intersection doesn't exist.
		return 20000.0;
	} else {//odds is inside, so intersection exists.
		return s;
	}
}//calculate intersection points Pi, Let, Pi = o + s*u, return value is s. hypothesis : o is out of the plane.

int crossVect(double* pInter, double* u, double* p0, double* p1, double *normal) { //pInter and p0, p1 is in the same plane.
	double v[3];
	for(int i = 0; i < 3; i++) v[i] = p1[i] - p0[i];
	double d = length(v, 3);
	for(int i = 0; i < 3; i++) v[i] = v[i]/d;
	double* N = crossProduct(v, normal); //new normal vector, and 
	double D = calculD(p0, N);
	if(parallel(u, v) == 1) {//parallel case
		return 0.0;
	} 
	//not parallel
	double s = -1.0*(D + dotProduct(N, pInter)) / dotProduct(N, u);
	free(N);
	if(s < 0.0) return 0.0; //no intersection. because cross point is behind the pInter.
	double pNew[3];
	for(int i = 0; i < 3; i++) pNew[i] = pInter[i] + u[i] * s; //cross Point.
	double vNew[3];
	for(int i = 0; i < 3; i++) vNew[i] = pNew[i] - p0[i];
	if(dotProduct(vNew, v) < 0.0) return 0.0; //cross point is behind p0.
	double dNew = length(vNew, 3);
	if(dNew <= d) return 1;
	else return 0;
}//find cross point exists
//because pInter, p0, p1 is on the same plane, vector u and v has 2 case. parallel or crossing.
//so N = crossProduct(v, normal vector of the same plane). and, when get intersection point with plane that has normal vector N
//and pInter+s*u, then the intersection point is on the vector v. because v is intersection line of N plane and normal plane.
//finally, if length of p0 -> pNew is bigger than 0 and shorter than length(v), it has crosspoint.

void setObject(string str) {
	sphereNum = 2;
	planeNum = 2;
	lightNum = 2;
	spheres = (Sphere*)malloc(sizeof(Sphere)*10);//replace 10 to sphereNum
	spheres[0].r = 30.0;
	spheres[1].r = 30.0;
	spheres[0].center[0] = 50.0;
	spheres[1].center[0] = -50.0;
	spheres[0].center[1] = 0.0;
	spheres[1].center[1] = 0.0;
	spheres[0].center[2] = 0.0;
	spheres[1].center[2] = 0.0;
	spheres[0].amb[0] = 0.0; spheres[0].amb[1] = 0.0; spheres[0].amb[2] = 0.0;
	spheres[1].amb[0] = 0.0; spheres[1].amb[1] = 0.0; spheres[1].amb[2] = 0.0;
	spheres[0].dif[0] = 0.1; spheres[0].dif[1] = 0.35; spheres[0].dif[2] = 0.1;
	spheres[1].dif[0] = 0.35; spheres[1].dif[1] = 0.1; spheres[1].dif[2] = 0.1;
	spheres[0].spe[0] = 0.3; spheres[0].spe[1] = 0.3; spheres[0].spe[2] = 0.3;
	spheres[1].spe[0] = 0.8; spheres[1].spe[1] = 0.8; spheres[1].spe[2] = 0.8;
	spheres[0].shi = 20.0;
	spheres[1].shi = 20.0;
	//test part for sphere
	planes = (Plane*)malloc(sizeof(Plane)*10);//replace 10 to planeNum
	planes[0].n = 4;
	planes[1].n = 4;
	planes[0].vertex = (double **)malloc(sizeof(double*)*planes[0].n);
	planes[1].vertex = (double **)malloc(sizeof(double*)*planes[1].n);
	for(int i = 0; i < planes[0].n; i++) {
		planes[0].vertex[i] = (double *)malloc(sizeof(double)*3);
		planes[1].vertex[i] = (double *)malloc(sizeof(double)*3);
	}
	planes[0].vertex[0][0] = 200.0; planes[0].vertex[0][1] = -30.0; planes[0].vertex[0][2] = 200.0;
	planes[0].vertex[1][0] = 200.0; planes[0].vertex[1][1] = -30.0; planes[0].vertex[1][2] = -200.0;
	planes[0].vertex[2][0] = -200.0; planes[0].vertex[2][1] = -30.0; planes[0].vertex[2][2] = -200.0;
	planes[0].vertex[3][0] = -200.0; planes[0].vertex[3][1] = -30.0; planes[0].vertex[3][2] = 200.0;
	
	planes[1].vertex[0][0] = 200.0; planes[1].vertex[0][1] = -30.0; planes[1].vertex[0][2] = 200.0;
	planes[1].vertex[1][0] = 200.0; planes[1].vertex[1][1] = 370.0; planes[1].vertex[1][2] = 200.0;
	planes[1].vertex[2][0] = 200.0; planes[1].vertex[2][1] = 370.0; planes[1].vertex[2][2] = -200.0;
	planes[1].vertex[3][0] = 200.0; planes[1].vertex[3][1] = -30.0; planes[1].vertex[3][2] = -200.0;
	
	planes[0].amb[0] = 0.2125; planes[0].amb[1] = 0.1275; planes[0].amb[2] = 0.054;
	planes[0].dif[0] = 0.714; planes[0].dif[1] = 0.4284; planes[0].dif[2] = 0.18144;
	planes[0].spe[0] = 0.3; planes[0].spe[1] = 0.3; planes[0].spe[2] = 0.3;
	
	planes[1].amb[0] = 0.0; planes[1].amb[1] = 0.0; planes[1].amb[2] = 0.0;
	planes[1].dif[0] = 0.3; planes[1].dif[1] = 0.7; planes[1].dif[2] = 0.2;
	planes[1].spe[0] = 0.5; planes[1].spe[2] = 0.5; planes[1].spe[2] = 0.5;

	planes[0].normal[0] = 0.0; planes[0].normal[1] = 1.0; planes[0].normal[2] = 0.0;
	planes[1].normal[0] = -1.0; planes[1].normal[1] = 0.0; planes[1].normal[2] = 0.0;

	planes[0].D = calculD(planes[0].vertex[0], planes[0].normal);
	planes[1].D = calculD(planes[1].vertex[1], planes[1].normal);
	//cout<<planes[0].D<<endl;
	planes[0].shi = 80.0;
	planes[1].shi = 20.0;
	//test part for polygon
	lights = (Light*)malloc(sizeof(Light)*10); //replace 10 to lightNum.
	lights[0].center[0] = 0.0; lights[0].center[1] = 100.0; lights[0].center[2] = 0.0;
	//lights[1].center[0] = 0.0; lights[1].center[1] = 0.0; lights[1].center[2] = 100.0;
	for(int i = 0; i < lightNum; i++) {
		for(int j = 0; j < 3; j++) {
			lights[i].amb[j] = 0.0;
			lights[i].dif[j] = 1.0;
			lights[i].spe[j] = 1.0;
		}
	}
	//test part for light	
	//part for test.
}

double calculD(double* vertex, double* normal) {
	return -1.0 * dotProduct(vertex, normal);
}

int parallel(double* v1, double* v2) {
	double d1 = length(v1, 3);
	double d2 = length(v2, 3);
	for(int i = 0; i < 3; i++) {
		double temp = v1[i]/d1 - v2[i]/d2;
		if(temp < 0.0001 && temp > -0.0001) continue;
		else return 0;
	}
	return 1;
}
