#include "definitions.h"
#include "load_obj.h"
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

extern object3d * _first_object;
extern object3d * _selected_object;

extern PerspCam *objKam;
extern PerspCam *ibilKam;
extern char kam_mota;
extern light * bonbila;
extern light * eguzkia;
extern light * fokua;
extern char argi_mota;



GLdouble* biderkatuObjKamBektorea(GLdouble *bektorea){

    GLdouble *lag = Stack_Top(objKam->stack);
    GLdouble batura=0;
    GLdouble *bek = malloc (sizeof(GLdouble)*4);
    int i,j;
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){
            batura=batura + lag[i+j*4]*bektorea[j];
        }
        bek[i]=batura;
        batura=0;
    }
    return bek;
}

GLdouble* biderkatuIbilKamBektorea(GLdouble *bektorea){

    GLdouble *lag = Stack_Top(ibilKam->stack);
    GLdouble batura=0;
    GLdouble *bek = malloc(sizeof(GLdouble)*4);
    int i,j;
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){
            batura=batura + lag[i+j*4]*bektorea[j];
        }
        bek[i]=batura;
        batura=0;
    }
    return bek;
}

void eguneratu_objKam(){

    GLdouble *eyeLag = malloc(sizeof(GLdouble)*4);
    eyeLag[0]=0;   eyeLag[1]=0;   eyeLag[2]=0; eyeLag[3]=1;
    GLdouble *centerLag = malloc(sizeof(GLdouble)*4);
    centerLag[0]=0;    centerLag[1]=0;    centerLag[2]=-30; centerLag[3]=1;
    GLdouble *upLag = malloc(sizeof(GLdouble)*4);
    upLag[0]=0;    upLag[1]=1;    upLag[2]=0; upLag[3]=0;

    objKam->posizioa=biderkatuObjKamBektorea(eyeLag);
    objKam->begira=biderkatuObjKamBektorea(centerLag);
    objKam->gora=biderkatuObjKamBektorea(upLag);

}

void eguneratu_ibilKam(){

    GLdouble *eyeLag = malloc(sizeof(GLdouble)*4);
    eyeLag[0]=0;   eyeLag[1]=0;   eyeLag[2]=0; eyeLag[3]=1;
    GLdouble *centerLag = malloc(sizeof(GLdouble)*4);
    centerLag[0]=0;    centerLag[1]=0;    centerLag[2]=-30; centerLag[3]=1;
    GLdouble *upLag = malloc(sizeof(GLdouble)*4);
    upLag[0]=0;    upLag[1]=1;    upLag[2]=0; upLag[3]=0;

    ibilKam->posizioa=biderkatuIbilKamBektorea(eyeLag);
    ibilKam->begira=biderkatuIbilKamBektorea(centerLag);
    ibilKam->gora=biderkatuIbilKamBektorea(upLag);

}


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

GLdouble* biderkatuKam(GLdouble * transformazioa){
	GLdouble * biderkatuta = malloc(sizeof(GLdouble)*16);
	GLdouble * stackTop;
	if (kam_mota == 'b') stackTop = Stack_Top(objKam-> stack);
	else stackTop = Stack_Top(ibilKam-> stack);
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

GLdouble* biderkatuArgia(Stack * p, GLdouble * transformazioa){
	GLdouble * biderkatuta = malloc(sizeof(GLdouble)*16);
	GLdouble * stackTop = Stack_Top(p);
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

GLdouble* biderkatuKamLokalki(GLdouble * transformazioa){
	GLdouble * biderkatuta = malloc(sizeof(GLdouble)*16);
	GLdouble * stackTop;
	if (kam_mota == 'b') stackTop = Stack_Top(objKam-> stack);
	else stackTop = Stack_Top(ibilKam-> stack);
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


void translazioaArgia(int tekla){
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


	if (argi_mota=='b') Stack_Push(bonbila-> stack,biderkatuArgia(bonbila->stack,m)); 
	else if (argi_mota == 'e') Stack_Push(eguzkia-> stack,biderkatuArgia(eguzkia->stack,m));
	else if (argi_mota == 'f') Stack_Push(fokua-> stack,biderkatuArgia(fokua->stack,m));

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

void translazioaKam(int tekla, char ref_sys){
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
	if (kam_mota == 'b'){
		if (ref_sys == 'g') Stack_Push(objKam-> stack,biderkatuKam(m)); //Erreferentzi sistema globalean egin eragiketa eta ondoren pilaratu
		else if (ref_sys == 'l') Stack_Push(objKam-> stack,biderkatuKamLokalki(m)); //Erreferentzi sistema lokalean egin eragiketa eta pilaratu
	} else {
		Stack_Push(ibilKam-> stack,biderkatuKam(m));
	}
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

void biraketaKam(int tekla, char ref_sys){
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
			m[5] = cos(M_PI/32);
			m[6] = sin(M_PI/32);
			m[9] = -sin(M_PI/32);
			m[10] = cos(M_PI/32);
			break;
		case GLUT_KEY_DOWN: //tekla behera
			m[5] = cos(-M_PI/32);
			m[6] = sin(-M_PI/32);
			m[9] = sin(M_PI/32);
			m[10] = cos(-M_PI/32);
			break;
		case GLUT_KEY_LEFT: //tekla ezkerrera
			m[0] = cos(-M_PI/32);
			m[2] = sin(M_PI/32);
			m[8] = sin(-M_PI/32);
			m[10] = cos(-M_PI/32);
			break;
		case GLUT_KEY_RIGHT: //tekla eskubira
			m[0] = cos(M_PI/32);
			m[2] = -sin(M_PI/32);
			m[8] = sin(M_PI/32);
			m[10] = cos(M_PI/32);
			break;
		case GLUT_KEY_PAGE_UP:
			m[0] = cos(M_PI/32);
			m[1] = sin(M_PI/32);
			m[4] = -sin(M_PI/32);
			m[5] = cos(M_PI/32);
			break;
		case GLUT_KEY_PAGE_DOWN:
			m[0] = cos(-M_PI/32);
			m[1] = sin(-M_PI/32);
			m[4] = sin(M_PI/32);
			m[5] = cos(-M_PI/32);
			break;
	}

	if (kam_mota == 'b'){
		if (ref_sys == 'g') Stack_Push(objKam-> stack,biderkatuKam(m)); //Erreferentzi sistema globalean egin eragiketa eta ondoren pilaratu
		else if (ref_sys == 'l') Stack_Push(objKam-> stack,biderkatuKamLokalki(m)); //Erreferentzi sistema lokalean egin eragiketa eta pilaratu
	} else {
		Stack_Push(ibilKam-> stack,biderkatuKamLokalki(m));
	}
}


void biraketaArgia(int tekla){
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

	if (argi_mota=='b') Stack_Push(bonbila-> stack,biderkatuArgia(bonbila->stack,m)); 
	else if (argi_mota == 'e') Stack_Push(eguzkia-> stack,biderkatuArgia(eguzkia->stack,m));
	else if (argi_mota == 'f') Stack_Push(fokua-> stack,biderkatuArgia(fokua->stack,m));

	
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

