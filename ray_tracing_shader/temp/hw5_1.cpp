#include<stdlib.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include<fstream>
#include<algorithm>
#include<iostream>
#include"vect.hpp"
#include"quat.hpp"
#include<Magick++.h>
//hi
#define PI 3.14159265
#define H 500 //init 250.
#define W 500 //init 250.
#define l 900 // used when calculating light decay.
#define rl 55 // used when caculating reflexion decay
/*
	TOKNOW : for debugging, sphereNum is zero
	TODO : 1. when coding reflection, dist parameter need and return value is vector. when dist is on the preset constant, return values is zero vector.
	
*/
using namespace Magick;
using namespace std;

double refP[] = {20.0, 230.0, 460.0}; //when refP is away from window, zoom : in, reverse is zoom out
double windowCenter[] = {20.0, 75.0, 150.0};
double pixels[H][W][3];
double* pixelD(int row, int col);
double color(double* o, double* v, double* rgb, double dist, int bType, int bIdx); //o : reference point, v : vector, return : rgb vector
void setMyObject(); //str is file name
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
	int isTexture; //flag of texture
	int height; //the number of row
	int width; //the number of col
	double*** texture; //if isTexture == 1, texture replaces dif.
	double nr;
	double alpha;
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
	int isTexture;
	int height;
	int width;
	double*** texture;
	double nr;
	double alpha;
	//materials after
} Plane;
double interSphere(double* o, double* u, Sphere* s); //return value is s, because u is normal vector, distance is s
double interPlane(double* o, double* u, Plane* p); //the same as interSphere.
int crossVect(double* pInter, double* u, double* p0, double* p1, double* normal); //return value : 0(no cross), 1(cross exists).
//crossVect is function for inside-outside test. find crosspoints exists with edge between p0 and p1, and half-line u
void setTexture(string str, int a, int b, int c, int d);
void objParser(string str, double* trans, double scale);
double* sphereTexture(int idx, double* normal);
double* planeTexture(int idx, double* point);
double* calculT(double *L, double* N, double ni, double nr);
double innerSphere(double* o, double* u, Sphere* s);

Light* lights;
Sphere* spheres;
Plane* planes;

double** vertexs;
double** normals;

int lightNum;
int sphereNum;
int planeNum;

//objfile byeonsu
int vertexNum;
int normalNum; 

int planesSize = 256;
int spheresSize = 256;

int vertexsSize = 256;
int normalsSize = 256;
//for debugging
int currRow;
int currCol;

int debugRow= -1;
int debugCol= -1;

int main(int argc, char** argv) {
	InitializeMagick(*argv);
	setMyObject();
	string tfName;
	cout<<"please enter your obj file : ";
	string objName;
	cin>>objName;
	double transV[] = {-110.0, 10.0, 100.0};
	objParser(objName, transV, 20.0);
	cout<<"please enter your texure information file : ";
	cin>>tfName;
	ifstream input2(tfName.data());
	string line;
	getline(input2, line);
	int textureNum = stoi(line);
	for(int i = 0; i < textureNum; i++) {
		string name;
		getline(input2, name);
		getline(input2, line);
		int a = stoi(line);
		getline(input2, line);
		int b = stoi(line);
		getline(input2, line);
		int c = stoi(line);
		getline(input2, line);
		int d = stoi(line);
		setTexture(name, a, b, c, d);
	}
	for(int i = 0; i < H; i++) {
		for(int j = 0; j < W; j++) {
			for(int k = 0; k < 3; k++) pixels[i][j][k] = 0.0;
		}
	}
	for(int i = 0; i < H; i++) {
		for(int j = 0; j < W; j++) {
			//cout<<i << " " <<j<<endl;
			currRow = i;
			currCol = j;
			double* v = pixelD(i, j);
			//cout<<v[0]<<", "<<v[1]<<", "<<v[2]<<endl;
			double* rgb = (double *)malloc(sizeof(double)*3);
			double ret = color(refP, v, rgb, 0.0, -1, -1);
			if(ret < 0.0) { //background color is black.
				pixels[i][j][0] = 0.0;
				pixels[i][j][1] = 0.0;
				pixels[i][j][2] = 0.0;
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
			if(r>255) r = 255;
			if(g>255) g = 255;
			if(b>255) b = 255;
			if(r<0 || g<0 || b<0) {
				cout<<"row, col : " <<i <<", "<<j<<endl;
				cout<<"v : "<<pixelD(i, j)[0]<<", "<<pixelD(i, j)[1]<<pixelD(i, j)[2]<<endl;
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
	if(dist > 2000.0) return -1.0; //so far away.
	double s = 20000.0;
	int currIdx;
	int type = 0;//0 is sphere, 1 is polygon.
	for(int i = 0; i < sphereNum; i++) {
		if(bType==0 && bIdx == i) continue; //pass the reflexed surface
		double tempS = interSphere(o, v, &(spheres[i]));
		if(tempS < s && tempS > 1) {//when the point is common point, regard as the same point
			s = tempS; 
			currIdx = i;
		}
	}
	if(currRow==debugRow&&currCol==debugCol) cout<<"start"<<endl;
	for(int i = 0; i < planeNum; i++) {
		if(currRow==debugRow&&currCol==debugCol) cout<<"i in color : "<<i<<endl;
		if(bType==1 && bIdx == i) continue; //pass the reflexed surface
		double tempS = interPlane(o, v, &(planes[i]));
		if(tempS < s && tempS > 1) {
			//if(i>5) cout<<currRow<<", "<<currCol<<endl;
			if(currRow==debugRow&&currCol==debugCol&&i>5) cout<<"###############################################################################33"<<endl;
			s = tempS;
			currIdx = i;
			type = 1;
		}
	}
	

	if(s == 20000.0) return -1.0; //no intersection :  basecase 

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
	int isTexture;
	if(type == 0) {
		isTexture = spheres[currIdx].isTexture;
		for(int i = 0; i < 3; i++) N[i] = (pInter[i] - spheres[currIdx].center[i])/spheres[currIdx].r;
		amb = spheres[currIdx].amb;
		if(spheres[currIdx].isTexture == 0) dif = spheres[currIdx].dif;
		else dif = sphereTexture(currIdx, N);
		spe = spheres[currIdx].spe;
		shi = spheres[currIdx].shi;
	} else {
		isTexture = planes[currIdx].isTexture;
		for(int i = 0; i < 3; i++) N[i] = planes[currIdx].normal[i];
		amb = planes[currIdx].amb;
		if(planes[currIdx].isTexture == 0) dif = planes[currIdx].dif;
		else dif = planeTexture(currIdx, pInter);
		spe = planes[currIdx].spe;
		shi = planes[currIdx].shi;
	} //informaton allocation

	double V[3];
	for(int i = 0; i < 3; i++) V[i] = -1.0 * v[i]; //reverse vector of v.
	if(dotProduct(N, V) < 0) {//when seeing the back-face.
		//cout<<"backface"<<endl;
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
	for(int i = 0; i < 3; i++) tempRgb[i] = 0.0;
	for(int i = 0; i < lightNum; i++) {
		if(dotProduct(N, L[i]) > 0.0) {//lights is in the direction.
			double tS = 20000.0; //base
			for(int j = 0; j < sphereNum; j++) {
				if(j != currIdx || type==1) {
					double ttS = interSphere(pInter, L[i], &(spheres[j]));
					if(ttS < tS && ttS < d[i] && ttS > 0.1) tS = ttS;
				}
			}
			
			for(int j = 0; j < planeNum; j++) {
				if(j != currIdx || type==0) {
					double ttS = interPlane(pInter, L[i], &(planes[j]));
					if(ttS < tS && ttS < d[i] && ttS > 0.1) tS = ttS;
				}
			}
			if(tS != 20000.0) continue; //light isn't seen. but ts is near zero, same point
			for(int j = 0; j < 3; j++) {
				double wow = d[i]/l;
				if(wow < 1.0)  wow = 1.0;
				tempRgb[j] += lights[i].amb[j]*amb[j]; //ambient
				tempRgb[j] += lights[i].dif[j]*dif[j]*dotProduct(N, L[i]) / wow; //diffuse
				if(dotProduct(R[i], V) > 0.0) {//specular
					double temp = lights[i].spe[j]*spe[j]*pow(dotProduct(R[i], V), shi) / wow;
					tempRgb[j] += temp;
				}
				//if(currRow==211&&currCol==269) cout<<tempRgb[0]<<", "<<tempRgb[1]<<", "<<tempRgb[2]<<endl;
			}//coloring by direct light source
		} //light hit the point. corloring.
	}//rgb is summation. light 0 only has amb
	
	//reflection part.
	//if(currRow==211&&currCol==269) cout<<"before reflecton : "<< pInter[0] <<", "<<pInter[1]<<", "<<pInter[2]<<endl;
	
	double* reflRgb = (double*)malloc(sizeof(double)*3);
	double vNew[3];
	for(int i = 0; i < 3; i++) vNew[i] = 2.0 * dotProduct(N, V) * N[i] - V[i]; //reflected v vector.
	double reflS = color(pInter, vNew, reflRgb, dist+s, type, currIdx); //reflRgb is light from vNew.
	if(reflS > 0.01) { //reflection occured.
		for(int i = 0; i < 3; i++) {
			double temp = reflS / rl; //rl is predefined decay constant
			if(temp < 1.0) temp = 1.0;
			tempRgb[i] += reflRgb[i] * spe[i] / temp;
		}
	}
	free(reflRgb);
	//if(currRow==211&&currCol==269) cout<<"after reflecton : "<< pInter[0] <<", "<<pInter[1]<<", "<<pInter[2]<<endl;
	//refraction part
	
	if(type == 0 && spheres[currIdx].alpha != 1.0) {//sphere, refraction occures two times.
		double *refrRgb = (double*)malloc(sizeof(double)*3);
		double nr = spheres[currIdx].nr;
		double alpha = spheres[currIdx].alpha;
		double* T = calculT(V, N, 1.0, nr); //ni is air, nr is value of object. T is vector of refraction light
		double s2 = innerSphere(pInter, T, &(spheres[currIdx]));
		if(s2 < 20000.0) {
			double pInter2[3]; //second intersection points
			double N2[3]; //
			for(int i = 0; i < 3; i++) pInter2[i] = pInter[i] + T[i] * s2;
			for(int i = 0; i < 3; i++) {
				T[i] = -1.0 * T[i]; //reverse for calculating new T.
				N2[i] = (spheres[currIdx].center[i]-pInter2[i])/spheres[currIdx].r;
				//normal vector of pInter2 to center. (because refraction occurs in sphere).
			}
			double* T2 = calculT(T, N2, nr, 1.0);
			double refrS = color(pInter2, T2, refrRgb, dist+s2+s, 0, currIdx);
			if(refrS > 0.01) {
				for(int i = 0;  i < 3; i++) {
					double temp = refrS / rl;
					if(temp < 1.0) temp = 1.0;
					tempRgb[i] = alpha * tempRgb[i] + (1-alpha) * refrRgb[i];
				}
			}
			free(T2);
		}
		free(refrRgb);
		free(T);
	} 

	if(type == 1 && planes[currIdx].alpha != 1.0) {//plane. refraction occures one times.
		double *refrRgb = (double *)malloc(sizeof(double)*3);
		double nr = planes[currIdx].nr;
		double alpha = planes[currIdx].alpha;
		double* T = calculT(V, N, 1.0, nr);
		double refrS = color(pInter, T, refrRgb, dist+s, 1, currIdx);
		if(refrS > 0.01) {
			for(int i = 0; i < 3; i++) {
				double temp = refrS / rl;
				if(temp < 1.0) temp = 1.0;
				tempRgb[i] = alpha * tempRgb[i] + (1-alpha) * refrRgb[i];
			}
		}
		free(T);
		free(refrRgb);
	}
	for(int i = 0; i < 3; i++) rgb[i] = tempRgb[i];
	return s;
}
double* calculT(double* L, double* N, double ni, double nr) {
	double cos1 = dotProduct(L, N); //because L, and N both normalized.
	double temp = ni/nr;
	double cos2 = sqrt(1- temp*temp*(1-cos1*cos1));
	double* ret = (double*)malloc(sizeof(double)*3);
	for(int i = 0; i < 3; i++) {
		ret[i] = (temp * cos1 - cos2)*N[i] - temp*L[i];
	}
	double d = length(ret, 3);
	for(int i = 0; i < 3; i++) ret[i] = ret[i] / d;
	return ret;
} //return vector of refracton.

double innerSphere(double* o, double *u, Sphere *s) {
	double deltaP[3]; //vector from the point o to the center of sphere.
	for(int i = 0; i < 3; i++) deltaP[i] = s->center[i] - o[i];
	double b = -2.0 * dotProduct(u, deltaP);
	double c = length(deltaP, 3) * length(deltaP, 3) - (s->r) * (s->r);
	double temp = b*b - 4.0 * c;
	if(temp < 0.0) {//no intersecton. original : -0.00001
		cout << "inner sphere no intersection, it's bug!" << endl;
		return 20000.0;
	} else if(temp > 0.0) {//2 pont intersection. original : 0.00001
		double s1 = (-1.0 * b - sqrt(temp)) / 2.0;
		double s2 = (-1.0 * b + sqrt(temp)) / 2.0;
		double ret = max(s1, s2);
		if(ret < 0.0) {
			cout << "two point intersection in innerSphere, but minus value" <<endl;
			return 20000.0;
		}
		else return ret;
	} else {//1 point intersection.
		return 20000.0;
	}
}//functon for refracton. return the point anther point. 

double interSphere(double* o, double* u, Sphere* s) {
	double deltaP[3];
	for(int i = 0; i < 3; i++) deltaP[i] = s->center[i] - o[i];
	double b = -2.0 * dotProduct(u, deltaP);
	double c = length(deltaP, 3) * length(deltaP, 3) - (s->r) * (s->r);
	double temp = b*b - 4.0 * c;
	if(temp < 0.0) { // no intersection. origianl : -0.0000001
		return 20000.0;
	} else if(temp > 0.0) { //2 point intersection origianl : 0.0000001
		double s1 = (-1.0 * b - sqrt(temp)) / 2.0;
		double s2 = (-1.0 * b + sqrt(temp)) / 2.0;
		/*double ret = min(s1, s2);
		if(ret < 0.0) return 20000.0; //because o is out of sphere, s1 and s2 both are minus value.
		else return ret; //because o is out of sphere, min value has less distance.*/
		double m = min(s1, s2);
		double M = max(s1, s2);
		if(M < 0.0) return 20000.0;
		else if(m >= 0.0) return m;
		else return M;
	} else {//1 point intersection.
		double ret = -1.0 * b / 2.0;
		if(ret < 0.0) return 20000.0; //because spehre is behind of o.
		else return ret;
	}
}//caculate intersect point Pi. Let, point Pi = o + s*u, return value is s. hyphothesis : o is out of the sphere.

double interPlane(double* o, double* u, Plane* p) {
	if(currRow==debugRow&&currCol==debugCol) cout<<currRow<<", "<<currCol<<endl;
	if(currRow==debugRow&&currCol==debugCol) cout<<"interPlane"<<endl;
	if(currRow==debugRow&&currCol==debugCol) cout<<"plane"<<endl;
	for(int i = 0; i < p->n; i++) {
		if(currRow==debugRow&&currCol==debugCol) cout<<"vertex "<<i<<" : "<<p->vertex[i][0]<<", "<<p->vertex[i][1]<<", "<<p->vertex[i][2]<<endl;
	}
	if(currRow==debugRow&&currCol==debugCol) cout<<"normal of plane : "<<p->normal[0]<<", "<<p->normal[1]<<", "<<p->normal[2]<<endl;
	if(currRow==debugRow&&currCol==debugCol) cout<<"o : "<< o[0]<<", "<<o[1]<<", "<<o[2]<<endl;
	if(currRow==debugRow&&currCol==debugCol) cout<<"u : "<< u[0]<<", "<<u[1]<<", "<<u[2]<<endl;
	double s;
	if(dotProduct(p->normal, u) < 0.001 && dotProduct(p->normal, u) > -0.001) {//on plane or parrallel to plane
		return 20000.0;
		double temp = dotProduct(p->normal, o) + p->D;
		if(temp < 0.001 && temp > -0.001) s = 0.0;
		else return 20000.0; //no intersection
	} else {
		s = -1.0*(p->D + dotProduct(p->normal, o)) / dotProduct(p->normal, u);
	}
	if(s < 0.0) {
		return 20000.0; //no intersection.
	}
	if(currRow==debugRow&&currCol==debugCol) cout<<"s : "<<s<<endl;
	double pInter[3];
	for(int i = 0; i < 3; i++) pInter[i] = o[i] + s * u[i];
	
	//inside-outside test.
	//int time = 0;
	int crossNum = 0;
	double uPlane[3];
	if(currRow==debugRow&&currCol==debugCol) cout<<"middle point 0 1 : ";
	for(int i = 0; i < 3; i++) {
		if(currRow==debugRow&&currCol==debugCol) cout<<(p->vertex[1][i] + p->vertex[0][i])/2 <<" ";
		uPlane[i] = (p->vertex[1][i] + p->vertex[0][i])/2 - pInter[i];
	}//uPlane : pInter -> mid(vertex[0], vertex[1]) vector.
	if(currRow==debugRow&&currCol==debugCol) cout<<endl;
	double d = length(uPlane, 3);
	for(int i = 0; i < 3; i++) uPlane[i] = uPlane[i]/d;
	for(int i = 1; i < p->n; i++) {//find existence of cross point. orginal : 0 but, it has obviously crossPoint.
		crossNum += crossVect(pInter, uPlane, p->vertex[i], p->vertex[(i+1)%(p->n)], p->normal);
		if(currRow==debugRow&&currCol==debugCol) cout<<"i : "<<i<<endl;
		if(currRow==debugRow&&currCol==debugCol) cout<<"crossNum : "<< crossNum<<endl;
	}
	/*if(crossNum%2==0) {//even is outside. so intersection doesn't exist.
		return 20000.0;
	} else {//odds is inside, so intersection exists.
		return s;
	}*/ //It is mathmatically right but, we think the face is convex.
	if(crossNum>0) return 20000.0;
	else return s;
}//calculate intersection points Pi, Let, Pi = o + s*u, return value is s. hypothesis : o is out of the plane.

int crossVect(double* pInter, double* u, double* p0, double* p1, double *normal) { //pInter and p0, p1 is in the same plane.
	if(currRow==debugRow&&currCol==debugCol) cout<<"crossVect"<<endl;
	if(currRow==debugRow&&currCol==debugCol) cout<<"p0 : "<<p0[0]<<", "<<p0[1]<<", "<<p0[2]<<endl;
	if(currRow==debugRow&&currCol==debugCol) cout<<"p1 : "<<p1[0]<<", "<<p1[1]<<", "<<p1[2]<<endl;
	double v[3];
	for(int i = 0; i < 3; i++) v[i] = p1[i] - p0[i];
	double d = length(v, 3);
	if(currRow==debugRow&&currCol==debugCol) cout<<"d : "<<d<<endl;
	for(int i = 0; i < 3; i++) v[i] = v[i]/d;
	if(currRow==debugRow&&currCol==debugCol) cout<<"v : "<<v[0]<<", "<<v[1]<<", "<<v[2]<<endl;
	double* N = crossProduct(v, normal); //new normal vector, and 
	double len = length(N, 3);
	for(int i = 0; i < 3; i++) N[i] = N[i]/len;
	double D = calculD(p0, N);
	if(currRow==debugRow&&currCol==debugCol) cout<<"N D :"<<N[0]<<", "<<N[1]<<", "<<N[2]<<", "<<D<<endl;
	double s;
	if(dotProduct(N, u) < 0.001 && dotProduct(N, u) > -0.001) {
		//return 0;
		double temp = dotProduct(N, pInter) + D;
		//if(temp == 0.0) s = 0.0;
		if(temp < 0.001 && temp > -0.001) s = 0.0; //on the same plane.
		else return 0;
	} else {
		s = -1.0*(D + dotProduct(N, pInter)) / dotProduct(N, u);
	}
	if(currRow==debugRow&&currCol==debugCol) cout<<"s : "<<s<<endl;
	free(N);
	if(s < 0.0) return 0; //no intersection. because cross point is behind the pInter.
	double pNew[3];
	for(int i = 0; i < 3; i++) pNew[i] = pInter[i] + u[i] * s; //cross Point.
	if(currRow==debugRow&&currCol==debugCol) cout<<"pNew : "<<pNew[0]<<", "<<pNew[1]<<", "<<pNew[2]<<", "<<endl;
	double vNew[3];
	for(int i = 0; i < 3; i++) vNew[i] = pNew[i] - p0[i];
	if(currRow==debugRow&&currCol==debugCol) cout<<"vNew : "<<vNew[0]<<", "<<vNew[1]<<", "<<vNew[2]<<", "<<endl;
	if(dotProduct(vNew, v) < 0.0) return 0; //cross point is behind p0.
	double dNew = length(vNew, 3);
	if(currRow==debugRow&&currCol==debugCol) cout<<"dNdw : "<<dNew<<endl;
	if(dNew <= d+1.0) { //after d is error covering
		return 1;
	} //because of the error of floating point, sum small error
	else return 0;
}//find cross point exists
//because pInter, p0, p1 is on the same plane, vector u and v has 2 case. parallel or crossing.
//so N = crossProduct(v, normal vector of the same plane). and, when get intersection point with plane that has normal vector N
//and pInter+s*u, then the intersection point is on the vector v. because v is intersection line of N plane and normal plane.
//finally, if length of p0 -> pNew is bigger than 0 and shorter than length(v), it has crosspoint.

void setMyObject() {
	sphereNum = 3;
	planeNum = 6;
	lightNum = 1;
	spheres = (Sphere*)malloc(sizeof(Sphere)*256);//replace 10 to sphereNum
	spheres[0].r = 30.0;
	spheres[1].r = 30.0;
	spheres[2].r = 30.0;
	
	spheres[0].center[0] = 0.0; spheres[0].center[1] = 0.0; spheres[0].center[2] = -50.0;
	spheres[1].center[0] = -70.0; spheres[1].center[1] = 0.0; spheres[1].center[2] = 40.0;
	spheres[2].center[0] = 75.0; spheres[2].center[1] = 0.0; spheres[2].center[2] = 55.0;

	spheres[0].amb[0] = 0.0; spheres[0].amb[1] = 0.0; spheres[0].amb[2] = 0.0;
	spheres[0].dif[0] = 0.1; spheres[0]. dif[1] = 0.35; spheres[0].dif[2] = 0.1;
	spheres[0].spe[0] = 0.0; spheres[0].spe[1] = 0.0; spheres[0].spe[2] = 0.0;

	spheres[1].amb[0] = 0.0; spheres[1].amb[1] = 0.0; spheres[1].amb[2] = 0.0;
	spheres[1].dif[0] = 0.35; spheres[1].dif[1] = 0.1; spheres[1].dif[2] = 0.1;
	spheres[1].spe[0] = 0.5; spheres[1].spe[1] = 0.5; spheres[1].spe[2] = 0.5;
	
	spheres[2].amb[0] = 0.0; spheres[2].amb[1] = 0.0; spheres[2].amb[2] = 0.0;
	spheres[2].dif[0] = 0.8; spheres[2].dif[1] = 0.8; spheres[2].dif[2] = 0.8;
	spheres[2].spe[0] = 0.4; spheres[2].spe[1] = 0.4; spheres[2].spe[2] = 0.4;

	spheres[0].shi = 50.0;
	spheres[1].shi = 20.0;
	spheres[2].shi = 80.0;
	
	spheres[0].isTexture = 0;
	spheres[1].isTexture = 0;
	spheres[2].isTexture = 0;
	
	spheres[0].alpha = 1.0;
	spheres[1].alpha = 1.0;
	spheres[2].alpha = 0.3;

	spheres[2].nr = 1.15;
	//test part for sphere
	planes = (Plane*)malloc(sizeof(Plane)*256);//replace 10 to planeNum
	for(int i = 0; i < 6; i++) {
		planes[i].n = 4;
		planes[i].vertex = (double **)malloc(sizeof(double*)*planes[i].n);
		for(int j = 0; j < planes[i].n; j++) {
			planes[i].vertex[j] = (double *)malloc(sizeof(double)*3);
		}
	}

	planes[0].vertex[0][0] = -350.0; planes[0].vertex[0][1] = -30.0; planes[0].vertex[0][2] = -350.0;
	planes[0].vertex[1][0] = -350.0; planes[0].vertex[1][1] = -30.0; planes[0].vertex[1][2] = 350.0;
	planes[0].vertex[2][0] = 350.0; planes[0].vertex[2][1] = -30.0; planes[0].vertex[2][2] = 350.0;
	planes[0].vertex[3][0] = 350.0; planes[0].vertex[3][1] = -30.0; planes[0].vertex[3][2] = -350.0;
	
	planes[1].vertex[0][0] = 90.0; planes[1].vertex[0][1] = -30.0; planes[1].vertex[0][2] = -30.0;
	planes[1].vertex[1][0] = 90.0; planes[1].vertex[1][1] = 100.0; planes[1].vertex[1][2] = -30.0;
	planes[1].vertex[2][0] = 0.0; planes[1].vertex[2][1] = 100.0; planes[1].vertex[2][2] = -120.0;
	planes[1].vertex[3][0] = 0.0; planes[1].vertex[3][1] = -30.0; planes[1].vertex[3][2] = -120.0;
	
	planes[2].vertex[0][0] = -90.0; planes[2].vertex[0][1] = 100.0; planes[2].vertex[0][2] = -30.0;
	planes[2].vertex[1][0] = -90.0; planes[2].vertex[1][1] = -30.0; planes[2].vertex[1][2] = -30.0;
	planes[2].vertex[2][0] = 0.0; planes[2].vertex[2][1] = -30.0; planes[2].vertex[2][2] = -120.0;
	planes[2].vertex[3][0] = 0.0; planes[2].vertex[3][1] = 100.0; planes[2].vertex[3][2] = -120.0;

	planes[3].vertex[0][0] = -350.0; planes[3].vertex[0][1] = 350.0; planes[3].vertex[0][2] = -350.0;
	planes[3].vertex[1][0] = -350.0; planes[3].vertex[1][1] = -30.0; planes[3].vertex[1][2] = -350.0;
	planes[3].vertex[2][0] = 350.0; planes[3].vertex[2][1] = -30.0; planes[3].vertex[2][2] = -350.0;
	planes[3].vertex[3][0] = 350.0; planes[3].vertex[3][1] = 350.0; planes[3].vertex[3][2] = -350.0;

	planes[4].vertex[0][0] = 350.0; planes[4].vertex[0][1] = 350.0; planes[4].vertex[0][2] = -350.0;
	planes[4].vertex[1][0] = 350.0; planes[4].vertex[1][1] = -30.0; planes[4].vertex[1][2] = -350.0;
	planes[4].vertex[2][0] = 350.0; planes[4].vertex[2][1] = -30.0; planes[4].vertex[2][2] = 350.0;
	planes[4].vertex[3][0] = 350.0; planes[4].vertex[3][1] = 350.0; planes[4].vertex[3][2] = 350.0;

	planes[5].vertex[0][0] = -350.0; planes[5].vertex[0][1] = 350.0; planes[5].vertex[0][2] = 350.0;
	planes[5].vertex[1][0] = -350.0; planes[5].vertex[1][1] = -30.0; planes[5].vertex[1][2] = 350.0;
	planes[5].vertex[2][0] = -350.0; planes[5].vertex[2][1] = -30.0; planes[5].vertex[2][2] = -350.0;
	planes[5].vertex[3][0] = -350.0; planes[5].vertex[3][1] = 350.0; planes[5].vertex[3][2] = -350.0;

	planes[0].amb[0] = 0.2125; planes[0].amb[1] = 0.1275; planes[0].amb[2] = 0.054;
	planes[0].dif[0] = 0.714; planes[0].dif[1] = 0.4284; planes[0].dif[2] = 0.18144;
	planes[0].spe[0] = 0.3; planes[0].spe[1] = 0.3; planes[0].spe[2] = 0.3;
	
	planes[1].amb[0] = 0.0; planes[1].amb[1] = 0.0; planes[1].amb[2] = 0.0;//mirror
	planes[1].dif[0] = 0.0; planes[1].dif[1] = 0.0; planes[1].dif[2] = 0.0;//mirror
	planes[1].spe[0] = 1.0; planes[1].spe[1] = 1.0; planes[1].spe[2] = 1.0;//mirror

	planes[2].amb[0] = 0.0; planes[2].amb[1] = 0.0; planes[2].amb[2] = 0.0;//mirror
	planes[2].dif[0] = 0.0; planes[2].dif[1] = 0.0; planes[2].dif[2] = 0.0;//mirror
	planes[2].spe[0] = 1.0; planes[2].spe[1] = 1.0; planes[2].spe[2] = 1.0;//mirror

	for(int i = 3; i < 6; i++) {
		planes[i].amb[0] = 0.0; planes[i].amb[1] = 0.0; planes[i].amb[2] = 0.0;
		planes[i].dif[0] = 0.5; planes[i].dif[1] = 0.5; planes[i].dif[2] = 0.5;
		planes[i].spe[0] = 0.0; planes[i].spe[1] = 0.0; planes[i].spe[2] = 0.0;
	}
	
	planes[0].normal[0] = 0.0; planes[0].normal[1] = 1.0; planes[0].normal[2] = 0.0;
	planes[1].normal[0] = -1.0; planes[1].normal[1] = 0.0; planes[1].normal[2] = 1.0;
	planes[2].normal[0] = 1.0; planes[2].normal[1] = 0.0; planes[2].normal[2] = 1.0;
	planes[3].normal[0] = 0.0; planes[3].normal[1] = 0.0; planes[3].normal[2] = 1.0;
	planes[4].normal[0] = -1.0; planes[4].normal[1] = 0.0; planes[4].normal[2] = 0.0;
	planes[5].normal[0] = 1.0; planes[5].normal[1] = 0.0; planes[5].normal[2] = 0.0;

	double d = length(planes[1].normal, 3);
	for(int i = 0; i < 3; i++) planes[1].normal[i] = planes[1].normal[i] / d;
	d = length(planes[2].normal, 3);
	for(int i = 0; i < 3; i++) planes[2].normal[i] = planes[2].normal[i] / d;

	for(int i = 0; i < 6; i++) planes[i].D = calculD(planes[i].vertex[0], planes[i].normal);

	planes[0].shi = 80.0;
	planes[1].shi = 10.0;
	planes[2].shi = 10.0;
	planes[3].shi = 80.0;
	planes[4].shi = 80.0;
	planes[5].shi = 80.0;

	for(int i = 0; i < 6; i++) {
		planes[i].isTexture = 0;
		planes[i].alpha = 1.0;
	}
	
	//test part for polygon
	lights = (Light*)malloc(sizeof(Light)*10); //replace 10 to lightNum.
	lights[0].center[0] = 10.0; lights[0].center[1] = 700.0; lights[0].center[2] = 0.0;
	lights[1].center[0] = 10.0; lights[1].center[1] = 0.0; lights[1].center[2] = 700.0;
	lights[2].center[0] = -100.0; lights[2].center[1] = 100.0; lights[2].center[2] = 130.0;
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

void setTexture(string str, int a, int b, int c, int d) {//set the texture of the plane or the sphere.
	int type = a;//type
	int idx = b;
	int height = c;
	int width = d;
	double*** rgbs = (double***)malloc(sizeof(double**)*height);
	for(int i = 0; i < height; i++) {
		rgbs[i] = (double**)malloc(sizeof(double*)*width);
		for(int j = 0; j < width; j++) {
			rgbs[i][j] = (double*)malloc(sizeof(double)*3);
		}
	}//memory allocation
	try {
		Image img(str);
		for(int i = 0; i < height; i++) {
			for(int j = 0; j < width; j++) {
				ColorRGB rgb(img.pixelColor(i, j));
				rgbs[i][j][0] = rgb.red();
				rgbs[i][j][1] = rgb.green();
				rgbs[i][j][2] = rgb.blue();
			}
		}
		if(type==0) {
			spheres[idx].isTexture = 1;
			spheres[idx].height = height;
			spheres[idx].width = width;
			spheres[idx].texture = rgbs;
		} else {
			planes[idx].isTexture = 1;
			planes[idx].height = height;
			planes[idx].width = width;
			planes[idx].texture = rgbs;
		}
	}
	catch(Exception & error) {
		cout<<error.what()<<endl;
	}
	return;
}

void objParser(string str, double* trans, double scale) {
	ifstream inputFile(str.data());
	if(inputFile.fail()) {
		cout<<"The object file of the name doesn't exist!"<<endl;
		return;
	}
	int check = 0; //vt checker
	double amb[3];
	double dif[3];
	double spe[3];
	double shi = 98.0;
	//After test, the vectros changes to pointer, and the value is in the mtl file.
	vertexNum = 0;
	normalNum = 0;
	vertexsSize = 256;
	normalsSize = 256;
	vertexs = (double**)malloc(sizeof(double*)*vertexsSize);
	normals = (double**)malloc(sizeof(double*)*normalsSize);
	string line;
	string mtlName;
	while(getline(inputFile, line)) { //read line by line
		string temp = line; //deep Copy
		temp.erase(remove(temp.begin(), temp.end(), ' '), temp.end()); //space removing
		temp.erase(remove(temp.begin(), temp.end(), '\t'), temp.end()); //tab removing
		if(temp.length() == 0) continue; //blank line
		if(temp.at(0)=='#') continue; //remark line
		if(temp.at(0)=='o') continue;
		if(temp.at(0)=='s') continue;
		//meaningful line part start
		size_t found = line.find_first_of(' ');
		string tag = line.substr(0, found);
		//cout<<tag<<endl;
		if(tag.compare("mtllib")==0) {//mtllb tag
			line = line.substr(found+1, line.length()); //tag remove
			found = line.find_first_of('#');
			if(found == string::npos) found = line.length();
			mtlName = line.substr(0, found); //save mtl file name to mtlName
			/*
				part for extract the information from the mtl file.
			*/
		} else if(tag.compare("v")==0) {//vertex tag
			line = line.substr(found+1, line.length()); //tag remove
			found = line.find_first_of('#'); //remark remover
			if(found == string::npos) found = line.length();
			line = line.substr(0, found);
			if(vertexNum == vertexsSize) {//realloc
				vertexsSize = 2*vertexsSize;
				double** tempVertexs = (double**)realloc(vertexs, sizeof(double*)*vertexsSize);
				vertexs = tempVertexs;
			}
			vertexs[vertexNum] = (double *)malloc(sizeof(double)*3); //allocation
			for(int i = 0; i < 3; i++) { //allocaton
				found = line.find_first_of(' ');
				if(found == string::npos) found = line.length();
				vertexs[vertexNum][i] = stod(line.substr(0, found))*scale + trans[i];
				if(found!=line.length()) {
					line = line.substr(found+1, line.length());
				}
			}
			vertexNum++;
		} else if(tag.compare("vn")==0) {
			line = line.substr(found+1, line.length());
			found = line.find_first_of('#');
			if(found == string::npos) found = line.length();
			line = line.substr(0, found);
			if(normalNum == normalsSize) {
				normalsSize = 2*normalsSize;
				double** tempNormals = (double**)realloc(normals, sizeof(double*)*normalsSize);
				normals = tempNormals;
			}
			normals[normalNum] = (double*)malloc(sizeof(double)*3);
			for(int i = 0; i < 3; i++) {
				found = line.find_first_of(' ');
				if(found == string::npos) found = line.length();
				normals[normalNum][i] = stod(line.substr(0, found));
				if(found!=line.length()) {
					line = line.substr(found+1, line.length());
				}
			}
			normalNum++;
		} else if(tag.compare("usemtl")==0) {//set material
			line = line.substr(found+1, line.length());
			found = line.find_first_of('#');
			if(found == string::npos) found = line.length();
			line = line.substr(0, found);
			if(line.compare("Bark")==0) {
				shi = 96.078431;
				amb[0] = 0.0; amb[1] = 0.0; amb[2] = 0.0;
				dif[0] = 0.176206*2.0; dif[1] = 0.051816*2.0; dif[2] = 0.016055*2.0;
				spe[0] = 0.015532; spe[1] = 0.06577; spe[2] = 0.02176;
			} else if(line.compare("Leaves")==0) {
				shi = 96.078431;
				amb[0] = 0.0; amb[1] = 0.0; amb[2] = 0.0;
				dif[0] = 0.02985*3.0; dif[1] = 0.106294*3.0; dif[2] = 0.017666*3.0;
				spe[0] = 0.085514; spe[1] = 0.355271; spe[2] = 0.0748115;
			} else {
				cout << "usemtl error" << endl;
			}
		} else if(tag.compare("vt")==0) {
			check = 1;
		} else if(tag.compare("f")==0) {//part of face
			line = line.substr(found+1, line.length());
			found = line.find_first_of('#');
			if(found == string::npos) found = line.length();
			line = line.substr(0, found);
			if(planeNum == planesSize) {
				planesSize = 2*planesSize;
				Plane* tempPlanes = (Plane*)realloc(planes, sizeof(Plane)*planesSize);
				planes = tempPlanes;
			}
			//some information updated.
			planes[planeNum].shi = shi;
			planes[planeNum].alpha = 1.0;
			planes[planeNum].isTexture = 0;
			for(int i = 0; i < 3; i++) {
				planes[planeNum].dif[i] = dif[i];
				planes[planeNum].spe[i] = spe[i];
				planes[planeNum].amb[i] = amb[i];
			}
			int normalIdx = -1; //normal vector idx
			int tempVertex[30]; //vertex idx array
			int n = 0; //number of vertex in the plane
			for(int i = 0; i < 30; i++) {
				//cout<<i<<endl;
				//cout<<line<<endl;
				found = line.find_first_of(' ');
				if(found == string::npos) found = line.length();
				if(check == 0) {//'v//vn' form
					string chunk = line.substr(0, found); //'v//vn'
					size_t tempFound = chunk.find_first_of('/');
					tempVertex[n] = stoi(chunk.substr(0, tempFound))-1;
					int tempNormal = stoi(chunk.substr(tempFound+2, chunk.length()))-1;
					if(normalIdx == -1) {//first time
						normalIdx = tempNormal;
					} else if(normalIdx != tempNormal) {
						cout<<"normal vector of objParser isn't same"<<endl;
						return;
					}
					n++;
					if(found!=line.length()) {
						line = line.substr(found+1, line.length());
						if(i==29) {
							cout<<"plane vertex has more than 30"<<endl;
							return;
						}
					} else break; //all vertex inserted
				} else {//'v/vt/vn' form
				}
			}
			//cout<<n<<endl;
			planes[planeNum].n = n;
			planes[planeNum].vertex = (double**)malloc(sizeof(double*)*n);
			for(int i = 0; i < n; i++) {
				//cout<<"i2: "<<i<<endl;
				planes[planeNum].vertex[i] = (double*)malloc(sizeof(double)*3);
				for(int j = 0; j < 3; j++) {
					planes[planeNum].vertex[i][j] = vertexs[tempVertex[i]][j];
				}
			}
			for(int i = 0; i < 3; i++) planes[planeNum].normal[i] = normals[normalIdx][i];
			planes[planeNum].D = calculD(planes[planeNum].vertex[0], planes[planeNum].normal);
			planeNum++;
		}
	}
	cout<<vertexNum<<endl;
	cout<<normalNum<<endl;
	cout<<planeNum<<endl;
	for(int i = 0; i < normalNum; i++) {
		free(normals[i]);
	}
	free(normals);
	for(int i = 0; i < vertexNum; i++) {
		free(vertexs[i]);
	}
	free(vertexs);
	return;
	/*Sphere* tempSpheres = (Sphere*)realloc(spheres, sizeof(Sphere) * (2 * spheresSize));
	spheres = tempSpheres;*/ //realloc example
}
double calculD(double* vertex, double* normal) {
	return -1.0 * dotProduct(vertex, normal);
}

double* sphereTexture(int idx, double* normal) {
	double u = 0.5 + atan2(normal[2], normal[0]) / (2 * PI);
	double v = 0.5 - asin(normal[1])/PI;
	int x = (int)(u * (spheres[idx].height-1)); 
	int y = (int)(v * (spheres[idx].width-1));
	double* ret = spheres[idx].texture[x][y];
	return ret;
}

double* planeTexture(int idx, double* point) {//for rectangle
	double uV[3]; //xAxis of plane
	double vV[3]; //yAxis of plane
	for(int i = 0; i < 3; i++) {
		uV[i] = planes[idx].vertex[3][i] - planes[idx].vertex[0][i];
		vV[i] = planes[idx].vertex[1][i] - planes[idx].vertex[0][i];
	}
	double DX = length(uV, 3);  //length of xAxis
	double DY = length(vV, 3);  //length of yAxis
	double pV[3];
	for(int i = 0; i < 3; i++) {
		uV[i] = uV[i] / DX;
		vV[i] = vV[i] / DY;
		pV[i] = point[i] - planes[idx].vertex[0][i];
	}
	double u = dotProduct(pV, uV) / DX; //dotProduct(pV) == length of xAxis of pV
	double v = dotProduct(pV, vV) / DY;
	int x = (int)(u * (planes[idx].height-1));
	int y = (int)(v * (planes[idx].width-1));
	double* ret = planes[idx].texture[x][y];
	return ret;
}
