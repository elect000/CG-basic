#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define OUTPUT_FILENAME "parametric_surface.obj" // 出力ファイル名
#define PI 3.1415926535

#define NUM_U 50  // U方向の分割数
#define NUM_V 50  // V方向の分割数

double x[NUM_U+1][NUM_V+1]; // x 座標
double y[NUM_U+1][NUM_V+1]; // y 座標
double z[NUM_U+1][NUM_V+1]; // z 座標

// 配列に座標値を設定する
// void setCoordinates() {

// 	for(int i = 0; i < NUM_U+1; i++) {
// 		for(int j = 0; j < NUM_V+1; j++) {

// 			// u と v の値を 0.0 ～ 1.0 に正規化する
// 			double u = 1.0 / NUM_U * i;
// 			double v = 1.0 / NUM_V * j;

// 			// 座標値の設定 (平面)
// 			x[i][j] = u;
// 			y[i][j] = v;
// 			z[i][j] = 0;
// 		}
// 	}
// }

// (a) 波紋
// void setCoordinates() {

// 	for(int i = 0; i < NUM_U+1; i++) {
// 		for(int j = 0; j < NUM_V+1; j++) {

// 			// u と v の値を 0.0 ～ 1.0 に正規化する
// 			double u = 1.0 / NUM_U * i - 0.5;
// 			double v = 1.0 / NUM_V * j - 0.5;

// 			// 座標値の設定 (平面)
// 			x[i][j] = u;
// 			y[i][j] = v;
// 			z[i][j] = 0.1
//         * sin ( 8 *
//                 sqrt (((u - (1/2)) * (u - (1/2)))
//                       + ((v - (1/2)) * (v - (1/2))))
//                 * PI);
// 		}
// 	}
// }
// (b) ガウス関数
// void setCoordinates() {

// 	for(int i = 0; i < NUM_U+1; i++) {
// 		for(int j = 0; j < NUM_V+1; j++) {

// 			// u と v の値を 0.0 ～ 1.0 に正規化する
// 			double u = 1.0 / NUM_U * i - 0.5;
// 			double v = 1.0 / NUM_V * j - 0.5;

// 			// 座標値の設定 (平面)
// 			x[i][j] = u;
// 			y[i][j] = v;
// 			z[i][j] = 0.5 * exp ( -1 * ((((u - 1/2) * (u - 1/2))
//                                   + ((v - 1/2) * (v - 1/2)))
//                                  / 0.1));
// 		}
// 	}
// }
// (c) 球
// void setCoordinates() {

// 	for(int i = 0; i < NUM_U+1; i++) {
// 		for(int j = 0; j < NUM_V+1; j++) {

// 			// u と v の値を 0.0 ～ 1.0 に正規化する
// 			double u = 1.0 / NUM_U * i * (2 * PI);
// 			double v = 1.0 / NUM_V * j * PI - (PI / 2);

// 			// 座標値の設定 (平面)
// 			x[i][j] = cos (u) * cos (v);
// 			y[i][j] = sin (u) * cos (v);
// 			z[i][j] = sin (v);
// 		}
// 	}
// }
// (1) トーラスモデル
void setCoordinates() {

	for(int i = 0; i < NUM_U+1; i++) {
		for(int j = 0; j < NUM_V+1; j++) {

			// u と v の値を 0.0 ～ 1.0 に正規化する
			double u = 1.0 / NUM_U * i * (2 * PI);
			double v = 1.0 / NUM_V * j * (2 * PI);

			// 座標値の設定 (平面)
			x[i][j] = (10 + 5 * cos (v)) * cos (u);
			y[i][j] = (10 + 5 * cos (v)) * sin (u);
			z[i][j] = 5 * sin (v);
		}
	}
}


// OBJ 形式でのファイルの出力
void exportOBJ() {
	FILE *fout;

	// 出力用ファイルを開く
	//---------------------------
	fout = fopen(OUTPUT_FILENAME, "w");

	// ファイルを作成できなかった場合のエラー処理
	if(fout == NULL) {
		printf("Error: fopen\n");
		exit(0);
	}
	
	
	// VS2013の場合は上記の内容を次のものに置き換える
/*
	// 出力用ファイルを開く
	//---------------------------
	if(0 != fopen_s(&fout, OUTPUT_FILENAME, "w") ) {
		// ファイルを作成できなかった場合のエラー処理
		printf("Error: fopen \n");
		exit(0);
	}
*/	

	// 頂点情報の出力
	//---------------------------
	for(int i = 0; i < NUM_U+1; i++) {
		for(int j = 0; j < NUM_V+1; j++) {
			fprintf(fout, "v %f %f %f\n", x[i][j], y[i][j] , z[i][j]);
		}
	}

	// 面情報の出力
	/*
    	★ 面情報を出力するには、以下のコメントアウトを外して、面を構成する頂点番号の情報を出力する
	*/
	//---------------------------
	
  for(int i = 0; i < NUM_U; i++) {
    for(int j = 0; j < NUM_V; j++) {
      int lb_index = (NUM_V + 1) * i + j + 1;  // 左下の頂点番号番号の計算式
      int lt_index = (NUM_V + 1) * i + j + 1 + (NUM_V + 1); // 左上の頂点番号番号の計算式
      int rb_index = (NUM_V + 1) * i + j; // 右下の頂点番号番号の計算式
      int rt_index = (NUM_V + 1) * i + j + (NUM_V + 1); // 右上の頂点番号番号の計算式

      // 三角形を構成する頂点番号を出力（OBJ形式の場合はインデックスが1から始まる）
      fprintf(fout, "f %d %d %d\n", lb_index+1, rt_index+1, lt_index+1);
      fprintf(fout, "f %d %d %d\n", lb_index+1, rb_index+1, rt_index+1);
    }
  }

	// 出力用ファイルを閉じる
	//---------------------------
	fclose(fout);
}


int main(int argc, char** argv) {
	setCoordinates();
	exportOBJ();
	return 0;
}
