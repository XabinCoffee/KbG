#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "display.h"
#include "io.h"
#include "definitions.h"

/** GLOBAL VARIABLES **/

GLdouble _window_ratio;                     /*Control of window's proportions */
GLdouble _ortho_x_min,_ortho_x_max;         /*Variables for the control of the orthographic projection*/
GLdouble _ortho_y_min ,_ortho_y_max;        /*Variables for the control of the orthographic projection*/
GLdouble _ortho_z_min,_ortho_z_max;         /*Variables for the control of the orthographic projection*/

object3d * _first_object= 0;                /*List of objects*/
object3d * _selected_object = 0;            /*Object currently selected*/

GLdouble * matrizea;

GLdouble fovy;
GLdouble aspect;
GLdouble znear;
GLdouble zfar;

PerspCam *objKam;
PerspCam *ibilKam;
GLdouble *matrix;

light* bonbila;
light* eguzkia;
light* fokua;

int argia = 0;


char kam_mota;

    /*
        'o' = ortografikoa
        'b' = objektu-mota
        'i' = ibiltaria
    */

char argi_mota;
    /*
        'e' = eguzkia;
        'f' = fokua;
        'b' = bonbila;
    */

char aldaketak;

    /* Aldaketak ze erreferentzi sistema erabilita egingo diren.
        'g' = Globala
        'l' = Lokala
    */

char egoera;

    /*  Momentuan ze transformazio dagoen aukeratuta.
        'm' = traslazioa
        'b' = biraketa
        't' = eskalaketa
    */


char aldaketakObjKam;

    /*
        'o' = Aldaketak objetuari
        'k' = Aldaketak kamarari
        'a' = Aldaketak argiari
        
    */


void gureHasieraketak(){
    /* Hasierako transformazio aldagaiak */
    kam_mota = 'o';
    argi_mota = 'e';
    egoera = 'm';
    aldaketak = 'g';
    aldaketakObjKam = 'o';



    /* Kamara bektoreen hasieraketak */

    GLdouble * eye = malloc(sizeof(GLdouble)*4);
        eye[0]=0;   eye[1]=0;   eye[2]=-30; eye[3]=1;
    GLdouble * center = malloc(sizeof(GLdouble)*4);
        center[0]=0;    center[1]=0;    center[2]=-30; center[3]=1;
    GLdouble * up = malloc(sizeof(GLdouble)*4);
        up[0]=0;    up[1]=1;    up[2]=0; up[3]=0;
    /* Obj. kamararen balioak */

    objKam = malloc(sizeof(PerspCam));
    Stack *p = malloc(sizeof(Stack));
    Stack_Init(p);
    Stack *d = malloc(sizeof(Stack));
    Stack_Init(d);

    objKam->stack = p;
    objKam->desegin = d;
    objKam->posizioa = malloc(sizeof(GLdouble)*4);
    objKam->begira = malloc(sizeof(GLdouble)*4);
    objKam->gora = malloc(sizeof(GLdouble)*4);

    objKam->posizioa = eye;
    objKam->begira = center;
    objKam->gora = up;
    
    GLdouble *matrix = malloc(sizeof(GLdouble)*16);

    //Kamararen hasierako matrizea
    matrix[0] = 1; matrix[4] = 0;  matrix[8] = 0; matrix[12] = 0; 
    matrix[1] = 0; matrix[5] = 1;  matrix[9] = 0; matrix[13] = 0; 
    matrix[2] = 0; matrix[6] = 0;  matrix[10] = 1; matrix[14] = 40; 
    matrix[3] = 0; matrix[7] = 0;  matrix[11] = 0; matrix[15] = 1; 
    Stack_Push(objKam->stack, matrix);

    /* Kamara ibiltariaren hasieraketa */

    ibilKam = malloc(sizeof(PerspCam));
    Stack *p2 = malloc(sizeof(Stack));
    Stack_Init(p2);
    Stack *d2 = malloc(sizeof(Stack));
    Stack_Init(d2);

    ibilKam->stack = p2;
    ibilKam->desegin = d2;
    ibilKam->posizioa = malloc(sizeof(GLdouble)*4);
    ibilKam->begira = malloc(sizeof(GLdouble)*4);
    ibilKam->gora = malloc(sizeof(GLdouble)*4);

    ibilKam->posizioa = eye;
    ibilKam->begira = center;
    ibilKam->gora = up;

    Stack_Push(ibilKam->stack, matrix);

    /* extra */

    fovy = 20;
    aspect = _window_ratio;
    znear = 10;
    zfar = 100;
    
    eguneratu_objKam();
    eguneratu_ibilKam();

}

void argiakHasieratu(){
    bonbila = malloc(sizeof(light));
    eguzkia = malloc(sizeof(light));
    fokua = malloc(sizeof(light));

    Stack *p = malloc(sizeof(Stack));
    Stack_Init(p);

    bonbila->stack = p;
    eguzkia->stack = p;
    fokua->stack = p;

    bonbila->egoera = 0;
    eguzkia->egoera = 0;
    fokua->egoera = 0;

    GLdouble *argimatrix = malloc(sizeof(GLdouble)*16);
    argimatrix[0] = 1; argimatrix[4] = 0;  argimatrix[8] = 0; argimatrix[12] = 0; 
    argimatrix[1] = 0; argimatrix[5] = 1;  argimatrix[9] = 0; argimatrix[13] = 0; 
    argimatrix[2] = 0; argimatrix[6] = 0;  argimatrix[10] = 1; argimatrix[14] = 0; 
    argimatrix[3] = 0; argimatrix[7] = 0;  argimatrix[11] = 0; argimatrix[15] = 1; 

    Stack_Push(bonbila->stack, argimatrix);
    Stack_Push(eguzkia->stack, argimatrix);
    Stack_Push(fokua->stack, argimatrix);

}

/** GENERAL INITIALIZATION **/
void initialization (){

    /*Initialization of all the variables with the default values*/
    _ortho_x_min = KG_ORTHO_X_MIN_INIT;
    _ortho_x_max = KG_ORTHO_X_MAX_INIT;
    _ortho_y_min = KG_ORTHO_Y_MIN_INIT;
    _ortho_y_max = KG_ORTHO_Y_MAX_INIT;
    _ortho_z_min = KG_ORTHO_Z_MIN_INIT;
    _ortho_z_max = KG_ORTHO_Z_MAX_INIT;

    _window_ratio = (GLdouble) KG_WINDOW_WIDTH / (GLdouble) KG_WINDOW_HEIGHT;

    /*Definition of the background color*/
    glClearColor(KG_COL_BACK_R, KG_COL_BACK_G, KG_COL_BACK_B, KG_COL_BACK_A);

    /*Definition of the method to draw the objects*/
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    gureHasieraketak(); //Transformazioak eta kamara hasieratzeko
    argiakHasieratu(); //Argiak hasieratzeko ID matrizearekin eta 0 egoerarekin

}


/** MAIN FUNCTION **/
int main(int argc, char** argv) {

    /*First of all, print the help information*/
    print_help();

    /* glut initializations */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(KG_WINDOW_WIDTH, KG_WINDOW_HEIGHT);
    glutInitWindowPosition(KG_WINDOW_X, KG_WINDOW_Y);
    glutCreateWindow(KG_WINDOW_TITLE);


    /* set the callback functions */
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(handleSpecialKeypress); /*To treat arrow keys*/

    /* this initialization has to be AFTER the creation of the window */
    initialization();
    glEnable(GL_DEPTH_TEST);


    /* start the main loop */
    glutMainLoop();
    return 0;
}
