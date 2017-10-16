#include <math.h>
#include <GL/glut.h>

#ifndef M_PI
#define M_PI 3.14159265358979
#endif

#define ID_DRAW_BALL 1
#define ID_DRAW_BALL2 2
#define ID_DRAW_BALL3 3
#define ID_DRAW_BOX 4

int rotateAngle;
float r;

void display(void){
  // init
  glClearColor (1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glPushMatrix();
  glColor3d(1.0, 0.4, 0.4);
  glRotated(rotateAngle * 20, 0, 0, 1);
  glTranslated(r, 0, 0);
  glCallList(ID_DRAW_BALL);
  glPopMatrix();

  glPushMatrix();
  glColor3d(0.4, 1.0, 0.4);
  glRotated(rotateAngle * 20, 0, 0, 1);
  glTranslated(r/sqrt(2), r/sqrt(2), 0);
  glCallList(ID_DRAW_BALL2);
  glPopMatrix();

  glPushMatrix();
  glColor3d(0.4, 0.4, 1.0);
  glRotated(rotateAngle * 20, 0, 0, 1);
  glTranslated(0, r, 0);
  glCallList(ID_DRAW_BALL3);
  glPopMatrix();

  glPushMatrix();
  glColor3d(0.7, 0.7, 0.7);
  glRotated(rotateAngle * 20, 0, 0, 1);
  glCallList(ID_DRAW_BOX);
  glPopMatrix();

  glutSwapBuffers();
}

void timer(int value) {
  rotateAngle++;
  r += 0.01;

  glutPostRedisplay();
  glutTimerFunc(100, timer, 0);
}

void buildCircle(float r2) {
  int i, n = 200;
  double x, y = 0.5;
  glBegin(GL_POLYGON);
  for (i = 0; i < n; ++i) {
    x =  r2 * cos(2.0 * 3.14 * ((double)i/n));
    y =  r2 * sin(2.0 * 3.14 * ((double)i/n));
    glVertex3f(x, y, 0.0);
  }
  glEnd();

}

void buildDisplayList() {
  glNewList(ID_DRAW_BALL, GL_COMPILE);
  buildCircle(0.1);
  glEndList();

  glNewList(ID_DRAW_BALL2, GL_COMPILE);
  buildCircle(0.15);
  glEndList();

  glNewList(ID_DRAW_BALL3, GL_COMPILE);
  buildCircle(0.2);
  glEndList();

  glNewList(ID_DRAW_BOX, GL_COMPILE);
  glBegin(GL_QUADS);
  glVertex2d(0.1, 0.1);
  glVertex2d(0.1, -0.1);
  glVertex2d(-0.1, -0.1);
  glVertex2d(-0.1, 0.1);
  glEnd();
  glEndList();
}

int main(int argc, char *argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);

  glutInitWindowSize(400, 400);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);

  glutTimerFunc(100, timer, 0);

  buildDisplayList();

  rotateAngle = 0;

  glutMainLoop();
  return 0;
}
