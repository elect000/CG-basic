#include <GL/glut.h>
#include <math.h>

// 表示部分をこの関数で記入

void koch(int level, float p1x, float p1y,  float p2x, float p2y) {
  if(level == 0) {
    glColor3d(0.0, 0.0, 1.0);
    glBegin(GL_LINE_LOOP);
    glVertex2d(p1x, p1y);
    glVertex2d(p2x, p2y);
    glEnd();
    return;
  }
  float sx = (2.0 * p1x + 1.0 * p2x) / 3.0;
  float sy = (2.0 * p1y + 1.0 * p2y) / 3.0;
  float tx = (1.0 * p1x + 2.0 * p2x) / 3.0;
  float ty = (1.0 * p1y + 2.0 * p2y) / 3.0;
  float ux = (tx - sx) * (1.0 / 2.0) - (ty - sy) * (sqrt(3) / 2.0) + sx;
  float uy = (tx - sx) * (sqrt(3.0) / 2.0) + (ty - sy) * (1.0 / 2.0) + sy;

  koch(level - 1, p1x, p1y, sx, sy);
  koch(level - 1, sx, sy, ux, uy);
  koch(level - 1, ux, uy, tx, ty);
  koch(level - 1, tx, ty, p2x, p2y);
}
void display(void) {
  glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear (GL_COLOR_BUFFER_BIT );     // 画面消去
  koch(3, 0.5 * 1.0, 0.5 * -1.0 * (sqrt(3.0) / 2.0),
       0.5 * -1.0, 0.5 * -1.0 * (sqrt(3.0) / 2.0));
  koch(3, -1.0 * 0.5, 0.5 * -1.0 * (sqrt(3.0) / 2.0),
       0.0 * 0.5, (sqrt(3.0) / 2.0) * 0.5);
  koch(3, 0.0 * 0.5, 0.5 * (sqrt(3.0) / 2.0),
       0.5 * 1.0, 0.5 * -1.0 * (sqrt(3.0) / 2.0));
  
  glFlush(); // 画面出力
}

// メインプログラム
int main (int argc, char *argv[]) { 
	glutInit(&argc, argv);          // ライブラリの初期化
	glutInitWindowSize(400 , 400);  // ウィンドウサイズを指定
	glutCreateWindow(argv[0]);      // ウィンドウを作成
	glutDisplayFunc(display);       // 表示関数を指定
	glutMainLoop();                 // イベント待ち
	return 0;
}
