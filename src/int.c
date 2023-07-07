// Author: Alan Hergenreder
#include "int.h"

/**
 * Basic functions to manage int with void*
*/

void* copy(void *data) {  

  void* copy = malloc(sizeof(int));
  // *(int*)copy = *(int*)data;
  memcpy(copy, data, sizeof(int));

  return copy;
}


void destroy(void *data) {

  free(data);
}


void visit(void *data) {

  if(data == NULL) {
    printf("NULL ");
    return;
  }
  printf("%i ", *(int*) data);
}


int compare(void* a, void* b) {

  if(*((int*)a) > *((int*)b)) return 1;
  if(*((int*)a) < *((int*)b)) return -1;
  return 0;
}