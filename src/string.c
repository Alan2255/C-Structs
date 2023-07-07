// Author: Alan Hergenreder
#include "string.h"

/**
 * Basic functions to manage string with void*
*/

void* copy(void *data) {  

  char* copy = malloc(sizeof(char) * (strlen((char*) data) + 1));
  
  strcpy(copy, data);

  return (void*) copy;
}


void destroy(void *data) {

  free(data);
}


void visit(void *data) {

  if(data == NULL) {
    printf("NULL ");
    return;
  }
  printf("%s ", (char*) data);
}


int compare(void* a, void* b) {

  return strcmp((char*) a, (char*) b);
}