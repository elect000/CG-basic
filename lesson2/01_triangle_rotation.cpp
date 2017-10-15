#include <math.h>
#include <GL/glut.h>

int rotateAngle;

void display(void) {
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glRotated(rotateAngle, 0, 0, 1);

  glColor3d(1.0, 0.0, 0.0);
  glBegin(GL_TRIANGLES);
  glVertex3d(0,     0, 0);
  glVertex3d(0.5,   0, 0);
  glVertex3d(0.5, 0.5, 0);
  glEnd();

  glutSwapBuffers();
}

void timer(int value){
  rotateAngle++;
  glutPostRedisplay();
  glutTimerFunc(100, timer, 0);
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);

  glutInitWindowSize(400, 400);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);

  glutTimerFunc(100, timer, 0);

  rotateAngle = 0;

  glutMainLoop();
  return 0;
}

