#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define OUTPUT_FILENAME "parametric_surface.obj" // �o�̓t�@�C����
#define PI 3.1415926535

#define NUM_U 50  // U�����̕�����
#define NUM_V 50  // V�����̕�����

double x[NUM_U+1][NUM_V+1]; // x ���W
double y[NUM_U+1][NUM_V+1]; // y ���W
double z[NUM_U+1][NUM_V+1]; // z ���W

// �z��ɍ��W�l��ݒ肷��
// void setCoordinates() {

// 	for(int i = 0; i < NUM_U+1; i++) {
// 		for(int j = 0; j < NUM_V+1; j++) {

// 			// u �� v �̒l�� 0.0 �` 1.0 �ɐ��K������
// 			double u = 1.0 / NUM_U * i;
// 			double v = 1.0 / NUM_V * j;

// 			// ���W�l�̐ݒ� (����)
// 			x[i][j] = u;
// 			y[i][j] = v;
// 			z[i][j] = 0;
// 		}
// 	}
// }

// (a) �g��
// void setCoordinates() {

// 	for(int i = 0; i < NUM_U+1; i++) {
// 		for(int j = 0; j < NUM_V+1; j++) {

// 			// u �� v �̒l�� 0.0 �` 1.0 �ɐ��K������
// 			double u = 1.0 / NUM_U * i - 0.5;
// 			double v = 1.0 / NUM_V * j - 0.5;

// 			// ���W�l�̐ݒ� (����)
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
// (b) �K�E�X�֐�
// void setCoordinates() {

// 	for(int i = 0; i < NUM_U+1; i++) {
// 		for(int j = 0; j < NUM_V+1; j++) {

// 			// u �� v �̒l�� 0.0 �` 1.0 �ɐ��K������
// 			double u = 1.0 / NUM_U * i - 0.5;
// 			double v = 1.0 / NUM_V * j - 0.5;

// 			// ���W�l�̐ݒ� (����)
// 			x[i][j] = u;
// 			y[i][j] = v;
// 			z[i][j] = 0.5 * exp ( -1 * ((((u - 1/2) * (u - 1/2))
//                                   + ((v - 1/2) * (v - 1/2)))
//                                  / 0.1));
// 		}
// 	}
// }
// (c) ��
// void setCoordinates() {

// 	for(int i = 0; i < NUM_U+1; i++) {
// 		for(int j = 0; j < NUM_V+1; j++) {

// 			// u �� v �̒l�� 0.0 �` 1.0 �ɐ��K������
// 			double u = 1.0 / NUM_U * i * (2 * PI);
// 			double v = 1.0 / NUM_V * j * PI - (PI / 2);

// 			// ���W�l�̐ݒ� (����)
// 			x[i][j] = cos (u) * cos (v);
// 			y[i][j] = sin (u) * cos (v);
// 			z[i][j] = sin (v);
// 		}
// 	}
// }
// (1) �g�[���X���f��
void setCoordinates() {

	for(int i = 0; i < NUM_U+1; i++) {
		for(int j = 0; j < NUM_V+1; j++) {

			// u �� v �̒l�� 0.0 �` 1.0 �ɐ��K������
			double u = 1.0 / NUM_U * i * (2 * PI);
			double v = 1.0 / NUM_V * j * (2 * PI);

			// ���W�l�̐ݒ� (����)
			x[i][j] = (10 + 5 * cos (v)) * cos (u);
			y[i][j] = (10 + 5 * cos (v)) * sin (u);
			z[i][j] = 5 * sin (v);
		}
	}
}


// OBJ �`���ł̃t�@�C���̏o��
void exportOBJ() {
	FILE *fout;

	// �o�͗p�t�@�C�����J��
	//---------------------------
	fout = fopen(OUTPUT_FILENAME, "w");

	// �t�@�C�����쐬�ł��Ȃ������ꍇ�̃G���[����
	if(fout == NULL) {
		printf("Error: fopen\n");
		exit(0);
	}
	
	
	// VS2013�̏ꍇ�͏�L�̓��e�����̂��̂ɒu��������
/*
	// �o�͗p�t�@�C�����J��
	//---------------------------
	if(0 != fopen_s(&fout, OUTPUT_FILENAME, "w") ) {
		// �t�@�C�����쐬�ł��Ȃ������ꍇ�̃G���[����
		printf("Error: fopen \n");
		exit(0);
	}
*/	

	// ���_���̏o��
	//---------------------------
	for(int i = 0; i < NUM_U+1; i++) {
		for(int j = 0; j < NUM_V+1; j++) {
			fprintf(fout, "v %f %f %f\n", x[i][j], y[i][j] , z[i][j]);
		}
	}

	// �ʏ��̏o��
	/*
    	�� �ʏ����o�͂���ɂ́A�ȉ��̃R�����g�A�E�g���O���āA�ʂ��\�����钸�_�ԍ��̏����o�͂���
	*/
	//---------------------------
	
  for(int i = 0; i < NUM_U; i++) {
    for(int j = 0; j < NUM_V; j++) {
      int lb_index = (NUM_V + 1) * i + j + 1;  // �����̒��_�ԍ��ԍ��̌v�Z��
      int lt_index = (NUM_V + 1) * i + j + 1 + (NUM_V + 1); // ����̒��_�ԍ��ԍ��̌v�Z��
      int rb_index = (NUM_V + 1) * i + j; // �E���̒��_�ԍ��ԍ��̌v�Z��
      int rt_index = (NUM_V + 1) * i + j + (NUM_V + 1); // �E��̒��_�ԍ��ԍ��̌v�Z��

      // �O�p�`���\�����钸�_�ԍ����o�́iOBJ�`���̏ꍇ�̓C���f�b�N�X��1����n�܂�j
      fprintf(fout, "f %d %d %d\n", lb_index+1, rt_index+1, lt_index+1);
      fprintf(fout, "f %d %d %d\n", lb_index+1, rb_index+1, rt_index+1);
    }
  }

	// �o�͗p�t�@�C�������
	//---------------------------
	fclose(fout);
}


int main(int argc, char** argv) {
	setCoordinates();
	exportOBJ();
	return 0;
}
