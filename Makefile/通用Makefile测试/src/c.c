#include <stdio.h>

#include "c.h"

void func_c()
{
    #ifdef DEBUG
        printf("Debug mode enabled.\n");
    #endif
    printf("Hello from func_c: %d\n",C);
}