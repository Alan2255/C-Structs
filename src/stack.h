#ifndef __STACK_H__
#define __STACK_H__

#include "array.h"


/**
 * Stack
*/
typedef struct _Stack {

    Array array;
    int last;

} *Stack;


/**
 * Create an empty stack
*/
Stack stack_create(int, FunctionCopy, FunctionDestroy, FunctionCompare, FunctionVisit);


/**
 * Destroy the stack
*/
void stack_destroy(Stack);


/**
 * Check if stack is empty, return true if it is, false otherwise
*/
int stack_is_empty(Stack);


/**
 * Return the top of the stack
*/
void* stack_top(Stack);


/**
 * Push data in the stack
*/
void stack_push(Stack, void*);


/**
 * Pop data from the stack
*/
void stack_pop(Stack);


/**
 * Print the stack
*/
void stack_print(Stack);


#endif