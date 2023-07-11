#include "heap.h"


/**
 * Create a heap with the given capacity
*/
BHeap bheap_create(int capacity, PriorityType type, FunctionCopy copy, FunctionDestroy destroy, FunctionCompare compare, FunctionVisit visit) {
    
    // Ask for memory to create the heap
    BHeap newHeap = malloc(sizeof(struct _BHeap));
    newHeap->array = malloc(sizeof(void*) * (capacity + 1)); // +1 because heap[0] == NULL

    newHeap->capacity = capacity;
    newHeap->last = 0;

    newHeap->type = type;
    
    newHeap->copy = copy;
    newHeap->destroy = destroy;
    newHeap->compare = compare;
    newHeap->visit = visit;

    return newHeap;
}


/**
 * Destroy the heap
*/
void bheap_destroy(BHeap heap) {
    
    if (not heap) return;
    
    for (int i = 1; i <= heap->last; i++) {
    
        heap->destroy(heap->array[i]);
    }
    free(heap->array);
    free(heap);
}


/**
 * Check the type of the heap to use its compare function 
*/
int bheap_comparation(BHeap heap, void* a, void* b) {
    
    // Check the type of the heap
    if (heap->type == MAX) {

        // Compare towards the max
        return heap->compare(a, b);
    }
    
    else {

        // Compare towards the min
        return heap->compare(b, a);
    }
}


/**
 * Check if the given heap is empty, return true if its, false otherwise
*/
int bheap_is_empty(BHeap heap) {

    if (not heap) return false;

    return (heap->last == 0 ? true : false);
}


/**
 * Print the heap
*/
void bheap_print(BHeap heap) {

    if (not heap) return;
    
    for (int i = 1; i <= heap->last; i++) {

        heap->visit(heap->array[i]);
    }
}


/**
 * Let climb some element at the given index of the heap
 * Return true if the element climb at least one time, 
 * false otherwise
*/
int bheap_climb(BHeap heap, int index) {

    void* aux;
    int swapHappend = false;

    // While its not at the top, and can climb up
    for (; index > 1 and bheap_comparation(heap, heap->array[index], heap->array[index/2]) > 0
        ; index = index/2) {

        // Swap the child with the father
        aux = heap->array[index];
        heap->array[index] = heap->array[index/2];
        heap->array[index/2] = aux;

        // At least one swap happend
        swapHappend = true;
    }

    return swapHappend;
}


/**
 * Insert the given data in the heap
*/
void bheap_insert(BHeap heap, void* data) {

    if (not heap) return;

    // Put the element in the last position
    heap->array[++heap->last] = heap->copy(data);

    // Let climb the last element
    bheap_climb(heap, heap->last);
}


/**
 * Let fall some element at the given index of the heap
 * Return true if the element fall at least one time,
 * false otherwise
*/
int bheap_fall(BHeap heap, int index) {
    
    if (not heap) return 0;

    void* aux;
    int k, canFall = true, swapHappend = false;

    // While the childs exist and can keep falling
    while (2 * index <= heap->last and canFall) {
        
        k = 2 * index;

        // Choose the best child (lower or greater depends on priority)
        if (k + 1 <= heap->last and bheap_comparation(heap, heap->array[k+1], heap->array[k]) > 0)
            k++;

        // If cant fall anymore
        if (bheap_comparation(heap, heap->array[index], heap->array[k]) > 0) {
            canFall = false;
        }

        // Otherwise swap the father with the best child
        else {
            
            aux = heap->array[index];
            heap->array[index] = heap->array[k];
            heap->array[k] = aux;
            index = k;

            // At least one swap happend
            swapHappend = true;
        }
    }

    return swapHappend;
}


/**
 * Delete the top of the heap
*/
void bheap_pop(BHeap heap) {
    
    if (not heap) return;

    // Destroy the top and put the last element in place
    heap->destroy(heap->array[1]);
    heap->array[1] = heap->array[heap->last--];

    // Let fall the top of the heap
    bheap_fall(heap, 1);
}


/**
 * Delete some given data from the heap
*/
void bheap_delete(BHeap heap, void* data) {

    if (not heap) return;

    // Search data in the heap
    int i = 1, found = false;
    for (; i <= heap->last and not found ; i++) {
        
        if (heap->compare(heap->array[i], data) == 0) {

            // Data found, save the index
            found = true;
            i--;
        }
    }

    // If it was found
    if (found) {

        // Delete the data
        heap->destroy(heap->array[i]);

        // Replace it with the last element
        heap->array[i] = heap->array[heap->last];
        heap->last--;

        // Let fall that element
        bheap_fall(heap, i);
    }
}


/**
 * Return a new heap created from the given array
*/
BHeap bheap_create_from_array(void** array, int length, PriorityType type, FunctionCopy copy, FunctionDestroy destroy, FunctionCompare compare, FunctionVisit visit) {
    
    if (not array) return NULL;

    BHeap heap = bheap_create(length, type, copy, destroy, compare, visit);

    // Make a copy of the given array
    heap->array[0] = NULL;
    int i = 0;
    for (; i < length; i++) heap->array[i+1] = copy(array[i]);

    // Try to go up with all the nodes
    while(i > 1) {

        // When the index cant climb anymore
        if (not bheap_climb(heap, i))
            i--;
    }

    return heap;
}
