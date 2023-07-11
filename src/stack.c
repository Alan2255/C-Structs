#include "stack.h"


/**
 * Stack
*/

/**
 * Create an empty stack
*/
Stack stack_create(int capacity, FunctionCopy copy, FunctionDestroy destroy, FunctionCompare compare, FunctionVisit visit) {

    Stack newStack = malloc(sizeof(struct _Stack));

    newStack->array = array_create(capacity, copy, destroy, compare, visit);
    newStack->last = -1;
    
    return newStack;
}


/**
 * Destroy the stack
*/
void stack_destroy(Stack stack) {

    if (not stack) return;

    array_destroy(stack->array);
    free(stack);
}


/**
 * Check if stack is empty, return true if it is, false otherwise
*/
int stack_is_empty(Stack stack) {

    if (not stack) return -1;

    return stack->last == -1;    
}


/**
 * Return the top of the stack
*/
void* stack_top(Stack stack) {

    if (not stack) return NULL;

    return array_read(stack->array, stack->last);
}


/**
 * Push data in the stack
*/
void stack_push(Stack stack, void* data) {

    if (not stack) return;

    if (stack->last + 1 == stack->array->capacity) {

        array_resize(stack->array, stack->array->capacity * 2);
    }

    array_write(stack->array, data, ++stack->last);
}


/**
 * Pop data from the stack
*/
void stack_pop(Stack stack) {

    if (not stack) return;

    array_delete(stack->array, stack->last--);
}


/**
 * Print the stack
*/
void stack_print(Stack stack) {

    if (not stack) return;

    array_print(stack->array);
}