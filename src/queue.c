#include "queue.h"


/**
 * Queue
*/

/**
 * Create an empty queue
*/
Queue queue_create(FunctionCopy copy, FunctionDestroy destroy, FunctionCompare compare, FunctionVisit visit) {

    Queue newQueue = malloc(sizeof(struct _Queue));

    newQueue->list = dclist_create();
    newQueue->copy = copy;
    newQueue->compare = compare;
    newQueue->destroy = destroy;
    newQueue->visit = visit;

    return newQueue;
}


/**
 * Destroy the queue
*/
void queue_destroy(Queue queue) {

    if (not queue) return;

    dclist_destroy(queue->list, queue->destroy);
    free(queue);
}


/**
 * Check if queue is empty, return true if it is, false otherwise
*/
int queue_is_empty(Queue queue) {

    if (not queue) return -1;

    return queue->list == NULL;
}


/**
 * Return the top of the queue
*/
void* queue_top(Queue queue) {
    
    if (not queue) return NULL;

    return queue->list exist ? queue->list->data : NULL;
}


/**
 * Push data in the queue
*/
void queue_push(Queue queue, void* data) {

    if (not queue) return;

    queue->list = dclist_add(queue->list, data, FORDWARD, queue->copy);
}


/**
 * Pop data from the queue
*/
void queue_pop(Queue queue) {

    if (not queue) return;

    queue->list = dclist_delete(queue->list, BACKWARD, queue->destroy);
}


/**
 * Print the queue
*/
void queue_print(Queue queue) {
    
    if (not queue) return;

    dclist_print(queue->list, FORDWARD, queue->visit);
}