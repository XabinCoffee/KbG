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

extern char kam_mota;
extern char aldaketak;
extern char egoera;
extern char aldaketakObjKam;


int zenbat = 0;
/**
 * @brief This function just prints information about the use
 * of the keys
 */
void print_help(){
	printf("KbG Irakasgaiaren Praktika. Programa honek 3D objektuak \n");
	printf("aldatzen eta bistaratzen ditu.  \n\n");

	printf("Egilea: Borja Calvo (borja.calvo@ehu.es) \n");
	printf("Salseatzen ibili diren ikasleak: Xabin Rodriguez eta Benat Zaldua \n");
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
	printf("<G> edo <g>\t Aldaketak modu globalean egiteko\n");
	printf("<L> edo <l>\t Aldaketak objetuaren erreferentzi sistema beran egiteko\n");
	printf("<CTRL + Z>\t Egindako aldaketak desegiteko\n");
	printf("<CTRL + Y>\t Desegindako aldaketak berriz aplikatzeko\n");
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
			struct Stack *desegin = malloc(sizeof(Stack));
			Stack_Init(pila);
			Stack_Init(desegin);
			Stack_Push(pila, matrizea);
			_selected_object->stack = pila;
			_selected_object->desegin = desegin;
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

		case 26: /*CTRL + z*/
			//Aldaketak desegiteko CTRL + Z erabiliz
			if (glutGetModifiers() == GLUT_ACTIVE_CTRL){
				if(_selected_object != NULL){
					if (_selected_object->stack->size > 1){
						Stack_Push(_selected_object->desegin,Stack_Top(_selected_object->stack));
						Stack_Pop(_selected_object->stack);
					}
				}
			}
			break;


		case 25: /*CTRL + y*/
			//Desegindako aldaketak berreskuratzeko
			if (glutGetModifiers() == GLUT_ACTIVE_CTRL){
				if(_selected_object != NULL){
					if (_selected_object->desegin->size > 1){
						GLdouble *m = malloc(sizeof(GLdouble));
						m = Stack_Top(_selected_object->desegin);
						Stack_Pop(_selected_object->desegin);
						Stack_Push(_selected_object->stack,m);
					}
				}
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
			if (egoera == 'm') break;
			egoera = 'm';
			printf("Orain objektua mugitu daiteke\n");
			break;

		case 98: /*<b>*/
		case 66: /*<B>*/
			if (egoera == 'b') break;
			egoera = 'b';
			printf("Orain objektua biratu daiteke\n");
			break;

		case 116: /*<t>*/
		case 84:  /*<T>*/
			if (egoera == 't') break;
			egoera = 't';
			printf("Orain objektua eskalatu daiteke\n");
			break;


		case 71:   /*<G>*/
		case 103: /*<g>*/
			if (aldaketak == 'g') break;
			aldaketak='g';
			printf("Aldaketak erreferentzi sistema globalarekin egingo dira\n");
			break;

		case 76:   /*<L>*/
		case 108: /*<l>*/
			if (aldaketak == 'l') break;
			aldaketak='l';
			printf("Aldaketak erreferentzi sistema lokalean egingo dira\n");
			break;

		case 99:   /*<c>*/
		case 67: /*<C>*/
			if (kam_mota == 'o') {
				kam_mota = 'b';
				printf("Objektu-kamara \n");

			}
			else if(kam_mota=='b'){
				kam_mota = 'i';
				printf("Kamara ibiltaria \n");
			}
			else {
				kam_mota = 'o';
				printf("Kamara ortogonala \n");
			}

			break;

		case 111:
		case 79:

			aldaketakObjKam = 'o';
			printf("Aldaketak objetuari aplikatuko zaizkio \n");
			break;

		case 107:
		case 75:

			aldaketakObjKam = 'k';
			printf("Aldaketak kamarari aplikatuko zaizkio \n");
			break;

		default:
			/*In the default case we just print the code of the key. This is usefull to define new cases*/
			printf("%d %c\n", key, key);
	}
	/*In case we have do any modification affecting the displaying of the object, we redraw them*/
	glutPostRedisplay();
}

/**
 * @brief Treating arrow keys/PageUp, PageDown
 * @param key Special Key that has been pressed
 */

void handleSpecialKeypress(int key, int x, int y) {

	if (aldaketakObjKam == 'o'){

		if (egoera == 'm'){
			// ============= TRANSLAZIOA =============//

			switch (key) {
			case GLUT_KEY_LEFT:
				if(_selected_object == NULL){
					printf("Ez dago objekturik.\n");
				}else{
					translazioa(key, aldaketak); //Traslazioa egin
					Stack_Init(_selected_object->desegin);
				}
				break;

			case GLUT_KEY_UP:
				if(_selected_object == NULL){
					printf("Ez dago objekturik.\n");
				}else{
					translazioa(key, aldaketak);
					Stack_Init(_selected_object->desegin);
				}
				break;

			case GLUT_KEY_DOWN:
				if(_selected_object == NULL){
					printf("Ez dago objekturik.\n");
				}else{
					translazioa(key, aldaketak);
					Stack_Init(_selected_object->desegin);
				}
				break;

			case GLUT_KEY_RIGHT:
				if(_selected_object == NULL){
					printf("Ez dago objekturik.\n");
				}else{
					translazioa(key, aldaketak);
					Stack_Init(_selected_object->desegin);
				}
				break;

			case GLUT_KEY_PAGE_UP:
				if(_selected_object == NULL){
					printf("Ez dago objekturik.\n");
				}else{
					translazioa(key, aldaketak);
					Stack_Init(_selected_object->desegin);
				}
				break;

			case GLUT_KEY_PAGE_DOWN:
				if(_selected_object == NULL){
					printf("Ez dago objekturik.\n");
				}else{
					translazioa(key, aldaketak);
					Stack_Init(_selected_object->desegin);
				}
				break;

			}

		}
		else if (egoera == 'b'){
			// ============== BIRAKETA ============//
			switch (key) {
			case GLUT_KEY_LEFT:
				if(_selected_object == NULL){
					printf("Ez dago objekturik.\n");
				}else{
					biraketa(key, aldaketak);
					Stack_Init(_selected_object->desegin);
				}
				break;
			case GLUT_KEY_UP:
				if(_selected_object == NULL){
					printf("Ez dago objekturik.\n");
				}else{
					biraketa(key, aldaketak);
					Stack_Init(_selected_object->desegin);
				}
				break;
			case GLUT_KEY_DOWN:
				if(_selected_object == NULL){
					printf("Ez dago objekturik.\n");
				}else{
					biraketa(key, aldaketak);
					Stack_Init(_selected_object->desegin);
				}
				break;
			case GLUT_KEY_RIGHT:
				if(_selected_object == NULL){
					printf("Ez dago objekturik.\n");
				}else{
					biraketa(key, aldaketak);
					Stack_Init(_selected_object->desegin);
				}
				break;

			case GLUT_KEY_PAGE_UP:
				if(_selected_object == NULL){
					printf("Ez dago objekturik.\n");
				}else{
					biraketa(key, aldaketak);
					Stack_Init(_selected_object->desegin);
				}
				break;

			case GLUT_KEY_PAGE_DOWN:
				if(_selected_object == NULL){
					printf("Ez dago objekturik.\n");
				}else{
					biraketa(key, aldaketak);
					Stack_Init(_selected_object->desegin);
				}
				break;
			}
		}

		else if(egoera == 't'){
			// ============== ESKALAKETA ============//
			switch(key){
			case GLUT_KEY_LEFT:
				if(_selected_object == NULL){
					printf("Ez dago objekturik.\n");
				}else{
					eskalatu(key, aldaketak);
					Stack_Init(_selected_object->desegin);
				}
				break;
			case GLUT_KEY_UP:
				if(_selected_object == NULL){
					printf("Ez dago objekturik.\n");
				}else{
					eskalatu(key, aldaketak);
					Stack_Init(_selected_object->desegin);
				}
				break;
			case GLUT_KEY_DOWN:
				if(_selected_object == NULL){
					printf("Ez dago objekturik.\n");
				}else{
					eskalatu(key, aldaketak);
					Stack_Init(_selected_object->desegin);
				}
				break;
			case GLUT_KEY_RIGHT:
				if(_selected_object == NULL){
					printf("Ez dago objekturik.\n");
				}else{
					eskalatu(key, aldaketak);
					Stack_Init(_selected_object->desegin);
				}
				break;

			case GLUT_KEY_PAGE_UP:
				if(_selected_object == NULL){
					printf("Ez dago objekturik.\n");
				}else{
					eskalatu(key, aldaketak);
					Stack_Init(_selected_object->desegin);
				}
				break;

			case GLUT_KEY_PAGE_DOWN:
				if(_selected_object == NULL){
					printf("Ez dago objekturik.\n");
				}else{
					eskalatu(key, aldaketak);
					Stack_Init(_selected_object->desegin);
				}
				break;
			}
		}
	}


	if (aldaketakObjKam == 'k'){

		if (kam_mota == 'b'){


			if (egoera == 'm'){

				switch(key){

					case GLUT_KEY_LEFT:
						translazioaKam(GLUT_KEY_RIGHT, aldaketak);
						break;

					case GLUT_KEY_RIGHT:
						translazioaKam(GLUT_KEY_LEFT, aldaketak);
						break;

					case GLUT_KEY_UP:
						translazioaKam(key, aldaketak);
						break;

					case GLUT_KEY_DOWN:
						translazioaKam(key, aldaketak);
						break;

					case GLUT_KEY_PAGE_UP:
						translazioaKam(key, aldaketak);
						break;

					case GLUT_KEY_PAGE_DOWN:
						translazioaKam(key, aldaketak);
						break;

					default: 
						break;

				}

			}

			if (egoera == 'b'){

				switch(key){

					case GLUT_KEY_LEFT:
						biraketaKam(key, aldaketak);
						break;

					case GLUT_KEY_RIGHT:
						biraketaKam(key, aldaketak);
						break;

					case GLUT_KEY_UP:
						biraketaKam(key, aldaketak);
						break;

					case GLUT_KEY_DOWN:
						biraketaKam(key, aldaketak);
						break;

					case GLUT_KEY_PAGE_UP:
						biraketaKam(key, aldaketak);
						break;

					case GLUT_KEY_PAGE_DOWN:
						biraketaKam(key, aldaketak);
						break;

					default: 
						break;

				}

			}

		}

		eguneratu_objKam();

	}

		if (kam_mota == 'i'){


			if (egoera == 'm'){

				switch(key){

					case GLUT_KEY_LEFT:
						translazioaKam(GLUT_KEY_RIGHT, aldaketak);
						break;

					case GLUT_KEY_RIGHT:
						translazioaKam(GLUT_KEY_LEFT, aldaketak);
						break;

					case GLUT_KEY_UP:
						translazioaKam(GLUT_KEY_PAGE_UP, aldaketak);
						break;

					case GLUT_KEY_DOWN:
						translazioaKam(GLUT_KEY_PAGE_DOWN, aldaketak);
						break;

					case GLUT_KEY_PAGE_UP:

						if (zenbat <= 2){
							biraketaKam(GLUT_KEY_UP, 'l');
							zenbat++;
						}
						break;

					case GLUT_KEY_PAGE_DOWN:
						if (zenbat >= -2){
							biraketaKam(GLUT_KEY_DOWN, 'l');
							zenbat--;
						}
						break;

					default: 
						break;

				}

			}

		eguneratu_ibilKam();

	}


	/*Zerbait berria marraztu behar badu marrazteko*/
	glutPostRedisplay();
}


