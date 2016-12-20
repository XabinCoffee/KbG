#include "definitions.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "io.h"

/** EXTERNAL VARIABLES **/

extern GLdouble _window_ratio;
extern GLdouble _ortho_x_min,_ortho_x_max;
extern GLdouble _ortho_y_min,_ortho_y_max;
extern GLdouble _ortho_z_min,_ortho_z_max;

extern object3d *_first_object;
extern object3d *_selected_object;

extern GLdouble * matrizea;

extern GLdouble fovy;
extern GLdouble zfar;
extern GLdouble znear;
extern GLdouble aspect;

extern kam_mota;

extern PerspCam *objKam;
extern PerspCam *ibilKam;

extern int argia;
extern char argi_mota;

extern light* bonbila;
extern light* eguzkia;
extern light* fokua;

/**
 * @brief Function to draw the axes
 */
void draw_axes()
{
    /*Draw X axis*/
    glColor3f(KG_COL_X_AXIS_R,KG_COL_X_AXIS_G,KG_COL_X_AXIS_B);
    glBegin(GL_LINES);
    glVertex3d(KG_MAX_DOUBLE,0,0);
    glVertex3d(-1*KG_MAX_DOUBLE,0,0);
    glEnd();
    /*Draw Y axis*/
    glColor3f(KG_COL_Y_AXIS_R,KG_COL_Y_AXIS_G,KG_COL_Y_AXIS_B);
    glBegin(GL_LINES);
    glVertex3d(0,KG_MAX_DOUBLE,0);
    glVertex3d(0,-1*KG_MAX_DOUBLE,0);
    glEnd();
    /*Draw Z axis*/
    glColor3f(KG_COL_Z_AXIS_R,KG_COL_Z_AXIS_G,KG_COL_Z_AXIS_B);
    glBegin(GL_LINES);
    glVertex3d(0,0,KG_MAX_DOUBLE);
    glVertex3d(0,0,-1*KG_MAX_DOUBLE);
    glEnd();
}


/**
 * @brief Callback reshape function. We just store the new proportions of the window
 * @param width New width of the window
 * @param height New height of the window
 */
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    /*  Take care, the width and height are integer numbers, but the ratio is a GLdouble so, in order to avoid
     *  rounding the ratio to integer values we need to cast width and height before computing the ratio */
    _window_ratio = (GLdouble) width / (GLdouble) height;
}

void kamaraKokatu(){
     switch(kam_mota){
        case 'o':
            /*When the window is wider than our original projection plane we extend the plane in the X axis*/
            if ((_ortho_x_max - _ortho_x_min) / (_ortho_y_max - _ortho_y_min) < _window_ratio) {
                /* New width */
                GLdouble wd = (_ortho_y_max - _ortho_y_min) * _window_ratio;
                /* Midpoint in the X axis */
                GLdouble midpt = (_ortho_x_min + _ortho_x_max) / 2;
                /*Definition of the projection*/
                glOrtho(midpt - (wd / 2), midpt + (wd / 2), _ortho_y_min, _ortho_y_max, _ortho_z_min, _ortho_z_max);
            } else {/* In the opposite situation we extend the Y axis */
                /* New height */
                GLdouble he = (_ortho_x_max - _ortho_x_min) / _window_ratio;
                /* Midpoint in the Y axis */
                GLdouble midpt = (_ortho_y_min + _ortho_y_max) / 2;
                /*Definition of the projection*/
                glOrtho(_ortho_x_min, _ortho_x_max, midpt - (he / 2), midpt + (he / 2), _ortho_z_min, _ortho_z_max);
            }
            break;

        case 'b': ;
            gluPerspective(fovy,aspect,znear,zfar);
            gluLookAt(objKam->posizioa[0],objKam->posizioa[1],objKam->posizioa[2],
                objKam->begira[0],objKam->begira[1],objKam->begira[2],
                objKam->gora[0],objKam->gora[1],objKam->gora[2]);
            break;

        case 'i':

            gluPerspective(fovy,aspect,znear,zfar);
            gluLookAt(ibilKam->posizioa[0],ibilKam->posizioa[1],ibilKam->posizioa[2],
                ibilKam->begira[0],ibilKam->begira[1],ibilKam->begira[2],
                ibilKam->gora[0],ibilKam->gora[1],ibilKam->gora[2]);

            break;

        }
}

/**
 * @brief Callback display function
 */
void display(void) {
    GLint v_index, v, f;
    object3d *aux_obj = _first_object;

    /* Clear the screen */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    /* Define the projection */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(argia) glDisable(GL_LIGHTING);
    kamaraKokatu();

    /* Now we start drawing the object */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /*Koloreak*/

    GLfloat horia [4] = {0.0, 1.0, 1.0, 1.0};
    GLfloat grisa [4] = {0.2, 0.2, 0.2, 1.0};
    GLfloat txuria [4] = {1.0, 1.0, 1.0, 1.0};

    GLfloat ambiente[4] = {0.19125, 0.0735, 0.0225, 1.0};
    GLfloat difuso[4] = {0.7038, 0.27048, 0.0828, 1.0};
    GLfloat especular[4] = {0.256777, 0.137622, 0.0806014, 1.0};

    /*First, we draw the axes*/
    if(argia){
        draw_axes();
        glEnable(GL_LIGHTING);
    }else{
        draw_axes();
    }

    GLfloat *position = malloc(sizeof(GLfloat)*4);
    GLfloat *nora = malloc(sizeof(GLfloat)*4);
    glLoadIdentity();

    if(argia){
        if (argi_mota == 'b') glMultMatrixd(Stack_Top(bonbila->stack));
        else if (argi_mota == 'e') glMultMatrixd(Stack_Top(eguzkia->stack));
        else if (argi_mota == 'f') glMultMatrixd(Stack_Top(fokua->stack));

        glLightfv(GL_LIGHT0 , GL_AMBIENT , grisa );
        glLightfv(GL_LIGHT0 , GL_DIFFUSE , horia );
        glLightfv(GL_LIGHT0 , GL_SPECULAR , txuria );
        glLightfv(GL_LIGHT1 , GL_AMBIENT , grisa );
        glLightfv(GL_LIGHT1 , GL_DIFFUSE , horia );
        glLightfv(GL_LIGHT1 , GL_SPECULAR , txuria );
        glLightfv(GL_LIGHT2 , GL_AMBIENT , grisa );
        glLightfv(GL_LIGHT2 , GL_DIFFUSE , horia );
        glLightfv(GL_LIGHT2 , GL_SPECULAR , txuria );
        glMaterialfv(GL_LIGHT0,GL_AMBIENT,ambiente);
        glMaterialfv(GL_LIGHT0,GL_DIFFUSE,difuso);
        glMaterialfv(GL_LIGHT0,GL_SPECULAR,especular);
        glMaterialf(GL_LIGHT0,GL_SHININESS, 0.6);
        glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION, 1.0);
        glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0);
        glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION, 0.8);
        glMaterialfv(GL_LIGHT1,GL_AMBIENT,ambiente);
        glMaterialfv(GL_LIGHT1,GL_DIFFUSE,difuso);
        glMaterialfv(GL_LIGHT1,GL_SPECULAR,especular);
        glMaterialf(GL_LIGHT1,GL_SHININESS, 0.6);
        glLightf(GL_LIGHT1,GL_CONSTANT_ATTENUATION, 1.0);
        glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.0);
        glLightf(GL_LIGHT1,GL_QUADRATIC_ATTENUATION, 0.0);
        glMaterialfv(GL_LIGHT2,GL_AMBIENT,ambiente);
        glMaterialfv(GL_LIGHT2,GL_DIFFUSE,difuso);
        glMaterialfv(GL_LIGHT2,GL_SPECULAR,especular);
        glMaterialf(GL_LIGHT2,GL_SHININESS, 0.6);
        glLightf(GL_LIGHT2,GL_CONSTANT_ATTENUATION, 1.0);
        glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.0);
        glLightf(GL_LIGHT2,GL_QUADRATIC_ATTENUATION, 0.0);

        if (argi_mota == 'b'){
            position[0] = 0.0; position[1] = 2.0; position[2] = 0.0; position[3] = 1.0;
            glLightfv(GL_LIGHT0, GL_POSITION, position);
            glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0);
        }
        else if (argi_mota == 'e'){
            position[0] = 1.0; position[1] = 0.0; position[2] = 0.0; position[3] = 0.0;
            glLightfv(GL_LIGHT1, GL_POSITION, position);
        }
        else if (argi_mota == 'f'){
            position[0] = 0.0; position[1] = 10.0; position[2] = 0.0; position[3] = 1.0;
            nora[0] = 0.0; nora[1] = -1.0; nora[2] = 0.0;
            glLightfv(GL_LIGHT2, GL_POSITION, position);
            glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, nora);
            glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 10.0);
            glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 1.0);
        }

    }

    /*Now each of the objects in the list*/
    while (aux_obj != 0) {

        /* Select the color, depending on whether the current object is the selected one or not */
        if (aux_obj == _selected_object){
            glColor3f(KG_COL_SELECTED_R,KG_COL_SELECTED_G,KG_COL_SELECTED_B);
        }else{
            glColor3f(KG_COL_NONSELECTED_R,KG_COL_NONSELECTED_G,KG_COL_NONSELECTED_B);
        }

        /* Draw the object; for each face create a new polygon with the corresponding vertices */
        glLoadIdentity();
        glMultMatrixd(Stack_Top(aux_obj->stack));

        for (f = 0; f < aux_obj->num_faces; f++) {
            glBegin(GL_POLYGON);
            for (v = 0; v < aux_obj->face_table[f].num_vertices; v++) {
                v_index = aux_obj->face_table[f].vertex_table[v];
                glNormal3d(aux_obj->normala[f].x,aux_obj->normala[f].y,aux_obj->normala[f].z);

                glVertex3d(aux_obj->vertex_table[v_index].coord.x,
                        aux_obj->vertex_table[v_index].coord.y,
                        aux_obj->vertex_table[v_index].coord.z);

            }
            glEnd();
        }
        aux_obj = aux_obj->next;
    }
    /*Do the actual drawing*/
    glFlush();
}

