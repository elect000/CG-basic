#include <GL/glut.h> // ライブラリ用ヘッダファイルの読み込み
#include <math.h>

// 表示部分をこの関数で記入
void display(void) {        
	glClearColor (1.0, 1.0, 1.0, 1.0);  // 消去色指定
	glClear (GL_COLOR_BUFFER_BIT );     // 画面消去

  // 胴の描画
	glColor3d(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	for(int i = 0; i < 360; i++) {
		double x = cos(i * 3.14159 /180.0);
		double y = sin(i * 3.14159 /180.0) - 0.5;
    glVertex2d(x * 0.6, y * 0.6); 
	}
	glEnd();

  // 頭の描画
	glColor3d(0.0, 0.0, 0.0); 
	glBegin(GL_LINE_LOOP);
	for(int i = 0; i < 360; i++) {
		double x = cos(i * 3.14159 /180.0) * 0.5;
		double y = (sin(i * 3.14159 /180.0) + 2.0) * 0.5;
    glVertex2d(x * 0.6, y * 0.6); 
	}
	glEnd();
  glColor3d(0.0, 0.0, 0.0); 
	glBegin(GL_POLYGON);
	for(int i = 0; i < 360; i++) {
		double x = cos(i * 3.14159 /180.0) * 0.1 + 0.25;
		double y = sin(i * 3.14159 /180.0) * 0.1 + 1.0;
    glVertex2d(x * 0.6, y * 0.6); 
	}
	glEnd();

  // 目の描画
  glColor3d(0.0, 0.0, 0.0); 
	glBegin(GL_POLYGON);
	for(int i = 0; i < 360; i++) {
    double x = cos(i * 3.14159 /180.0) * 0.1 - 0.25;
		double y = sin(i * 3.14159 /180.0) * 0.1 + 1.0;
    glVertex2d(x * 0.6, y * 0.6); 
	}
	glEnd();
  glColor3d(0.0, 0.0, 0.0); 
	glBegin(GL_POLYGON); 
  glVertex2d( 0.0, 0.5);
  glVertex2d( -0.05, 0.4);
  glVertex2d( 0.05,  0.4);
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
