#ifndef MATRIZEAK_H
#define MATRIZEAK_H 

GLdouble* biderkatu(GLdouble* transformazioa);
GLdouble* biderkatuLokalki(GLdouble* transformazioa);
void translazioa(int tekla, char ref_sys);
void biraketa(int tekla, char ref_sys);
void eskalatu(int tekla, char ref_sys);

//void inprimatu(GLdouble* matrizea); Momentuz ez dugu behar

#endif //MATRIZEAK_H
