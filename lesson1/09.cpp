#include <GL/glut.h> // ライブラリ用ヘッダファイルの読み込み
#include <math.h>

// 表示部分をこの関数で記入
void display(void) {
  float x = 1.0;
  float y = 1.0;
  float color = 1.0;

  glClearColor (1.0, 1.0, 1.0, 1.0);  // 消去色指定
	glClear (GL_COLOR_BUFFER_BIT );     // 画面消去

  for (int i = 0; i < 10; ++i) {
    // 塗りつぶす色を交互に入れ替える
    if (i % 2 == 0) {
      glColor3d(color, color, color);
    }
    else {
      glColor3d(color * 0, color * 0, color * 0);
    }
    // 塗り潰す四角形の描画
    glBegin(GL_QUADS);
    glVertex2d(x, y);
    glVertex2d(-1 * x , y);
    glVertex2d(-1 * x , -1 * y);
    glVertex2d(x , -1 * y);
    glEnd();
    // 正方形のサイズを小さくする。
    x -= 0.1;
    y -= 0.1;
  }

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
