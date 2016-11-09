#ifndef IO_H
#define IO_H

extern GLdouble * matrizea;

void keyboard(unsigned char key, int x, int y);
void handleSpecialKeypress(int key, int x, int y); /*To treat arrow keys*/
void print_help();

#endif // IO_H
