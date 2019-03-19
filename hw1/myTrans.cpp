#include<math.h>
#include<GL/glut.h>
#define PA 57.295779

//(x, y, z) -> (x', y', z') is standard
GLdouble getAlpha(GLdouble vx, GLdouble vy, GLdouble vz) {
	GLdouble d = sqrt(vy * vy + vz * vz);
	return atan2(vy / d, vz/d) * PA;
}

GLdouble getBeta(GLdouble vx, GLdouble vy, GLdouble vz) {
	GLdouble d = sqrt(vy * vy + vz * vz);
	return atan2(-1 * vx, d) * PA;
}
