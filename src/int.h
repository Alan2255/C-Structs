#ifndef __INT_H__
#define __INT_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Basic functions to manage int with void*
*/

void* copy_int(void*);

void destroy_int(void*);

void visit_int(void*);

int compare_int(void*, void*);


#endif