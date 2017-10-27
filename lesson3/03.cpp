#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <cstdlib>
#include <GL/glut.h>
#define _USE_MATH_DEFINES	// Visual Studio で 円周率として定数 M_PI を使用するため
#include <cmath>


const int g_NumBox = 100;
const float g_BigBoxSize = 3.f;
const float g_SmallBoxSize = 1.f;


const float g_EyeCenterY = 9.f;
const float g_EyeCenterZ = 30.f;
const float g_EyeRadius = 8.f;
float g_EyeY, g_EyeZ;

const int g_AnimationIntervalMsec = 10;
int GLtimenow = 0;
int GLtimebase = 0;

float g_RotationDegree = 0.f;
const float g_DeltaRotationDegree = 0.3f;

int g_WindowWidth = 512;
int g_WindowHeight = 512;

const float Pos = 6.f;

float frand() { return rand() / (float)RAND_MAX; }

void display () {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.0, g_WindowWidth/(double)g_WindowHeight, 1, 100.0);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0, g_EyeY, g_EyeZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  float ambientColor[] = {0.1745,   0.01175,  0.01175,   1.0} /*{ 0.4f, 0.2f, 0.2f, 1.0f }*/;
	float diffuseColor[] = {0.61424,  0.04136,  0.04136,   1.0} /*{ 1.f, 0.8f, 0.8f, 1.0f }*/;
	float specularColor[] = {0.727811, 0.626959, 0.626959,  1.0};
	float shininess = 76.8f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularColor);
	glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);

  GLtimenow = glutGet(GLUT_ELAPSED_TIME);
  if (!(GLtimenow - GLtimebase > 3000)) {
    glPushMatrix();
    glTranslatef(sinf(g_RotationDegree),
                 sinf(g_RotationDegree) * sinf(3.0 * g_RotationDegree) ,
                 0);
    glRotatef(g_RotationDegree * 10, 0, 1, 0);
    glColor4f(1.0, 1.0, 1.0, 0.2);
    glutSolidTeapot(g_BigBoxSize);
    glPopMatrix();
  } else {
    srand(0);
    int j;
    for (int i = 1; i < g_NumBox + 1; ++i) {
      if (i % 2) {j=-1;} else{j=1;}
      glPushMatrix();
      glRotatef(g_RotationDegree * 0.1, 1, 1, 1);
      glRotatef(100, (1 + rand()) * 10, (1 + rand()) * 10, (1 + rand()) * 10);
      glTranslatef(i * (g_RotationDegree * 0.01),
                   i * (g_RotationDegree * 0.01),
                   i * (g_RotationDegree * 0.01));
      glRotatef(g_RotationDegree * 0.1, 1, 1, 1);
      glutSolidTeapot(1.2f * g_SmallBoxSize);
      glPopMatrix();
    }
  }
  glutSwapBuffers();
}

void init() {
	glClearColor(1, 1, 1, 1);
	glClearDepth(100.f);

	float lightAmbientColor0[] = { 0.2f, 0.2f, 0.2f, 0.0f };
	float lightDiffuseColor0[] = { 0.4f, 0.4f, 0.4f, 0.0f };
	float lightSpecularColor0[] = { 0.8f, 0.8f, 0.8f, 0.0f };
	float lightPosition0[] = { 5.0f, 5.0f, 8.0f, 0.0f };

	float lightAmbientColor1[] = { 0.2f, 0.2f, 0.2f, 0.0f };
	float lightDiffuseColor1[] = { 0.4f, 0.4f, 0.4f, 0.0f };
	float lightSpecularColor1[] = { 0.8f, 0.8f, 0.8f, 0.0f };
	float lightPosition1[] = { -5.0f, 2.0f, 3.0f, 0.0f };

	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbientColor0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuseColor0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecularColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition0);

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbientColor1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuseColor1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecularColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1);


	glEnable(GL_DEPTH_TEST);
}


void timer(int val) {
  g_RotationDegree += g_DeltaRotationDegree;
  g_EyeY = g_EyeCenterY;
  g_EyeZ = g_EyeCenterZ;
  if (100 < g_RotationDegree && g_RotationDegree < 400) {
    g_EyeY = g_EyeCenterY * (1.0 + sinf(0.05  * g_RotationDegree));
    g_EyeZ = g_EyeCenterZ * (1 + 0.05 *  sinf(0.05  * g_RotationDegree));
  }


  glutPostRedisplay();
  glutTimerFunc(g_AnimationIntervalMsec, timer, val);
}




void reshape(int w, int h) {
  if (h < 1) return;
  glViewport(0, 0, w, h);
	g_WindowWidth = w;
	g_WindowHeight = h;
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(g_WindowWidth, g_WindowHeight);

	
  glutCreateWindow("Bomb!!!");

  glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(g_AnimationIntervalMsec, timer, 0);

	init();

	glutMainLoop();

	return 0;
}
