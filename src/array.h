#ifndef __ARRAY_H__
#define __ARRAY_H__


#include <stdlib.h>
#include <stdio.h>
#include "void.h"
#include "sugar.h"


/**
 * Array
*/
typedef struct _Array {

    void* *at;
    int capacity;

    FunctionCopy copy;
    FunctionDestroy destroy;
    FunctionCompare compare;
    FunctionVisit visit;

} *Array;


/**
 * Create an empty array
*/
Array array_create(int, FunctionCopy, FunctionDestroy, FunctionCompare, FunctionVisit);


/**
 * Destroy the array
*/
void array_destroy(Array);


/**
 * Read the given index of the array
*/
void* array_read(Array, int);


/**
 * Write data in given index of the array
*/
void array_write(Array, void*, int);


/**
 * Return the capacity of the array
*/
int array_capacity(Array);


/**
 * Print the array
*/
void array_print(Array);


/**
 * Adjust the size of the array
*/
void array_resize(Array, int);


/**
 * Insert data in the given index, use resize if canot reach the index
*/
void array_insert(Array, void*, int);


/**
 * Delete data from given index if exist
*/
void array_delete(Array, int);


#endif 