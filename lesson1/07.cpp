#include <GL/glut.h> // ライブラリ用ヘッダファイルの読み込み

// 表示部分をこの関数で記入
void display(void) {        
  float x = -0.6;
  float y = 0.6;
  int xcount = 0;
  int ycount = 0;

  glClearColor (1.0, 1.0, 1.0, 1.0);  // 消去色指定
	glClear (GL_COLOR_BUFFER_BIT );     // 画面消去

  while(ycount < 5) {
    while (xcount < 5) {
      glColor3d(0.0, 0.0, 1.0);
      glBegin(GL_TRIANGLES);
      glVertex2d( x, y + 0.3); // 頂点位置の指定(1つめ)
      glVertex2d( x - 0.3, y); // 頂点位置の指定(2つめ)
      glVertex2d( x, y); // 頂点位置の指定(3つめ)
      glEnd();
      x += 0.3;
      xcount++;
    }
    xcount = 0;
    ycount++;
    x = -0.6;
    y -= 0.3;
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
