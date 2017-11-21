#include <cstdlib>
#include <cmath>
#include <vector>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h> 
// ========================================================================
#define OUTPUT_FILENAME "parametric_surface.obj" // 出力ファイル名
#define PI 3.1415926535

#define NUM_U 50  // U方向の分割数
#define NUM_V 50  // V方向の分割数

double x[NUM_U+1][NUM_V+1]; // x 座標
double y[NUM_U+1][NUM_V+1]; // y 座標
double z[NUM_U+1][NUM_V+1]; // z 座標
// =========================================================================
using namespace std;

class Vector2d {
public:
  double x, y;
  Vector2d() { x = y = 0; }
  Vector2d(double _x, double _y) { x = _x; y = _y; }
  void set(double _x, double _y) { x = _x; y = _y; }

  void normalize() {
    double len = length();
    x /= len; y /= len;
  }
  double length() { return sqrt(x * x + y * y); }
  void scale(const double s) { x *= s; y *= s; }
  Vector2d operator+(Vector2d v) { return Vector2d(x + v.x, y + v.y); }
  Vector2d operator-(Vector2d v) { return Vector2d(x - v.x, y - v.y); }
  double operator*(Vector2d v) { return x * v.x + y* v.y; }
  Vector2d& operator=(const Vector2d& v){ x = v.x; y = v.y; return (*this); }
  Vector2d& operator+=(const Vector2d& v) { x += v.x; y += v.y; return (*this); }
  Vector2d& operator-=(const Vector2d& v) { x -= v.x; y -= v.y; return (*this); }
  void print() { printf("Vector2d(%f %f)\n", x, y); }
};

Vector2d operator-( const Vector2d& v ) { return( Vector2d( -v.x, -v.y ) ); }
Vector2d operator*( const double& k, const Vector2d& v ) { return( Vector2d( k*v.x, k*v.y ) );}
Vector2d operator*( const Vector2d& v, const double& k ) { return( Vector2d( v.x*k, v.y*k ) );}
Vector2d operator/( const Vector2d& v, const double& k ) { return( Vector2d( v.x/k, v.y/k ) );}

// ================================================================================================


std::vector<Vector2d> g_ControlPoints;

int Fact (int n)
{
  int r = 1;
  for (int i=2; i<=n; i++)
    r *= i;
  return r;
}

void display(void) {
  glClearColor (1.0, 1.0, 1.0, 1.0);
  glClear (GL_COLOR_BUFFER_BIT );

  glPointSize(5);
  glColor3d(0.0, 0.0, 0.0);
  glBegin(GL_POINTS);
  for(unsigned int i = 0; i < g_ControlPoints.size(); i++) {
    glVertex2d(g_ControlPoints[i].x, g_ControlPoints[i].y);
  }
  glEnd();

  glColor3d(1.0, 0.0, 0.0);
  glLineWidth(1);
  glBegin(GL_LINE_STRIP);
  for(unsigned int i = 0; i < g_ControlPoints.size(); i++) {
    glVertex2d(g_ControlPoints[i].x, g_ControlPoints[i].y);
  }
  glEnd();

  int n = 3;
  int count = 0;
  int temp = g_ControlPoints.size() - n;
  while (temp > 0) {
    count++;
    temp -= n;
  }

  Vector2d sum = Vector2d(0, 0);
  glColor3d(0.0, 0.0, 1.0);
  glLineWidth(1);
  glBegin(GL_LINE_STRIP);
  if (g_ControlPoints.size() >= n + 1) {
    for (int k = 0; k < count; ++k) {
      for (float t = 0.0f; t <= 1.001f; t += 0.01f) {
        sum = Vector2d(0, 0);
        for (int i = 0; i <= n; ++i) {
          sum += Fact(n)/Fact(i)/Fact(n - i) * pow(t, i) * pow((1.0 - t), (n - i)) * g_ControlPoints[i + k * n];
        }
        glVertex2d(sum.x, sum.y);
      }
    }
  }
  glEnd();

  glutSwapBuffers();
}


void setCoordinates() {
  int n = 3;
  int count = 0;
  int temp = g_ControlPoints.size() - n;
  while (temp > 0) {
    count++;
    temp -= n;
  }
  printf("set...");
  Vector2d sum = Vector2d(0, 0);
  Vector2d presum = Vector2d(0, 0);
  int c, k, loop;
  if (g_ControlPoints.size() >= n + 1) {
    for (k = 0; k < count; ++k) {
      c = -1;
      for (float t = 0.0f; t <= 1.001f; t += 1.0f / (NUM_U  / count)) {
        c++;
        sum = Vector2d(0, 0);
        for (int i = 0; i <= n; ++i) {
          sum += Fact(n)/Fact(i)/Fact(n - i) * pow(t, i)
            * pow((1.0 - t), (n - i)) * g_ControlPoints[i + k * n];
        }
        for (int j = 0; j < NUM_U + 1; ++j) {
          loop++;
          double u = 1.0 / NUM_U * j * (2 * PI);
          x[k * (NUM_U / count) + c][j] = sum.x * sin(u);
          y[k * (NUM_U / count) + c][j] = sum.y;
          z[k * (NUM_U / count) + c][j] = sum.x * cos(u);
        }
        presum = sum;
      }
    }
    while (loop < NUM_U * NUM_V) {
      for (int i = 0; i < NUM_U + 1; ++i) {
          double u = 1.0 / NUM_U * i * (2 * PI);
          x[loop / NUM_U][loop % NUM_U] = presum.x * sin(u);
          y[loop / NUM_U][loop % NUM_U] = presum.y;
          z[loop / NUM_U][loop % NUM_U] = presum.x * cos(u);
          loop++;
      }
    }
  }
  printf("finish!\n");
}

void resizeWindow(int w, int h) {
  h = (h == 0) ? 1 : h;
  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, w, h, 0, -10, 10);
  glMatrixMode(GL_MODELVIEW);
}

void exportOBJ() {
  printf("exporting...");
	FILE *fout;

	fout = fopen(OUTPUT_FILENAME, "w");

	if(fout == NULL) {
		printf("Error: fopen\n");
		exit(0);
	}

	for(int i = 0; i < NUM_U+1; i++) {
		for(int j = 0; j < NUM_V+1; j++) {
			fprintf(fout, "v %f %f %f\n", x[i][j], y[i][j] , z[i][j]);
		}
	}

  for(int i = 0; i < NUM_U; i++) {
    for(int j = 0; j < NUM_V; j++) {
      int lb_index = (NUM_V + 1) * i + j + 1;
      int lt_index = (NUM_V + 1) * i + j + 1 + (NUM_V + 1); 
      int rb_index = (NUM_V + 1) * i + j;
      int rt_index = (NUM_V + 1) * i + j + (NUM_V + 1);

      fprintf(fout, "f %d %d %d\n", lb_index+1, rt_index+1, lt_index+1);
      fprintf(fout, "f %d %d %d\n", lb_index+1, rb_index+1, rt_index+1);
    }
  }

	fclose(fout);
  printf("exported!\n");
}


void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 'q':
  case 'Q':
  case '\033':
    exit(0);  /* '\033' は ESC の ASCII コード */
  case 'c':
    setCoordinates();
    exportOBJ();
  default:
    break;
  }
  glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
  if(state == GLUT_DOWN) {
    switch (button) {
    case GLUT_LEFT_BUTTON:
      g_ControlPoints.push_back(Vector2d(x, y));
    break;
    case GLUT_MIDDLE_BUTTON:
    break;
    case GLUT_RIGHT_BUTTON:
      if(!g_ControlPoints.empty()) {
        g_ControlPoints.pop_back();
      }
    break;
    default:
    break;
    }
    glutPostRedisplay();
  }
}

int main (int argc, char *argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);
  glutInitWindowSize(800 , 800);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);
  glutReshapeFunc(resizeWindow);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMainLoop();
  return 0;
}
