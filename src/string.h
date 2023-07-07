// Author: Alan Hergenreder
#ifndef __STRING_H__
#define __STRING_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Basic functions to manage string with void*
*/

void* copy(void*);

void destroy(void*);

void visit(void*);

int compare(void*, void*);


#endif