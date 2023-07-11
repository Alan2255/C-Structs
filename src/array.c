#include "array.h"


/**
 * Array 
*/

/**
 * Create an empty array
*/
Array array_create(int capacity, FunctionCopy copy, FunctionDestroy destroy, FunctionCompare compare, FunctionVisit visit) {

    Array newArray = malloc(sizeof(struct _Array));
    newArray->at = malloc(sizeof(void*) * capacity);

    // Initialize the array
    for (int i = 0; i < capacity; i++) {

        newArray->at[i] = NULL;
    }

    newArray->capacity = capacity;
    newArray->copy = copy;
    newArray->destroy = destroy;
    newArray->compare = compare;
    newArray->visit = visit;

    return newArray;
}


/**
 * Return the capacity of the array
*/
int array_capacity(Array array) { return array->capacity; }


/**
 * Destroy the array
*/
void array_destroy(Array array) {

    if (not array) return;

    for (int i = 0; i < array->capacity; i++) {

        if (array->at[i] exist) {

            array->destroy(array->at[i]);
        }
    }

    free(array->at);
    free(array);
}


/**
 * Return data at given index of the array
*/
void* array_read(Array array, int i) {

    if (not array or i < 0 or i >= array->capacity) return NULL;

    return array->at[i];
}


/**
 * Write data at given index of the array
*/
void array_write(Array array, void* data, int i) {

    if (not array or i < 0 or i >= array->capacity) return;

    // If there is something there already
    if (array->at[i] exist) {

        // Destroy it to not lose memory
        array->destroy(array->at[i]);
    }

    // Put data at index
    array->at[i] = array->copy(data);
}


/**
 * Delete data from given index if exist
*/
void array_delete(Array array, int i) {

    if (not array) return;

    if (array->at[i] exist) {

        array->destroy(array->at[i]);
        array->at[i] = NULL;
    }
}


/**
 * Print the array
*/
void array_print(Array array) {

    if (not array) return;

    for (int i = 0; i < array->capacity; i++) {

        printf("[%i]: ", i);
        if (array->at[i] exist) {

            array->visit(array->at[i]);
        }
        
        puts("");
    }
}


/**
 * Adjust the size of the array
*/
void array_resize(Array array, int size) {

    if (not array or size <= 0 or size == array->capacity) return;

    // Truncate the array without lose memory
    if (size < array->capacity) {

        for (int i = size; i < array->capacity; i++) {

            array->destroy(array->at[i]);
        }
    }

    // Resize
    array->at = realloc(array->at, sizeof(void*) * size);

    // Initialize what is left over
    for (int i = array->capacity; i < size; i++) {

        array->at[i] = NULL;
    }

    array->capacity = size;
}


/**
 * Insert data in the given index
*/
void array_insert(Array array, void* data, int i) {

    if (not array or i < 0 or i >= array->capacity) return;

    // If the index was occupied
    if (array->at[i] exist) {

        // Variables to move data
        void* aux = array->at[i];
        void* swap;

        // Put data at index
        array->at[i] = array->copy(data);

        // Move each element to the right until there is space
        int j = i + 1;
        for (; j < array->capacity and array->at[j] exist; j++) {

            // Swap
            swap = array->at[j];
            array->at[j] = aux;
            aux = swap;
        }


        // If found free space
        if (j < array->capacity) {

            array->at[j] = aux;
        }

        // If reach the end
        else if (j == array->capacity) {

            array->destroy(aux);
        }
    }

    else {

        array->at[i] = array->copy(data);
    }
}