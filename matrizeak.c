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

GLdouble* biderkatuKamera(){
	GLdouble * biderkatuta = malloc(sizeof(GLdouble)*16);
	GLdouble * stackTop = Stack_Top(objKam-> stack);
	printf("%.2f\n", stackTop[1]);
	int i, j;
	int k = 0;
	for(i = 0;i<16;i+=4){
		for(j=0;j<4;j++){
			biderkatuta[k] = stackTop[i]*stackTop[j]+stackTop[i+1]*stackTop[j+4]+stackTop[i+2]*stackTop[j+8]+stackTop[i+3]*stackTop[j+12];
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


/*void inprimatu(GLdouble* matrizea){

	printf("-----   MATRIZEA   ----\n");
	//printf("%f  %f  %f  %f  %f \n", matrizea[0], matrizea[4], matrizea[8], matrizea[12]);
	//printf("%f  %f  %f  %f  %f \n", matrizea[1], matrizea[5], matrizea[9], matrizea[13]);
	//printf("%f  %f  %f  %f  %f \n", matrizea[2], matrizea[6], matrizea[10], matrizea[14]);
	//printf("%f  %f  %f  %f  %f \n", matrizea[3], matrizea[7], matrizea[11], matrizea[15]);
	printf("-----------------------\n");
}*/