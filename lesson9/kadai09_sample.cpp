#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>

class Vector3d {
public:
  double x, y, z;
  Vector3d() { x = y = z = 0; }
  Vector3d(double _x, double _y, double _z) { x = _x; y = _y; z = _z; }
  void set(double _x, double _y, double _z) { x = _x; y = _y; z = _z; }
  void normalize() {
    double len = length();
    x /= len; y /= len; z /= len;
  }
  double length() { return sqrt(x * x + y * y + z * z); }
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


// ����
class Sphere {
public:
  Vector3d center; // ���S���W
  double radius;   // ���a
  double cR, cG, cB;  // Red, Green, Blue �l 0.0�`1.0

  Sphere(double x, double y, double z, double r,
    double cr, double cg, double cb) {
      center.x = x;
      center.y = y;
      center.z = z;
      radius = r;
      cR = cr;
      cG = cg;
      cB = cb;
  }

  // �_p��ʂ�Av������Ray�Ƃ̌����𔻒肷��B
  // ��_�� p+tv �Ƃ��ĕ\����ꍇ�� t �̒l��Ԃ��B�����Ȃ��ꍇ��-1��Ԃ�
  double getIntersec(Vector3d &p, Vector3d &v) {
    // A*t^2 + B*t + C = 0 �̌`�ŕ\��
    double A = v.x * v.x + v.y * v.y + v.z * v.z;
    double B = 2.0 * (p.x * v.x - v.x * center.x +
      p.y * v.y - v.y * center.y +
      p.z * v.z- v.z * center.z);
    double C = p.x * p.x - 2 * p.x * center.x + center.x * center.x +
      p.y * p.y - 2 * p.y * center.y + center.y * center.y +
      p.z * p.z - 2 * p.z * center.z + center.z * center.z -
      radius * radius;
    double D = B * B - 4 * A * C; // ���ʎ�

    if (D >= 0) { // �����
      double t1 = (-B - sqrt(D)) / (2.0 * A);
      double t2 = (-B + sqrt(D)) / (2.0 * A);
      return t1 < t2 ? t1 : t2; // �������ق���t�̒l��Ԃ�
    } else { // �����Ȃ�
      return -1.0;
    }
  }
};

// �Bxz���ʂɕ��s�ȖʂƂ���
class Board {
public:
  double y; // y���W�l

  Board(double _y) {
    y = _y;
  }

  // �_p��ʂ�Av������Ray�Ƃ̌����𔻒肷��B
  // ��_�� p+tv �Ƃ��ĕ\����ꍇ�� t �̒l��Ԃ��B�����Ȃ��ꍇ�͕��̒l��Ԃ�
  double getIntersec(Vector3d &p, Vector3d &v) {
    if(fabs(v.y) < 1.0e-10) return -1; // ������Ray�͌����Ȃ�

    double t = -1;
    // �������� t �̒l���v�Z����
    // ��������z���W��-3000��菬�����Ȃ�����Ȃ����̂Ƃ���
    // -----------------------------------------------------------------------------
    t = (y + p.y) / v.y;
    if ((t < 0) || ((p + t * v).z < -3000)) {
      return -1;
    }
    // -----------------------------------------------------------------------------
    return t;
  }

  // x �� z �̒l���珰�̐F��Ԃ��i�i�q�͗l�ɂȂ�悤�Ɂj
  Vector3d getColorVec(double x, double z) {
    // �� x, z �̒l�ɂ����(1.0, 1.0, 0.7)�܂���(0.6, 0.6, 0.6)�̂ǂ��炩�̐F��Ԃ��悤�ɂ���
    int flag = 1;
    if (x > 0) {
      if (((int)x) % 200 > 100) flag *= -1;
    } else {
      if (((int) (-1 * x)) % 200 < 100) flag *= -1;
    }
    if (z > 0) {
      if (((int)z) % 200 > 100) flag *= -1;
    } else {
      if (((int) (-1 * z)) % 200 < 100) flag *= -1;
    }
    if (flag > 0) {
      return Vector3d(0.6, 0.6, 0.6);
    } else {
      return Vector3d(1.0, 1.0, 0.7);
    }
  }
};

int halfWidth;    // �`��̈�̉���/2
int halfHeight;   // �`��̈�̏c��/2
double screen_z = -1000;  // ���e�ʂ�z���W
double Kd = 0.8;  // �g�U���˒萔
double Ks = 0.8;  // ���ʔ��˒萔
double Iin = 0.5; // ���ˌ��̋���
double Ia  = 0.5; // ����
Vector3d viewPosition(0, 0, 0); // ���_=���_
Vector3d lightDirection(-2, -4, -2); // ���ˌ��̐i�s����


// �����_�����O���鋅��
Sphere sphere(0.0, 0.0, -1500, // ���S���W
        150.0,           // ���a
        0.1, 0.7, 0.7);  // RGB�l

// ���̂̒u����Ă��鏰
Board board(-150); // y���W�l�� -150 �ɂ���B�i���Ɛڂ���悤�ɂ���j

// 2�̃x�N�g���̐����p��cos�l���v�Z����
double getCos(Vector3d &v0, Vector3d &v1) {
  return (v0.x * v1.x + v0.y * v1.y + v0.z * v1.z ) / (v0.length() * v1.length());
}

// x, y �Ŏw�肳�ꂽ�X�N���[�����W�ł̐F(RGB)�� colorVec ��xyz�̒l�Ɋi�[����
void getPixelColor(double x, double y, Vector3d &colorVec) {
  // ���_����X�N���[����̃s�N�Z���֔�΂����C�̕���
  Vector3d ray(x - viewPosition.x, y - viewPosition.y, screen_z - viewPosition.z);

  ray.normalize(); // ���C�̒����̐��K��

  // ���C���΂��ċ��ƌ������邩���߂�
  double t_sphere = sphere.getIntersec(viewPosition, ray);
  if(t_sphere > 0) { // ���Ƃ̌�_������
    // ���O��̉ۑ���Q�l�ɁA���̂̕\�ʂ̐F���v�Z�ŋ��߁AcolorVec�ɐݒ肷��
    double r = 0;
    double g = 0;
    double b = 0;
    // ---------------------------------------------------------------------------------
    double Id, Is, cos_Id, cos_Is, a, I;
    Is = Id = I = 0;
    Vector3d P = viewPosition + t_sphere * ray;
    Vector3d N = P - sphere.center;
    N.normalize();
    cos_Id = N * (-1 * lightDirection);
    if (cos_Id > 0) {
      Id = Iin * Kd * cos_Id;
    }
    int n = 20;
    a = -1 * (lightDirection * N);
    Vector3d R = lightDirection + 2 * a * N;
    Vector3d V = P - viewPosition;
    R.normalize();
    V.normalize();
    cos_Is = -1 * R * V;
    if (cos_Is > 0) {
      Is = Iin * Ks * pow(cos_Is, n);
    }
    I = Id + Is + Ia;
    r = std::min(I * sphere.cR, 1.0);
    g = std::min(I * sphere.cG, 1.0);
    b = std::min(I * sphere.cB, 1.0);
    // ---------------------------------------------------------------------------------
    colorVec.set(r, g, b);
    return;
  }

  // ���C���΂��ď��ƌ������邩���߂�
  double t_board = board.getIntersec(viewPosition, ray);

  if(t_board > 0) { // ���Ƃ̌�_������
    // �����̕\�ʂ̐F��ݒ肷��
    // �����̉e�ɂȂ�ꍇ�́ARGB�̒l�����ꂼ��0.5�{����
    double r = 0;
    double g = 0;
    double b = 0;
    // ----------------------------------------------------------------------------------
    double x, z;
    Vector3d board_pos = viewPosition + t_board * ray;
    Vector3d colors;
    x = board_pos.x;
    z = board_pos.z;
    colors = board.getColorVec(x, z);
    r = colors.x;
    g = colors.y;
    b = colors.z;
    if (sphere.getIntersec(board_pos, lightDirection) != -1.0) {
      r *= 0.5;
      g *= 0.5;
      b *= 0.5;
    } 
    // ----------------------------------------------------------------------------------
    colorVec.set(r, g, b);
    return;
  }

  // ���Ƃ��������Ȃ�
  colorVec.set(0, 0, 0); // �w�i�F�i���j��ݒ肷��
}

// �`����s��
void display(void) {

  glClear(GL_COLOR_BUFFER_BIT); // �`����e�̃N���A

  // �s�N�Z���P�ʂŕ`��F�����肷�郋�[�v����
  for(int y = (-halfHeight); y <= halfHeight; y++ ) {
    for(int x = (-halfWidth); x <= halfWidth; x++ ) {
      // ---------------------------------------------------------------------------------
      int i, j;
      Vector3d colorVecs[9];
      Vector3d colorVec;
      for (i = 1; i < 4; ++i) {
        for (j = 1; j < 4; ++j) {
          getPixelColor(x - i / 3.0, y - j / 3.0, colorVecs[(i - 1) * 3 + (j - 1)]);
        }
      }
      for (i = 0; i < 9; ++i) {
        colorVec += colorVecs[i];
      }
      //�擾�����F�ŁA�`��F��ݒ肷��
      glColor3d(colorVec.x / 9.0, colorVec.y / 9.0, colorVec.z / 9.0);
      // ---------------------------------------------------------------------------------

      // (x, y) �̉�f��`��
      glBegin(GL_POINTS);
      glVertex2i( x, y );
      glEnd();
    }
  }
  glFlush();
}

void resizeWindow(int w, int h) {
  h = (h == 0) ? 1 : h;
  glViewport(0, 0, w, h);
  halfWidth = w/2;
  halfHeight = h/2;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // �E�B���h�E���̍��W�n�ݒ�
  glOrtho( -halfWidth, halfWidth, -halfHeight, halfHeight, 0.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 27: exit(0);  /* ESC code */
  }
  glutPostRedisplay();
}

int main(int argc, char** argv) {
  lightDirection.normalize();

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(400,400);
  glutInitWindowPosition(180,10);
  glutCreateWindow(argv[0]);
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glShadeModel(GL_FLAT);

  glutDisplayFunc(display);
  glutReshapeFunc(resizeWindow);
  glutKeyboardFunc(keyboard);
  glutMainLoop();

  return 0;
}

