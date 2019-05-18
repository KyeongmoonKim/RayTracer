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

using namespace std;

int main(int argc, char** argv) {
	double p[3];
	double c[3];
	float vect[4];
	p[0] = 5.0; p[1] = 0.0; p[2] = 0.0;
	c[0] = 3.0; c[1] = 0.0; c[2] = 0.0;
	vect[0] = 1.5707963f, vect[1] = 0.0f; vect[2] = 1.0; vect[3] = 0.0f;
	double* newP = rotate(p, c, vect);
	cout<< newP[0]<<", "<<newP[1]<<", "<<newP[2]<<endl;
	free(newP);
}
