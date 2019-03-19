#include<GL/glut.h>
#include<math.h>

//hiearachical modeling 
GLfloat thetaBody;
GLfloat thetaHS;
//It doesn't display anything because the objtect is so far away
//for animation, 

void drawBody() {
	glutWireCube(3);
}

void drawHead() {
	glColor3f(0.0, 1.0, 0.0);
	glutWireCube(1);
}
void drawArm() {
	glPushMatrix();
	glScalef(1.0, 3.0, 1.0);
	glutWireCube(1);
	glPopMatrix();
}

void human() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(3,3,3,0,0,0,0,1,0);

	//start making world coordinates
	glColor3f(1.0, 0.0, 0.0);
	
	glPushMatrix(); //to body edge
	drawBody(); //body at zero-point
	
	glPushMatrix(); //to head edge
	glTranslatef(0.0, 2.0, 0.0); //from head coordinate to body coordinate
	drawHead();
	glPopMatrix(); //from head node
	glPushMatrix(); //to right shoulder joint
	glTranslatef(1.5, 1.5, 0.0);
	//move arm to front
	//transform coordinate of joint from body view
	glRotatef(180, 1.0, 0.0, 0.0); //for arm moves to front, it rotate (-90) rotate, so cooridnate system move 90
        glPushMatrix();//to right arm
	glTranslatef(0.1061, -0.1061, 0.0);
	glRotatef(45.0, 0.0, 0.0, 1.0); //transform coordinate of arm from joint view
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
	glMatrixMode(GL_PROJECTION);
	glFrustum(-5, 5, -5, 5, 1, 10);
	glutDisplayFunc(human);
	glutMainLoop();
}
