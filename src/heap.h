#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdlib.h>
#include "void.h"
#include "sugar.h"


/**
 * Heap type
*/
typedef enum {

  MAX,  /* Maximun at the top */
  MIN, /* Minimun at the top */

} PriorityType;


/**
 * Binary heap
*/
typedef struct _BHeap {

  void* *array;
  int capacity;
  int last;

  PriorityType type;

  FunctionCopy copy;
  FunctionDestroy destroy;
  FunctionCompare compare;
  FunctionVisit visit;

} *BHeap;


/**
 * Create an empty binary heap
*/
BHeap bheap_create(int, PriorityType, FunctionCopy, FunctionDestroy, FunctionCompare, FunctionVisit);


/**
 * Destroy the binary heap
*/
void bheap_destroy(BHeap);


/**
 * Check if the binary heap is empty, return true if its, false otherwise
*/
int bheap_is_empty(BHeap);


/**
 * Print the binary heap
*/
void bheap_print(BHeap);


/**
 * Let climb some element at the given index of the binary heap
 * Return true if the element climb at least one time, false otherwise
*/
int bheap_climb(BHeap, int);


/**
 * Insert the given data in the binary heap
*/
void bheap_insert(BHeap, void*);


/**
 * Let fall some element at the given index of the binary heap
 * Return true if the element fall at least one time, false otherwise
*/
int bheap_fall(BHeap, int);


/**
 * Delete the top of the binary heap
*/
void bheap_pop(BHeap);


/**
 * Delete data from the binary heap
*/
void bheap_delete(BHeap, void*);


/**
 * Return a binary heap created from the given array
*/
BHeap bheap_create_from_array(void**, int, PriorityType, FunctionCopy, FunctionDestroy, FunctionCompare, FunctionVisit);


#endif