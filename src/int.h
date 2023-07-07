// Author: Alan Hergenreder
#ifndef __INT_H__
#define __INT_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Basic functions to manage int with void*
*/

void* copy(void*);

void destroy(void*);

void visit(void*);

int compare(void*, void*);


#endif