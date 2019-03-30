#include<math.h>
#include<GL/glut.h>
#include<iostream>
#define PA 57.295779

/*(x, y, z) -> (x', y', z') is standard. test is vaiable for z-axis move to x-axis(gimble lock problem)
(x', y', z') -> (x, y, z) case: y rotation first. and z axis becomes x-axis so gimble lock occured.
*/
GLdouble getAlpha(GLdouble vx, GLdouble vy, GLdouble vz, GLint test) {
	if(test == 1) return 0.0;
	GLdouble d = sqrt(vy * vy + vz * vz);
	return atan2(vy / d, vz/d) * PA;
}

GLdouble getBeta(GLdouble vx, GLdouble vy, GLdouble vz, GLint test) {
	if(test == 1) {
		if(vx > 0) return -1 * 90.0;
		else if(vx < 0) return 90.0;
		else std::cout << "get Beta error : vx , vy, vz is all zero" << std::endl; 
	}
	GLdouble d = sqrt(vy * vy + vz * vz);
	return atan2(-1 * vx, d) * PA;
}
