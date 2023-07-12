#include "int.h"

/**
 * Basic functions to manage int with void*
*/

void* copy_int(void *data) {  

  void* copy = malloc(sizeof(int));
  // *(int*)copy = *(int*)data;
  memcpy(copy, data, sizeof(int));

  return copy;
}


void destroy_int(void *data) {

  free(data);
}


void visit_int(void *data) {

  if(data == NULL) {
    printf("NULL ");
    return;
  }
  printf("%i ", *(int*) data);
}


int compare_int(void* a, void* b) {

  if(*((int*)a) > *((int*)b)) return 1;
  if(*((int*)a) < *((int*)b)) return -1;
  return 0;
}