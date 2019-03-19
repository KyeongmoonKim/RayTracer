#include<GL/glut.h>
#include<math.h>

//hiearachical modeling 
GLfloat thetaBody;
GLfloat thetaHS;

//right joint movement

void drawBody() {
	glutWireCube(0.3);
}

void drawHead() {
	glColor3f(0.0, 1.0, 0.0);
	glutWireCube(0.1);
}
void drawArm() {
	glPushMatrix();
	glScalef(1.0, 3.0, 1.0);
	glutWireCube(0.1);
	glPopMatrix();
}

void human() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(-45.0, 1.0, 0.0, 0.0);
	glRotatef(45.0, 0.0, 1.0, 0.0); //for viewing vector (1, 1, 1);
	glColor3f(1.0, 0.0, 0.0);
	
	glPushMatrix(); //to body edge
	drawBody(); //body at zero-point
	
	glPushMatrix(); //to head edge
	glTranslatef(0.0, 0.2, 0.0); //from head coordinate to body coordinate
	drawHead();
	glPopMatrix(); //from head node
	
	glPushMatrix(); //to right shoulder joint
	glTranslatef(0.15, 0.15, 0.0);
        glPushMatrix();//to right arm
	glTranslatef(0.1061, -0.1061, 0.0);
	glRotatef(45.0, 0.0, 0.0, 1.0);
	drawArm();
	glPopMatrix();//from right arm
	glPopMatrix();//from right shulder joint
	glPopMatrix(); //from body node
	glutSwapBuffers();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(700, 700);
	glutCreateWindow("An Example");

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glutDisplayFunc(human);
	glutMainLoop();
}
