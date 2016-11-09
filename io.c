#include "definitions.h"
#include "load_obj.h"
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "display.h"
#include "matrizeak.h"

extern object3d * _first_object;
extern object3d * _selected_object;

extern GLdouble _ortho_x_min,_ortho_x_max;
extern GLdouble _ortho_y_min,_ortho_y_max;
extern GLdouble _ortho_z_min,_ortho_z_max;

extern GLdouble * matrizea;

int egoera = 0;
    /*
        0 = traslazioa
        1 = biraketa
        2 = eskalaketa
    */


/**
 * @brief This function just prints information about the use
 * of the keys
 */
 void print_help(){
    printf("KbG Irakasgaiaren Praktika. Programa honek 3D objektuak \n");
    printf("aldatzen eta bistaratzen ditu.  \n\n");

    printf("Egilea: Borja Calvo (borja.calvo@ehu.es) \n");
    printf("Salseatzen ibili diren ikasleak: Xabin Rodriguez eta B Zaldua \n");
    printf("Data: Urria, 2016 \n");
    printf("\n\n");
    printf("FUNTZIO NAGUSIAK \n");
    printf("<?>\t\t Laguntza hau bistaratu \n");
    printf("<ESC>\t\t Programatik irten \n");
    printf("<F>\t\t Objektua bat kargatu\n");
    printf("<TAB>\t\t Kargaturiko objektuen artean bat hautatu\n");
    printf("<DEL>\t\t Hautatutako objektua ezabatu\n");
    printf("<CTRL + ->\t Bistaratze-eremua handitu\n");
    printf("<CTRL + +>\t Bistaratze-eremua txikitu\n");
    printf("<I> edo <i>\t Objektuaren informazioa ikusteko \n");
    printf("<M> edo <m>\t Mugitzeko\n");
    printf("<B> edo <b>\t Biratzeko\n");
    printf("<T> edo <t>\t Eskalatzeko\n");
    printf("\n\n");
}

/**
 * @brief Callback function to control the basic keys
 * @param key Key that has been pressed
 * @param x X coordinate of the mouse pointer when the key was pressed
 * @param y Y coordinate of the mouse pointer when the key was pressed
 */
 void keyboard(unsigned char key, int x, int y) {

    char* fname = malloc(sizeof (char)*128); /* Note that scanf adds a null character at the end of the vector*/
    int read = 0;
    object3d *auxiliar_object = 0;
    GLdouble wd,he,midx,midy;
    matrizea = malloc(sizeof(GLdouble)*16);

    switch (key) {
        case 'f':
        case 'F':
        /*Ask for file*/
        printf("%s", KG_MSSG_SELECT_FILE);
        scanf("%s", fname);
        /*Allocate memory for the structure and read the file*/
        auxiliar_object = (object3d *) malloc(sizeof (object3d));
        read = read_wavefront(fname, auxiliar_object);
        switch (read) {
        /*Errors in the reading*/
            case 1:
            printf("%s: %s\n", fname, KG_MSSG_FILENOTFOUND);
            break;
            case 2:
            printf("%s: %s\n", fname, KG_MSSG_INVALIDFILE);
            break;
            case 3:
            printf("%s: %s\n", fname, KG_MSSG_EMPTYFILE);
            break;
        /*Read OK*/
            case 0:
            /*Insert the new object in the list*/
            auxiliar_object->next = _first_object;
            _first_object = auxiliar_object;
            _selected_object = _first_object;
            _selected_object->name = fname;
            /*Hasierako puntuko matrizea sortu*/
            matrizea[0]=1;
            matrizea[1]=0;
            matrizea[2]=0;
            matrizea[3]=0;
            matrizea[4]=0;
            matrizea[5]=1;
            matrizea[6]=0;
            matrizea[7]=0;
            matrizea[8]=0;
            matrizea[9]=0;
            matrizea[10]=1; 
            matrizea[11]=0;
            matrizea[12]=0;
            matrizea[13]=0; 
            matrizea[14]=0; 
            matrizea[15]=1;
            /*Hasierako puntua gorde pilan*/
            struct Stack *pila = malloc(sizeof(Stack));
            Stack_Init(pila);
            Stack_Push(pila, matrizea);
            _selected_object->stack = pila;
            printf("%s\n",KG_MSSG_FILEREAD);
            break;
        }
        break;

    case 9: /* <TAB> */
        if(_selected_object == NULL){
            printf("Ez dago objekturik kargatuta. \n");
        }else{
            _selected_object = _selected_object->next;
            /*The selection is circular, thus if we move out of the list we go back to the first element*/
            if (_selected_object == 0) _selected_object = _first_object;
        }
        break;

    case 127: /* <SUPR> */
        /*Erasing an object depends on whether it is the first one or not*/
        if (_selected_object == _first_object)
        {
	    //Ezabaketa _first_object nulua ez bada bakarrik egingo da, nulua den kasuan objeturik ez dela kargatu esan nahi du eta.
           if (_first_object != NULL) {
	    	/*To remove the first object we just set the first as the current's next*/
               _first_object = _first_object->next;
            	/*Once updated the pointer to the first object it is save to free the memory*/
               free(_selected_object);
            	/*Finally, set the selected to the new first one*/
               _selected_object = _first_object;
           }
           else {printf("Ez dago ezabatzeko ezer! \n");}
       } else {
            /*In this case we need to get the previous element to the one we want to erase*/
        auxiliar_object = _first_object;
        while (auxiliar_object->next != _selected_object)
            auxiliar_object = auxiliar_object->next;
            /*Now we bypass the element to erase*/
        auxiliar_object->next = _selected_object->next;
            /*free the memory*/
        free(_selected_object);
            /*and update the selection*/
        _selected_object = auxiliar_object;
    }
    break;

    case '-':
        if (glutGetModifiers() == GLUT_ACTIVE_CTRL){ //Ctrl tekla ea zapalduta dagoen kontrolatzen du
            /*Increase the projection plane; compute the new dimensions*/
            wd=(_ortho_x_max-_ortho_x_min)/KG_STEP_ZOOM; //Planoaren zabalera berria hartu
            he=(_ortho_y_max-_ortho_y_min)/KG_STEP_ZOOM; // = Altuera berria
            /*In order to avoid moving the center of the plane, we get its coordinates*/
	    //Planoaren erdiko puntuak lortzeko
            midx = (_ortho_x_max+_ortho_x_min)/2;
            midy = (_ortho_y_max+_ortho_y_min)/2;
            /*The the new limits are set, keeping the center of the plane*/

            _ortho_x_max = midx + wd/2;
            _ortho_x_min = midx - wd/2;
            _ortho_y_max = midy + he/2;
            _ortho_y_min = midy - he/2;
        }
        break;

        case '+':
        //INPLEMENTA EZAZU CTRL + + KONBINAZIOAREN FUNTZIOANLITATEA
        if (glutGetModifiers() == GLUT_ACTIVE_CTRL){
            /*Increase the projection plane; compute the new dimensions*/
	    //Aurreko eragiketan kamara urruntzeko zatiketa erabiltzen da, hemen biderketa nahi dugu.
            wd=(_ortho_x_max-_ortho_x_min)*KG_STEP_ZOOM;
            he=(_ortho_y_max-_ortho_y_min)*KG_STEP_ZOOM;
            /*In order to avoid moving the center of the plane, we get its coordinates*/
            midx = (_ortho_x_max+_ortho_x_min)/2;
            midy = (_ortho_y_max+_ortho_y_min)/2;
            /*The the new limits are set, keeping the center of the plane*/
            _ortho_x_max = midx + wd/2;
            _ortho_x_min = midx - wd/2;
            _ortho_y_max = midy + he/2;
            _ortho_y_min = midy - he/2;
        }
        break;

        case '?':
        print_help();
        break;

    case 27: /* <ESC> */
        exit(0);
        break;

	case 73:   /*<I>*/
	case 105: /*<i>*/
		if(_selected_object == NULL) //Ikusteko ea objeturik hautatuta dagoen
		{
			printf("Ez dago objekturik hautatuta \n");
		}else{
			GLint erpinak = _selected_object -> num_vertices; //objectuaren erpin kopurua lortzen du
			GLint aurpegiak = _selected_object -> num_faces; //Objektuaren aurpegi kopurua
			char *iz = _selected_object->name; //Struct-ari guk gehitu diogu izena (path-a) lortzen du
			printf("Erpinak: %d, Aurpegiak: %d, Izena: %s \n", erpinak, aurpegiak, iz);
		}
		break;

        case 77: /*<M>*/
        case 109: /*<m>*/
        egoera = 0;
        printf("Orain objektua mugitu daiteke\n");
        break;
        
        case 98: /*<b>*/
        case 66: /*<B>*/
        egoera = 1;
        printf("Orain objektua biratu daiteke\n");
        break;

        case 116: /*<t>*/
        case 84:  /*<T>*/
        egoera = 2;
        printf("Orain objektua eskalatu daiteke\n");
        break;

        default:
        /*In the default case we just print the code of the key. This is usefull to define new cases*/
        printf("%d %c\n", key, key);
    }
    /*In case we have do any modification affecting the displaying of the object, we redraw them*/
    glutPostRedisplay();
}

/*To treat arrow keys*/
void handleSpecialKeypress(int key, int x, int y) {
    GLdouble * lagMatrizea = malloc(sizeof(GLdouble)*16);
    GLint f, v, v_index;


    if (egoera == 0){


       switch (key) {
           case GLUT_KEY_LEFT:

           if(_selected_object == NULL){
            printf("Ez dago objekturik.\n");
        }else{
            matrizea[0]=1;
            matrizea[1]=0;
            matrizea[2]=0;
            matrizea[3]=0;
            matrizea[4]=0;
            matrizea[5]=1;
            matrizea[6]=0;
            matrizea[7]=0;
            matrizea[8]=0;
            matrizea[9]=0;
            matrizea[10]=1; 
            matrizea[11]=0;
            matrizea[12]=-5;
            matrizea[13]=0; 
            matrizea[14]=0; 
            matrizea[15]=1;
            lagMatrizea = Stack_Top(_selected_object -> stack);
                    /*lagMatrizea bider matrizea -> gorde pilan*/
                    /*TODO: biderketa funtzioa*/
                    /**/
            Stack_Push(_selected_object->stack, matrizea);
            printf("Elementu kopurua pilan: %d \n", _selected_object->stack->size);


        }
        break;

        case GLUT_KEY_UP:
        if(_selected_object == NULL){
            printf("Ez dago objekturik.\n");
        }else{
            matrizea[0]=1;
            matrizea[1]=0;
            matrizea[2]=0;
            matrizea[3]=0;
            matrizea[4]=0;
            matrizea[5]=1;
            matrizea[6]=0;
            matrizea[7]=0;
            matrizea[8]=0;
            matrizea[9]=0;
            matrizea[10]=1; 
            matrizea[11]=0;
            matrizea[12]=0;
            matrizea[13]=5; 
            matrizea[14]=0; 
            matrizea[15]=1;

        }
        break;


        case GLUT_KEY_DOWN:
        if(_selected_object == NULL){
            printf("Ez dago objekturik.\n");
        }else{

            matrizea[0]=1;
            matrizea[1]=0;
            matrizea[2]=0;
            matrizea[3]=0;
            matrizea[4]=0;
            matrizea[5]=1;
            matrizea[6]=0;
            matrizea[7]=0;
            matrizea[8]=0;
            matrizea[9]=0;
            matrizea[10]=1; 
            matrizea[11]=0;
            matrizea[12]=0;
            matrizea[13]=-5; 
            matrizea[14]=0; 
            matrizea[15]=1;

        }
        break;

        case GLUT_KEY_RIGHT:
        if(_selected_object == NULL){
            printf("Ez dago objekturik.\n");
        }else{
            matrizea[0]=1;
            matrizea[1]=0;
            matrizea[2]=0;
            matrizea[3]=0;
            matrizea[4]=0;
            matrizea[5]=1;
            matrizea[6]=0;
            matrizea[7]=0;
            matrizea[8]=0;
            matrizea[9]=0;
            matrizea[10]=1; 
            matrizea[11]=0;
            matrizea[12]=5;
            matrizea[13]=0; 
            matrizea[14]=0; 
            matrizea[15]=1;
            
        }

        break;

    }

}



else if (egoera == 1){
// ============== BIRAKETA ============//

   switch (key) {
       case GLUT_KEY_LEFT:

       if(_selected_object == NULL){
        printf("Ez dago objekturik.\n");
    }else{

        matrizea[0]=1;
        matrizea[1]=0;
        matrizea[2]=0;
        matrizea[3]=0;
        matrizea[4]=0;
        matrizea[5]=cos(90);
        matrizea[6]=sin(90);
        matrizea[7]=0;
        matrizea[8]=0;
        matrizea[9]=-sin(90);
        matrizea[10]=cos(90); 
        matrizea[11]=0;
        matrizea[12]=0;
        matrizea[13]=0; 
        matrizea[14]=0; 
        matrizea[15]=1;
        Stack_Push(_selected_object->stack, matrizea);
        printf("Elementu kopurua pilan: %d \n", _selected_object->stack->size);
    }
    break;

    case GLUT_KEY_UP:
    if(_selected_object == NULL){
        printf("Ez dago objekturik.\n");
    }else{
            /*/*/
    }
    break;

    case GLUT_KEY_DOWN:
    if(_selected_object == NULL){
        printf("Ez dago objekturik.\n");
    }else{
            /* */
    }
    break;

    case GLUT_KEY_RIGHT:
    if(_selected_object == NULL){
        printf("Ez dago objekturik.\n");
    }else{
           /* */
    }
    break;
}



}

    /*Zerbait berria marraztu behar badu marrazteko*/
glutPostRedisplay();
}