#include <GL/glut.h> // ライブラリ用ヘッダファイルの読み込み

// 表示部分をこの関数で記入
void display(void) {        
	glClearColor (1.0, 1.0, 1.0, 1.0);  // 消去色指定
	glClear (GL_COLOR_BUFFER_BIT );     // 画面消去

	glColor3d(0.0, 1.0, 1.0);   // 色指定(R,G,B)で0～1まで
	glBegin(GL_TRIANGLES);      // 描画するものを指定
  glVertex2d(-0.9, -0.8); // 頂点位置の指定(1つめ)
  glVertex2d( 0.9, -0.8); // 頂点位置の指定(2つめ)
  glVertex2d( 0.0,  0.8); // 頂点位置の指定(3つめ) 
	glEnd();                               

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
