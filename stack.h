#include "definitions.h"
#include <stdio.h>

struct Stack {
    GLdouble     *data[9999];
    int     size;
};
typedef struct Stack Stack;

void Stack_Init(Stack *S);

GLdouble * Stack_Top(Stack *S);

void Stack_Push(Stack *S, GLdouble *d);

void Stack_Pop(Stack *S);


