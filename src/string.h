// Author: Alan Hergenreder
#ifndef __STRING_H__
#define __STRING_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Basic functions to manage string with void*
*/

void* copy_string(void*);

void destroy_string(void*);

void visit_string(void*);

int compare_string(void*, void*);


#endif