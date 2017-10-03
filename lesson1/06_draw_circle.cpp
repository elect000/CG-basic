#include <GL/glut.h> // ライブラリ用ヘッダファイルの読み込み
#include <math.h>

// 表示部分をこの関数で記入
void display(void) {        
	glClearColor (1.0, 1.0, 1.0, 1.0);  // 消去色指定
	glClear (GL_COLOR_BUFFER_BIT );     // 画面消去

	glColor3d(0.0, 0.0, 0.0);   // 色指定(R,G,B)で0～1まで
	glBegin(GL_LINE_LOOP);
	for(int i = 0; i < 360; i++) {
		double x = cos(i * 3.14159 /180.0);
		double y = sin(i * 3.14159 /180.0);
    glVertex2d(x * 0.6, y * 0.6); 
	}
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
