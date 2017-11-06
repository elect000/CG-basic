#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <GL/glut.h>

using namespace std;

class Vector3d
{
public:
  double x, y, z;
  Vector3d() {x = y = z = 0;}
  Vector3d(double _x, double _y, double _z) {x = _x; y = _y; z = _z;}

  void set(double _x, double _y, double _z) {x = _x; y = _y; z = _z;}

  void normalize() {
    double len = length();
    x /= len; y /= len; z /= len;
  }

  double length() { return sqrt(x * x + y * y + z * z);}

  void scale(const double s) { x *= s; y *= s; z *= s; }

  Vector3d operator+(Vector3d v) { return Vector3d(x + v.x, y + v.y, z + v.z); }

  Vector3d operator-(Vector3d v) { return Vector3d(x - v.x, y - v.y, z - v.z); }

  double operator*(Vector3d v) { return x * v.x + y* v.y + z * v.z; }

  Vector3d operator%(Vector3d v) { return Vector3d(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }

  Vector3d& operator=(const Vector3d& v){ x = v.x; y = v.y; z = v.z; return (*this); }

  Vector3d& operator+=(const Vector3d& v) { x += v.x; y += v.y; z += v.z; return (*this); }

  Vector3d& operator-=(const Vector3d& v) { x -= v.x; y -= v.y; z -= v.z; return (*this); }

  void print() { printf("Vector3d(%f %f %f)\n", x, y, z); }
};

Vector3d operator-( const Vector3d& v ) { return( Vector3d( -v.x, -v.y, -v.z ) ); }

Vector3d operator*( const double& k, const Vector3d& v ) { return( Vector3d( k*v.x, k*v.y, k*v.z ) );}
Vector3d operator*( const Vector3d& v, const double& k ) { return( Vector3d( v.x*k, v.y*k, v.z*k ) );}

Vector3d operator/( const Vector3d& v, const double& k ) { return( Vector3d( v.x/k, v.y/k, v.z/k ) );}

class Sphere {
public:
	Vector3d position;
	float color[3];
  int size_s = 32;
  float size = 0.5;
  bool visible = true;

	void setColor(float r, float g, float b) {
    /*
      require: (float)rgb-red (float)rgb-green (float)rgb-blue
      return: null
     */
		color[0] = r; color[1] = g; color[2] = b;
	}

	void display() {
		glPushMatrix();
		glTranslated(position.x, position.y, position.z);
		glScaled(2, 2, 2);
		glutSolidSphere(size, size_s, size_s);
		glPopMatrix();
	}
};

Sphere g_Sphere[20];
int g_SelectedSphereID = -1;

int g_WindowWidth = 512;
int g_WindowHeight = 512;

int num = 3; // number of spheres

int pickSphere(int x, int y) {
  /*
    require: (int)x-position,(int)y-position
    return: sphere-color-number or -1
   */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

  for (int i = 0; i < num; i++) {
		glColor3ub(i, 0, 0);
		g_Sphere[i].display();
	}

  GLubyte c[num];
  glReadPixels(x,y, 1, 1, GL_RGB,GL_UNSIGNED_BYTE, c);
  return (c[0] == num) ? -1 : (int)c[0];
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, g_WindowWidth / (float)g_WindowHeight, 1.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 30, 0, 0, 0, 0, 1, 0);
  // もしすべてが偽ならばレベルアップする。
  // srand(0);
	for (int i = 0; i < num ; i++) {
    if (g_Sphere[i].visible == true) {
      glMaterialfv(GL_FRONT, GL_DIFFUSE, g_Sphere[i].color);
      g_Sphere[i].display();
    }
    else {
      glMaterialfv(GL_FRONT, GL_DIFFUSE, g_Sphere[i].color);
      g_Sphere[i].display();
    }
	}

	if(g_SelectedSphereID != -1) {
    g_Sphere[g_SelectedSphereID].visible = false;
    glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);

		glRasterPos3d(0, 0, 0);

		char str[256];
		sprintf(str, "Clicked!");

		for (int i = 0; str[i] != '\0'; i++) {
		  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
		}
  }
	glutSwapBuffers();
}

void resize(int w, int h) {
	if (h < 1) return;

	glViewport(0, 0, w, h);

	g_WindowWidth = w;
	g_WindowHeight = h;
}

void MousePick(int x, int _y) {

	printf("MousePick(%d, %d)\n", x, _y);

	const int y = g_WindowHeight - _y;

	g_SelectedSphereID = pickSphere(x, y);

	if (g_SelectedSphereID == -1) return;

  double M[16];
  glGetDoublev(GL_MODELVIEW_MATRIX, M);
  double P[16];
  glGetDoublev(GL_PROJECTION_MATRIX, P);
  int V[4];
  glGetIntegerv(GL_VIEWPORT, V);
  float z;
  glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
  double objX, objY, objZ;
  gluUnProject(x, y, z, M, P, V, &objX, &objY, &objZ);
}

void mouse(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) MousePick(x, y); 
	glutPostRedisplay();
}

void motion(int x, int y) {
	MousePick(x, y);
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'q':
	case 'Q':
	case '\033':
		exit(0);  /* '\033' は ESC の ASCII コード */
	default:
		break;
	}

	glutPostRedisplay();
}

void init() {
  srand(0);
	// Level 1
  g_Sphere[0].position.set(-5, 0, 0);
	g_Sphere[1].position.set( 0, 0, 0);
	g_Sphere[2].position.set( 5, 0, 0);
	g_Sphere[0].setColor(1, 0, 0);
	g_Sphere[1].setColor(0, 1, 0);
	g_Sphere[2].setColor(0, 0, 1);

	glClearDepth(1000.0);
	glClearColor(1, 1, 1, 1);

	float lightAmbientColor[] = { 0.2f, 0.2f, 0.2f, 0.0f };
	float lightDiffuseColor[] = { 1.f, 1.f, 1.f, 0.0f };
	float lightSpecularColor[] = { 0.4f, 0.4f, 0.4f, 0.0f };
	float lightPosition[] = { 0.0f, 30.0f, 30.0f, 0.0f };
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbientColor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuseColor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecularColor);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	float specularColor[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	float diffuseColor[] = { 1.f, 0.f, 0.f, 1.f };
	float shininess = 64.f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularColor);
	glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseColor);
}

int main(int argc, char**argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(g_WindowWidth, g_WindowHeight);
	glutCreateWindow("Mouse Picking");

	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);

	init();

	glutMainLoop();

	return 0;
}
