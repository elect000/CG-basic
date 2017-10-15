#include <math.h>
#include <GL/glut.h>

#ifndef M_PI
#define M_PI 3.14159265358979
#endif

#define ID_DRAW_STAR 1
#define ID_DRAW_STAR2 2

int  rotateAngle;

void display(void){
  glClearColor (1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  // *
  glPushMatrix();
  glTranslated(0, 0, 0);
  glRotated(rotateAngle, 0, 0, 1);
//1-5
  glPushMatrix();
  glColor3d(1.0, 1.0, 0.0);
  glTranslated(0.5, 0, 0);
  glRotated(rotateAngle, 0, 0, 1);
  glCallList(ID_DRAW_STAR2);
  glPopMatrix();
//1
  glPushMatrix();
  glColor3d(1.0, 0.0, 0.0);
  glTranslated(0.5, 0, 0);
  glRotated(rotateAngle, 0, 0, 1);
  glCallList(ID_DRAW_STAR);
  glPopMatrix();
//2
	glPushMatrix();
	glColor3d(0.0, 1.0, 0.0);
	glTranslated(0, 0.5, 0);
  glRotated(rotateAngle, 0, 0, 1);
  glCallList(ID_DRAW_STAR);
  glPopMatrix();
//3
  glPushMatrix();
  glColor3d(0.0, 0.0, 1.0);
  glTranslated(-0.5, 0, 0);
  glRotated(rotateAngle, 0, 0, 1);
  glCallList(ID_DRAW_STAR);
  glPopMatrix();

  glPopMatrix();
  glutSwapBuffers();
}

void timer(int value) {
	rotateAngle++;

	glutPostRedisplay();
	glutTimerFunc(100, timer, 0);
}

void buildDisplayList() {
	glNewList(ID_DRAW_STAR,GL_COMPILE);

	double r0 = 0.15;
	double r1 = 0.4;
	glBegin(GL_TRIANGLES);
	for(int i = 0; i < 5; i++) {
		int deg = i * 72;
		glVertex3d(r0 * cos( (deg - 36) * M_PI / 180.0), r0 * sin( (deg - 36) * M_PI / 180.0), 0);
		glVertex3d(r1 * cos( deg * M_PI / 180.0), r1 * sin( deg * M_PI / 180.0), 0);
		glVertex3d(r0 * cos( (deg + 36) * M_PI / 180.0), r0 * sin( (deg + 36) * M_PI / 180.0) ,0);
	}
	glEnd();

	glEndList();

  glNewList(ID_DRAW_STAR2,GL_COMPILE);

	//double r0 = 0.15;
	//double r1 = 0.4;
	glBegin(GL_TRIANGLES);
	for(int i = 0; i < 5; i++) {
		int deg = i * 72;
		glVertex3d(r0 * cos( (deg - 36) * M_PI / 180.0), r0 * sin( (deg - 36) * M_PI / 180.0), 0);
		glVertex3d(r1 * cos( deg * M_PI / 180.0), r1 * sin( deg * M_PI / 180.0), 0);
		glVertex3d(r0 * cos( (deg + 36) * M_PI / 180.0), r0 * sin( (deg + 36) * M_PI / 180.0) ,0);
	}
	glEnd();

	glEndList();
}


int main (int argc, char *argv[]) { 
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);

	glutInitWindowSize(400 , 400);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(display);

	glutTimerFunc(100 , timer , 0);

	buildDisplayList();

	rotateAngle = 0;

	glutMainLoop();
	return 0;
}
