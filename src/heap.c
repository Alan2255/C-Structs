#include "heap.h"


/**
 * Create a heap with the given capacity
*/
BHeap bheap_create(int capacity, FunctionCompare compare, PriorityType type) {
    
    // Ask for memory to create the heap
    BHeap newHeap = malloc(sizeof(*newHeap));
    void **array = malloc(sizeof(void*) * (capacity + 1));

    newHeap->array = array;
    newHeap->capacity = capacity;
    newHeap->last = 0;
    newHeap->compare = compare;
    newHeap->type = type;
    
    return newHeap;
}


/**
 * Destroy the heap
*/
void bheap_destroy(BHeap heap, FunctionDestroy destroy) {
    
    if (!heap) return;
    
    for (int i = 1; i <= heap->last; i++) {
    
        destroy(heap->array[i]);
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
 * Check if the given heap is empty, return 1 if its, 0 if it doesnt
 * and -1 if its NULL
*/
int bheap_is_empty(BHeap heap) {

    if (!heap) return -1;

    return (heap->last == 0 ? 1 : 0);
}


/**
 * Travel through the heap apllying the given function to each element 
*/
void bheap_travel(BHeap heap, FunctionVisit visit) {

    if (!heap) return;
    
    for (int i = 1; i <= heap->last; i++) {

        visit(heap->array[i]);
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
void bheap_insert(BHeap heap, void* data, FunctionCopy copy) {

    if (!heap) return;

    // Put the element in the last position
    heap->array[++heap->last] = copy(data);

    // Let climb the last element
    bheap_climb(heap, heap->last);
}


/**
 * Let fall some element at the given index of the heap
 * Return true if the element fall at least one time,
 * false otherwise
*/
int bheap_fall(BHeap heap, int index) {
    
    if (!heap) return 0;

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
void bheap_pop(BHeap heap, FunctionDestroy destroy) {
    
    if (!heap) return;

    // Destroy the top and put the last element in place
    destroy(heap->array[1]);
    heap->array[1] = heap->array[heap->last--];

    // Let fall the top of the heap
    bheap_fall(heap, 1);
}


/**
 * Delete some given data from the heap
*/
void bheap_delete(BHeap heap, void* data, FunctionDestroy destroy) {

    if (!heap) return;

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
        destroy(heap->array[i]);

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
BHeap bheap_create_from_array(void** array, int length, FunctionCopy copy, FunctionCompare compare, PriorityType type) {
    
    if (!array) return NULL;

    // Ask memory for a new heap and array
    BHeap newHeap = malloc(sizeof(*newHeap));
    void** newArray = malloc(sizeof(void*) * (length + 1)); // +1 because heap[0] == NULL
    
    newHeap->capacity = length;
    newHeap->last = length;
    newHeap->compare = compare;
    newHeap->type = type;

    // Make a copy of the given array
    newArray[0] = NULL;
    int i = 0;
    for (; i < length; i++) newArray[i+1] = copy(array[i]);

    // Put the array on the heap
    newHeap->array = newArray;
    
    // Try to go up with all the nodes
    while(i > 1) {

        // When the index cant climb anymore
        if (not bheap_climb(newHeap, i))
            i--;
    }

    return newHeap;
}
