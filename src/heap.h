#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "void.h"
#include "sugar.h"

/*
Binary heap
*/

typedef enum {

  MAX,  /* Maximun at the top */
  MIN, /* Minimun at the top */

} PriorityType;


typedef struct _BHeap {

    void **array;
    int capacity;
    int last;
    PriorityType type;
    FunctionCompare compare;

} *BHeap;


/**
 * Create a heap with the given capacity and 
*/
BHeap bheap_create(int, FunctionCompare, PriorityType);


/**
 * Destroy the heap
*/
void bheap_destroy(BHeap, FunctionDestroy);


/**
 * Check if the heap is empty, return 1 if its and 0 otherwise
*/
int bheap_is_empty(BHeap);


/**
 * Travel through the heap apllying the given function to every element 
*/
void bheap_travel(BHeap, FunctionVisit);


/**
 * Let climb some element at the given index of the heap
 * Return true if the element climb at least one time, 
 * false otherwise
*/
int bheap_climb(BHeap, int);


/**
 * Insert the given data in the heap
*/
void bheap_insert(BHeap, void*, FunctionCopy);


/**
 * Let fall some element at the given index of the heap
 * Return true if the element fall at least one time,
 * false otherwise
*/
int bheap_fall(BHeap, int);


/**
 * Delete the top of the heap
*/
void bheap_pop(BHeap, FunctionDestroy);


/**
 * Delete some given data from the heap
*/
void bheap_delete(BHeap, void*, FunctionDestroy);


/**
 * Return a new heap created from the given array
*/
BHeap bheap_create_from_array(void**, int, FunctionCopy, FunctionCompare, PriorityType);


#endif