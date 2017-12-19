#include <GL/glut.h>
#include <math.h>   
#include <stdlib.h> 
#include <stdio.h> 
#include <algorithm>
#include <vector>

// 3�����x�N�g��
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

// ------------------------------------------------------
// ����
class Sphere {
public:
  Vector3d center; // ���S���W
  double radius;   // ���a

  Sphere(double x, double y, double z, double r) {
    center.x = x;
    center.y = y;
    center.z = z;
    radius = r;
  }
  void display() { 
		glPushMatrix(); // ���݂̃��f���ϊ��s���ޔ����Ă���
    glColor3f(0.1,0.1,0.1);
		// ���W�̕��s�ړ��ƃX�P�[���ϊ����{���ċ��̂�`�悷��
    glTranslated(center.x, center.y, center.z);
		glScaled(1, 1, 1);
    glRotated(90, 1.0, 0.0, 0.0);
		glutWireSphere(radius, 32, 32);
		
		glPopMatrix();  // �ޔ����Ă������f���ϊ��s���߂�
	}
};
double radius = 4.0;
Sphere sphere = Sphere(0, 0, 0, radius);
// ------------------------------------------------------
// ���_
class Point {
public:
	Vector3d f; // ���_�ɓ����͂̃x�N�g��
	Vector3d v; // ���x�x�N�g��
	Vector3d p; // �ʒu
	bool bFixed; // true: �Œ肳��Ă��� false:�Œ肳��Ă��Ȃ�
};

// �o�l
class Spring {
public:
	Point *p0; // ���_
	Point *p1; // ���_
	double restLength; // ���R��

	Spring(Point *_p0, Point *_p1) {
		p0 = _p0;
		p1 = _p1;
		restLength = (p0->p - p1->p).length();
	}
};

// ----------------------------------------------------------------
#define POINT_NUM 20
// #define POINT_NUM 40
// #define POINT_NUM 60
// ----------------------------------------------------------------

// �z�̒�`
class Cloth {
public:
	Point points[POINT_NUM][POINT_NUM];
	std::vector<Spring*> springs;

	Cloth() {
		// ���_�̒�`
		for(int y = 0; y < POINT_NUM; y++) {
			for(int x = 0; x < POINT_NUM; x++) {
				points[x][y].bFixed = false;
        // ---------------------------------------------------------------
				points[x][y].p.set((x - POINT_NUM / 2) / (POINT_NUM / 20.0),
                           (POINT_NUM / 2) / (POINT_NUM / 20.0),
                           - y / (POINT_NUM / 20.0));
        // --------------------------------------------------------------
			}
		}
		// �o�l�̐ݒ�
		for(int y = 0; y < POINT_NUM; y++) {
			for(int x = 0; x < POINT_NUM; x++) {
				// �������̃o�l
				if(x < POINT_NUM - 1) {				
					springs.push_back(new Spring(&points[x][y], &points[x+1][y]));
				}
				// �c�����̃o�l
				if(y < POINT_NUM -1) {
					springs.push_back(new Spring(&points[x][y], &points[x][y+1]));
				}
				// �E�������̃o�l
				if(x < POINT_NUM - 1 && y < POINT_NUM - 1) {				
					springs.push_back(new Spring(&points[x][y], &points[x+1][y+1]));
				}
				// ���������̃o�l
				if(x > 0 && y < POINT_NUM - 1) {				
					springs.push_back(new Spring(&points[x][y], &points[x-1][y+1]));
				}
			}
		}

		// �Œ�_�̎w��
		points[0][0].bFixed = true;
		points[POINT_NUM-1][0].bFixed = true;
	}
};


Cloth *cloth;
double rotateAngleH_deg; // ��ʐ��������̉�]�p�x
double rotateAngleV_deg; // �c�����̉�]�p�x
int preMousePositionX;   // �}�E�X�J�[�\���̈ʒu���L�����Ă����ϐ�
int preMousePositionY;   // �}�E�X�J�[�\���̈ʒu���L�����Ă����ϐ�
bool bRunning; // �A�j���[�V�����̎��s/��~��؂�ւ���t���O

double Ks = 8;   // �o�l�萔
double Mass = 30; // ���_�̎���
double dT = 1; // ���ԍ��ݕ�
double Dk = 0.1; // ���x�ɔ�Ⴕ�āA�t�����ɂ͂��炭��R�W��
Vector3d gravity(0, -0.002, 0); // �d��(y�������̕��̌����ɓ���)

void drawCloth(void) {

	// �o�l��`��
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	for(int i = 0; i < cloth->springs.size(); i++) {
		glVertex3d(cloth->springs[i]->p0->p.x, cloth->springs[i]->p0->p.y, cloth->springs[i]->p0->p.z);
		glVertex3d(cloth->springs[i]->p1->p.x, cloth->springs[i]->p1->p.y, cloth->springs[i]->p1->p.z);
	}
	glEnd();

	// ���_��`��
	glColor3f(1.0f, 0.0f, 0.0f);
	glPointSize(4.0f);
	glBegin(GL_POINTS);
	for(int y = 0; y < POINT_NUM; y++) {
		for(int x = 0; x < POINT_NUM; x++) {
			glVertex3d(cloth->points[x][y].p.x, cloth->points[x][y].p.y, cloth->points[x][y].p.z);
		}
	}
	glEnd();
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_LIGHTING);
	glLoadIdentity();
	glTranslated(0, 0.0, -50);
	glRotated(rotateAngleV_deg, 1.0, 0.0, 0.0);
	glRotated(rotateAngleH_deg, 0.0, 1.0, 0.0);
  sphere.display();
	drawCloth();

	glFlush();
}

void resize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {

	if (key == '\033' || key == 'q') { exit(0);} // ESC �܂��� q �ŏI��
	if (key == 'a') { bRunning = !bRunning; }    // a �ŃA�j���[�V�����̃I���I�t
}

void mouse(int button, int state, int x, int y) {
	switch (button) {
  case GLUT_LEFT_BUTTON:
	  preMousePositionX = x;
	  preMousePositionY = y;
	  break;
  case GLUT_MIDDLE_BUTTON:
	  break;
  case GLUT_RIGHT_BUTTON:
	  preMousePositionX = x;
	  preMousePositionY = y;
	  break;
  default:
	  break;
	}
}
// -------------------------------------------------------------------
// �z�̌`��̍X�V
// �z�̌`��̍X�V
void updateCloth(void) {
	// �����̎菇�Ŏ��_�̈ʒu�����肷��
	// 1. ���_�ɓ����͂����߂�
	// 2. ���_�̉����x�����߂�
	// 3. ���_�̑��x���X�V����
	// 4. ���_�̈ʒu���X�V����

	//1-1. ���_�ɓ����͂����Z�b�g
	// �S�Ă̎��_�ɂ��ď��Ԃɏ�������
	for(int y = 0; y < POINT_NUM; y++) {
		for(int x = 0; x < POINT_NUM; x++) {
			cloth->points[x][y].f.set(0, 0, 0);
		}
	}

	//1-2. �o�l�̗��[�̎��_�ɗ͂𓭂�����
	// �i�l�����j���_��1��1���ׂ�̂ł͂Ȃ�
	// �o�l��1��1�����āA���̗��[�̎��_�ɗ͂����Z���Ă����A�v���[�`���Ƃ�
	for(int i = 0; i < cloth->springs.size(); i++) {// �S�Ẵo�l�ɂ��ď��Ԃɏ�������
		Spring *spring = cloth->springs[i]; 
	
		// (a) �o�l�̎��R���ƌ��݂̒����̍��������߂�
		// �o�l�̎��R���� spring->restLength �Ɋi�[����Ă���
		// ���݂̒����́A���[�̎��_�Ԃ̋������v�Z���ċ��߂�
		double prelen = spring->restLength;
    double newlen = (spring->p0->p - spring->p1->p).length();
    double lendiff = newlen - prelen;
    
		// (b) �o�l�����_�ɉ�����͂̑傫�������߂�
		// �i���R�� - ���݂̒����j�Ƀo�l�萔 Ks ���|�����l�����߂�傫��
    double powerval = Ks * lendiff;
		// (c) �o�l�����_�ɉ�����͂̌����i�P�ʃx�N�g���j�����߂�
		// �o�l�ɂ́A���[�Ɏ��_�����Ă���̂ŁA������瑼���Ɍ������������͂̌���
    Vector3d powervec = spring->p0->p - spring->p1->p;
    powervec.normalize();
		// (d) ���[�̎��_�ɑ΂��āA�̓x�N�g���i �傫����(b)�ŋ��߂��B������(c)�ŋ��߂��j�����Z����
		// spring->p0->f �o�l�̈���̎��_�ɉ����͂�\���̂ŁA����ɗ̓x�N�g����������i�����ɒ��Ӂj
		// spring->p1->f �o�l�̂�������̎��_�ɉ����͂�\���̂ŁA����ɗ̓x�N�g����������i�����ɒ��Ӂj
    spring->p0->f -= powerval * powervec;
    spring->p1->f += powerval * powervec;
	}

	//1-3. �d�́A��C��R�ɂ��͂����Z����
	// �S�Ă̎��_�ɂ��ď��Ԃɏ�������
	for(int y = 0; y < POINT_NUM; y++) {
		for(int x = 0; x < POINT_NUM; x++) {
			// cloth->points[x][y].f �ɁA�d�͂ɂ��͂����Z����
      cloth->points[x][y].f += Mass * gravity;
			// cloth->points[x][y].f �ɁA��C��R�ɂ��͂����Z����
			// ��C��R�ɂ��͂͑��x�ɒ萔Dk�����������́B�����������͑��x�Ƌt����
			// ���x�� cloth->points[x][y].v �ŕ\�����
			Vector3d newv = Dk * cloth->points[x][y].v;
      cloth->points[x][y].f -= newv;
		}
	}
	
	// �����܂łŁA���_�ɉ����͂����ׂČv�Z���I�����

	// �S�Ă̎��_�ɂ��ď��Ԃɏ�������
	for(int y = 0; y < POINT_NUM; y++) {
		for(int x = 0; x < POINT_NUM; x++) {
			// ���_���Œ肳��Ă���ꍇ�͉������Ȃ�
			// if(cloth->points[x][y].bFixed) continue;			
			// 2. ���_�̉����x�i�x�N�g���j���v�Z (�̓x�N�g�� cloth->points[x][y].f �����ʂŊ������l)
      Vector3d acceralation = cloth->points[x][y].f / Mass;
			// 3. ���_�̑��x (cloth->points[x][y].v) �������x�Ɋ�Â��čX�V����
      cloth->points[x][y].v += acceralation * dT;
			// 4. ���_�̈ʒu (cloth->points[x][y].p) �𑬓x�Ɋ�Â��čX�V����
      cloth->points[x][y].p += cloth->points[x][y].v * dT;
			// �I�v�V����. ���̂̓����ɓ���悤�Ȃ�A�����I�ɊO�Ɉړ�������
      if (cloth->points[x][y].p.length() < radius) {
        cloth->points[x][y].p -= cloth->points[x][y].v * dT;
        cloth->points[x][y].v = Vector3d(0,0,0);
      }
		}
	}
}
// -------------------------------------------------------------------
void motion(int x, int y) {
	int diffX = x - preMousePositionX;
	int diffY = y - preMousePositionY;

	rotateAngleH_deg += diffX * 0.1;
	rotateAngleV_deg += diffY * 0.1;

	preMousePositionX = x;
	preMousePositionY = y;
	glutPostRedisplay();
}

// ��莞�Ԃ��ƂɎ��s�����
void timer(int value) {
	if(bRunning) {
		updateCloth();
		glutPostRedisplay();
	}

	glutTimerFunc(10 , timer , 0);
}

void init(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

int main(int argc, char *argv[]) {

	bRunning = true;
	cloth = new Cloth();

	glutInit(&argc, argv);
	glutInitWindowSize(600,600);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutTimerFunc(10 , timer , 0);

	init();
	glutMainLoop();
	return 0;
}
