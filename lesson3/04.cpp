#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <cstdlib>
#include <GL/glut.h>
#define _USE_MATH_DEFINES	// Visual Studio で 円周率として定数 M_PI を使用するため
#include <cmath>


// float型の値は、数字の後ろにfを付ける。末尾のゼロは省略できる
const float g_TeapotSize = 1.f;
const float g_InnerRadius = 6.f;
const float g_OuterRadius = 7.5f;
const float g_HeightAmplitude = 0.8f;
const float g_HeightOffset = 0.2f;

const float g_EyeCenterY = 9.f;
const float g_EyeCenterZ = 30.f;
const float g_EyeRadius = 8.f;
float g_EyeY, g_EyeZ;

const int g_AnimationIntervalMsec = 10;

float g_RotationDegree = 0.f;
const float g_DeltaRotationDegree = 0.3; // 0.3f ?????

int g_WindowWidth = 512;
int g_WindowHeight = 512;

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
	// 透視投影変換の設定
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, g_WindowWidth/(double)g_WindowHeight, 1, 100.0);

	// モデル座標の操作へモード切り替え
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, g_EyeY, g_EyeZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	float ambientColor[] = { 0.4f, 0.2f, 0.2f, 1.0f };
	float diffuseColor[] = { 1.f, 0.8f, 0.8f, 1.0f };
	float specularColor[] = { 0.4f, 0.3f, 0.3f, 1.0f };
	float shininess = 5.f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularColor);
	glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);


	// 屋根の上のティーポット
	glPushMatrix();
	glTranslatef(0, g_HeightAmplitude + g_HeightOffset + 5.5f, 0);
	glRotatef(g_RotationDegree, 0, 1, 0); // 回転させている
	glutSolidTeapot(g_TeapotSize);
	glPopMatrix();


	
	glutSwapBuffers();
}

float frand() { return rand() / (float)RAND_MAX; }

// 初期設定を行う関数
void init() {
	glClearColor(1, 1, 1, 1);
	glClearDepth(100.f);

	float lightAmbientColor0[] = { 0.2f, 0.2f, 0.2f, 0.0f };
	float lightDiffuseColor0[] = { 0.4f, 0.4f, 0.4f, 0.0f };
	float lightSpecularColor0[] = { 0.8f, 0.8f, 0.8f, 0.0f };
	float lightPosition0[] = { 5.0f, 5.0f, 8.0f, 0.0f };

	float lightAmbientColor1[] = { 0.2f, 0.2f, 0.2f, 0.0f };
	float lightDiffuseColor1[] = { 0.4f, 0.4f, 0.4f, 0.0f };
	float lightSpecularColor1[] = { 0.8f, 0.8f, 0.8f, 0.0f };
	float lightPosition1[] = { -5.0f, 2.0f, 3.0f, 0.0f };

	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbientColor0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuseColor0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecularColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition0);

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbientColor1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuseColor1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecularColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1);


	glEnable(GL_DEPTH_TEST);
}

// 一定時間ごとに実行される関数
void timer(int val) {
	// 回転角度の更新
	g_RotationDegree += g_DeltaRotationDegree;

	const float rotationRad = 2.f * g_RotationDegree * M_PI / 180.f;
	
	// ★ 下のコードでは視点が固定だけど
	// ここで  g_EyeY と g_EyeZ の値を変えることで視点位置を変化させることができる
	g_EyeY = g_EyeCenterY * (1.0 + sinf(0.05  * g_RotationDegree));
	g_EyeZ = g_EyeCenterZ * (1 + 0.05 *  sinf(0.05  * g_RotationDegree));

	glutPostRedisplay();

	glutTimerFunc(g_AnimationIntervalMsec, timer, val);
}

// ウィンドウサイズが変更されたときの処理
void reshape(int w, int h) {
	if (h < 1) return;

	// ビューポートをウィンドウサイズに変更
	glViewport(0, 0, w, h);
	g_WindowWidth = w;
	g_WindowHeight = h;
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(g_WindowWidth, g_WindowHeight);

	// ウィンドウタイトルに表示する文字列を指定する場合
	glutCreateWindow("Teapot Merry-Go-Round");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape); // ウィンドウサイズが変更されたときに実行される関数を指定
	glutTimerFunc(g_AnimationIntervalMsec, timer, 0);

	// 初期設定を行う
	init();

	glutMainLoop();

	return 0;
}
