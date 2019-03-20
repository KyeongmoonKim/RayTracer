#include<GL/glut.h>
#include<math.h>
#include<iostream>
#include"myTrans.h"
//hiearachical modeling 
GLfloat thetaBody;
GLfloat thetaRA;

/*
	STATE : body and arm detach. rotating joint
	TODO : The transform matrix should be added before body.
*/

void myCube(GLfloat sx, GLfloat sy, GLfloat sz, GLfloat r, GLfloat g, GLfloat b, GLfloat len) {
	glPushMatrix();
	glScalef(sx, sy, sz);
	glColor3f(r, g, b);
	glutSolidCube(len);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(len + 0.00001);
	glPopMatrix();
}


void human() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(2,3,4, 0,0,0, 0,1,0);
	//start making world coordinate
	glPushMatrix(); //to body edge
	{
		myCube(1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 3.0);
		//body local cs
		glPushMatrix(); //to head edge
		{
			glTranslatef(0.0, 2.0, 0.0); //from head coordinate to body coordinate
			//head local cs
			myCube(1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0);
		}
		glPopMatrix(); //from head node
		glPushMatrix(); //to right shoulder joint
		{//for rightshoulder
			//glRotatef(45, 0.0, 0.0, 1.0); //parent rotate children using parent's 
			glTranslatef(1.5, 1.5, 0.0);
			glPushMatrix();
			{//for arm1
				glRotatef(0.0, 0.0, 0.0, 1.0); //arm1 up-down rotation will be happen
				glTranslatef(0.75, 0.0, 0.0);
				glRotatef(90.0, 0.0, 0.0, 1.0); //transform coordinates
				myCube(0.5, 1.5, 0.5, 1.0, 0.0, 0.0, 1.0); 
				glPushMatrix();
				{//for arm1-2 joint
					glTranslatef(0.0, -0.75, 0.0);
					glPushMatrix();
					{//for arm2
						glRotatef(90, 0, 0, 1);
						glTranslatef(0.0, -0.75, 0.0);
						myCube(0.5, 1.5, 0.5, 0.0, 1.0, 0.0, 1.0);
					}
					glPopMatrix();
				}
				glPopMatrix();
			}
			glPopMatrix();

		}
		glPopMatrix(); // from right shoulder joint
	}
	glPopMatrix();
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
	glFrustum(-3, 3, -3, 3, 1, 20);
	glutDisplayFunc(human);
	glutMainLoop();
}
