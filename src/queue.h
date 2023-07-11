#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "list.h"


/**
 * Queue
*/
typedef struct _Queue {

    DCList list;

    FunctionCopy copy;
    FunctionDestroy destroy;
    FunctionCompare compare;
    FunctionVisit visit;

} *Queue;


/**
 * Create an empty queue
*/
Queue queue_create(FunctionCopy, FunctionDestroy, FunctionCompare, FunctionVisit);


/**
 * Destroy the queue
*/
void queue_destroy(Queue);


/**
 * Check if queue is empty, return true if it is, false otherwise
*/
int queue_is_empty(Queue);


/**
 * Return the top of the queue
*/
void* queue_top(Queue);


/**
 * Push data in the queue
*/
void queue_push(Queue, void*);


/**
 * Pop data from the queue
*/
void queue_pop(Queue);


/**
 * Print the queue
*/
void queue_print(Queue);


#endif