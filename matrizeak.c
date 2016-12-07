#include "definitions.h"
#include "load_obj.h"
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

extern object3d * _first_object;
extern object3d * _selected_object;

extern PerspCam *objKam;


GLdouble* biderkatu(GLdouble * transformazioa){
	GLdouble * biderkatuta = malloc(sizeof(GLdouble)*16);
	GLdouble * stackTop = Stack_Top(_selected_object-> stack);
	int i, j;
	int k = 0;

	for(i = 0;i<16;i+=4){
		for(j=0;j<4;j++){
			biderkatuta[k] = stackTop[i]*transformazioa[j]+stackTop[i+1]*transformazioa[j+4]+stackTop[i+2]*transformazioa[j+8]+stackTop[i+3]*transformazioa[j+12];
			k++;
		}
	}
	return biderkatuta;
}


GLdouble* biderkatuLokalki(GLdouble * transformazioa){
	GLdouble * biderkatuta = malloc(sizeof(GLdouble)*16);
	GLdouble * stackTop = Stack_Top(_selected_object-> stack);
	int i, j;
	int k = 0;

	for(i = 0;i<16;i+=4){
		for(j=0;j<4;j++){
			biderkatuta[k] = transformazioa[i] * stackTop[j] + transformazioa[i+1]*stackTop[j+4] + transformazioa[i+2]*stackTop[j+8]+transformazioa[i+3]*stackTop[j+12];
			k++;
		}
	}
	return biderkatuta;
}

void translazioa(int tekla, char ref_sys){
	GLdouble * m = malloc(sizeof(GLdouble)*16);

	/*  IDENTITATE MATRIZEA
		1  0  0  0
		0  1  0  0
		0  0  1  0
		0  0  0  1
	*/
		
	m[0] = 1;  m[4] = 0;  m[8] = 0;  m[12] = 0;
	m[1] = 0;  m[5] = 1;  m[9] = 0;  m[13] = 0;
	m[2] = 0;  m[6] = 0;  m[10] = 1;  m[14] = 0;
	m[3] = 0;  m[7] = 0;  m[11] = 0;  m[15] = 1;

	// Teklaren arabera aldatu matrizea
	switch(tekla){
		case GLUT_KEY_UP: //Tekla gora
			m[13] = 1;
			break;
		case GLUT_KEY_DOWN: //Tekla bera
			m[13] = -1;
			break;
		case GLUT_KEY_LEFT: //Tekla ezkerrera
			m[12] = -1;
			break;
		case GLUT_KEY_RIGHT: //Tekla eskubira
			m[12] = 1;
			break;
		case GLUT_KEY_PAGE_UP:
			m[14] = 1;
			break;
		case GLUT_KEY_PAGE_DOWN:
			m[14] = -1;
			break;
	}

	if (ref_sys == 'g') Stack_Push(_selected_object-> stack,biderkatu(m)); //Erreferentzi sistema globalean egin eragiketa eta ondoren pilaratu
	else if (ref_sys == 'l') Stack_Push(_selected_object-> stack,biderkatuLokalki(m)); //Erreferentzi sistema lokalean egin eragiketa eta pilaratu
}

void biraketa(int tekla, char ref_sys){
	GLdouble * m = malloc(sizeof(GLdouble)*16);

	/*  IDENTITATE MATRIZEA
		1  0  0  0
		0  1  0  0
		0  0  1  0
		0  0  0  1
	*/
	m[0] = 1;  m[4] = 0;  m[8] = 0;  m[12] = 0;
	m[1] = 0;  m[5] = 1;  m[9] = 0;  m[13] = 0;
	m[2] = 0;  m[6] = 0;  m[10] = 1;  m[14] = 0;
	m[3] = 0;  m[7] = 0;  m[11] = 0;  m[15] = 1;

	//Teklaren arabera aldatu matrizea
	switch(tekla){
		case GLUT_KEY_UP: //tekla gora
			m[5] = cos(M_PI/8);
			m[6] = sin(M_PI/8);
			m[9] = -sin(M_PI/8);
			m[10] = cos(M_PI/8);
			break;
		case GLUT_KEY_DOWN: //tekla behera
			m[5] = cos(-M_PI/8);
			m[6] = sin(-M_PI/8);
			m[9] = sin(M_PI/8);
			m[10] = cos(-M_PI/8);
			break;
		case GLUT_KEY_LEFT: //tekla ezkerrera
			m[0] = cos(-M_PI/8);
			m[2] = sin(M_PI/8);
			m[8] = sin(-M_PI/8);
			m[10] = cos(-M_PI/8);
			break;
		case GLUT_KEY_RIGHT: //tekla eskubira
			m[0] = cos(M_PI/8);
			m[2] = -sin(M_PI/8);
			m[8] = sin(M_PI/8);
			m[10] = cos(M_PI/8);
			break;
		case GLUT_KEY_PAGE_UP:
			m[0] = cos(M_PI/8);
			m[1] = sin(M_PI/8);
			m[4] = -sin(M_PI/8);
			m[5] = cos(M_PI/8);
			break;
		case GLUT_KEY_PAGE_DOWN:
			m[0] = cos(-M_PI/8);
			m[1] = sin(-M_PI/8);
			m[4] = sin(M_PI/8);
			m[5] = cos(-M_PI/8);
			break;
	}

	if (ref_sys == 'g') Stack_Push(_selected_object-> stack,biderkatu(m)); 
	else if (ref_sys == 'l') Stack_Push(_selected_object-> stack,biderkatuLokalki(m));
}


void eskalatu(int tekla, char ref_sys){
	GLdouble * m = malloc(sizeof(GLdouble)*16);

	/*  IDENTITATE MATRIZEA
		1  0  0  0
		0  1  0  0
		0  0  1  0
		0  0  0  1
	*/
	m[0] = 1;  m[4] = 0;  m[8] = 0;  m[12] = 0;
	m[1] = 0;  m[5] = 1;  m[9] = 0;  m[13] = 0;
	m[2] = 0;  m[6] = 0;  m[10] = 1;  m[14] = 0;
	m[3] = 0;  m[7] = 0;  m[11] = 0;  m[15] = 1;

	//Treklaren arabera aldaketak egin
	switch(tekla){
		case GLUT_KEY_UP: //tekla gora
			m[5] = 1.2;
			break;
		case GLUT_KEY_DOWN: //tekla behera
			m[5] = 0.8;
			break;
		case GLUT_KEY_LEFT: //tekla ezkerrera
			m[0] = 0.8;
			break;
		case GLUT_KEY_RIGHT: //tekla eskubira
			m[0] = 1.2;
			break;
		case GLUT_KEY_PAGE_UP:
			m[10] = 1.2;
			break;
		case GLUT_KEY_PAGE_DOWN:
			m[10] = 0.8;
			break;
	}

	if (ref_sys == 'g') Stack_Push(_selected_object-> stack,biderkatu(m));
	else if (ref_sys == 'l') Stack_Push(_selected_object-> stack,biderkatuLokalki(m));
}

void kamaraBiderkatu(){
	GLdouble* matrizeaTop = malloc((sizeof(GLdouble))*16);
	matrizeaTop = Stack_Top(objKam -> stack);

	GLdouble* lagMatrix = malloc((sizeof(GLdouble))*16);

	int i;
	for(i=0;i<16;i++)
		lagMatrix[i] = 0;

	/*lagMatrix[2] = 10;
	lagMatrix[3] = 1;
	lagMatrix[7] = 1;
	lagMatrix[9] = 1;*/
	lagMatrix[0] = 1;
	lagMatrix[5] = 1;
	lagMatrix[10] = 1;
	lagMatrix[15] = 1;

	GLdouble* erantzuna = malloc((sizeof(GLdouble))*16);

	erantzuna[0] = lagMatrix[0]*matrizeaTop[0] + lagMatrix[1]*matrizeaTop[4] + lagMatrix[2]*matrizeaTop[8] + lagMatrix[3]*matrizeaTop[12];
	erantzuna[1] = lagMatrix[0]*matrizeaTop[1] + lagMatrix[1]*matrizeaTop[5] + lagMatrix[2]*matrizeaTop[9] + lagMatrix[3]*matrizeaTop[13];
	erantzuna[2] = lagMatrix[0]*matrizeaTop[2] + lagMatrix[1]*matrizeaTop[6] + lagMatrix[2]*matrizeaTop[10] + lagMatrix[3]*matrizeaTop[14];
	erantzuna[3] = lagMatrix[0]*matrizeaTop[3] + lagMatrix[1]*matrizeaTop[7] + lagMatrix[2]*matrizeaTop[11] + lagMatrix[3]*matrizeaTop[15];
	erantzuna[4] = lagMatrix[4]*matrizeaTop[0] + lagMatrix[5]*matrizeaTop[4] + lagMatrix[6]*matrizeaTop[8] + lagMatrix[7]*matrizeaTop[12];
	erantzuna[5] = lagMatrix[4]*matrizeaTop[1] + lagMatrix[5]*matrizeaTop[5] + lagMatrix[6]*matrizeaTop[9] + lagMatrix[7]*matrizeaTop[13];
	erantzuna[6] = lagMatrix[4]*matrizeaTop[2] + lagMatrix[5]*matrizeaTop[6] + lagMatrix[6]*matrizeaTop[10] + lagMatrix[7]*matrizeaTop[14];
	erantzuna[7] = lagMatrix[4]*matrizeaTop[3] + lagMatrix[5]*matrizeaTop[7] + lagMatrix[6]*matrizeaTop[11] + lagMatrix[7]*matrizeaTop[15];
	erantzuna[8] = lagMatrix[8]*matrizeaTop[0] + lagMatrix[9]*matrizeaTop[4] + lagMatrix[10]*matrizeaTop[8] + lagMatrix[11]*matrizeaTop[12];
	erantzuna[9] = lagMatrix[8]*matrizeaTop[1] + lagMatrix[9]*matrizeaTop[5] + lagMatrix[10]*matrizeaTop[9] + lagMatrix[11]*matrizeaTop[13];
	erantzuna[10] = lagMatrix[8]*matrizeaTop[2] + lagMatrix[9]*matrizeaTop[6] + lagMatrix[10]*matrizeaTop[10] + lagMatrix[11]*matrizeaTop[14];
	erantzuna[11] = lagMatrix[8]*matrizeaTop[3] + lagMatrix[9]*matrizeaTop[7] + lagMatrix[10]*matrizeaTop[11] + lagMatrix[11]*matrizeaTop[15];
	erantzuna[12] = lagMatrix[12]*matrizeaTop[0] + lagMatrix[13]*matrizeaTop[4] + lagMatrix[14]*matrizeaTop[8] + lagMatrix[15]*matrizeaTop[12];
	erantzuna[13] = lagMatrix[12]*matrizeaTop[1] + lagMatrix[13]*matrizeaTop[5] + lagMatrix[14]*matrizeaTop[9] + lagMatrix[15]*matrizeaTop[13];
	erantzuna[14] = lagMatrix[12]*matrizeaTop[2] + lagMatrix[13]*matrizeaTop[6] + lagMatrix[14]*matrizeaTop[10] + lagMatrix[15]*matrizeaTop[14];
	erantzuna[15] = lagMatrix[12]*matrizeaTop[3] + lagMatrix[13]*matrizeaTop[7] + lagMatrix[14]*matrizeaTop[11] + lagMatrix[15]*matrizeaTop[15];


	Stack_Push(objKam -> stack, erantzuna);
}