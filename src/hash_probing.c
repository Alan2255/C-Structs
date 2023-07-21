#include "hash_probing.h"


/**
 * Hash table
 * 
 * Closed hashing: probing
*/


/**
 * Apply some probing
*/
int probing(int x, int i, int capacity, ProbingType type) {

    // Linear probing
    if (type == LINEAR) {

        return (x + 1) % capacity;
    }

    // Cuadratic probing
    else if (type == CUADRATIC) {

        return (x + C1 * i + C2 *i*i) % capacity;
    }
    
    // Double hashing
    // (!) May define some proper CO_PRIME to work
    else if (type == DOUBLE_HASHING) {

        return CO_PRIME - (x % CO_PRIME);
    }
    
    // None of the probing options 
    else {

        return -1;
    }
}


/**
 * Create an empty hash table
*/
Hash hash_create(int capacity, ProbingType type, FunctionCopy copy, FunctionDestroy destroy, FunctionCompare compare, FunctionVisit visit, FunctionHash hash) {

    // Ark memory for the hash table
    Hash newTable = malloc(sizeof(struct _Hash));

    newTable->capacity = capacity;
    newTable->stuffed = 0;
    
    // Ark memory for the array
    newTable->array = malloc(sizeof(Cell) * newTable->capacity);

    // Initialize the array
    for (int i = 0; i < newTable->capacity; i++) {

        // Ask memory for each cell
        newTable->array[i] = malloc(sizeof(struct _Cell));
        newTable->array[i]->data = NULL;
        newTable->array[i]->deleted = false;
    }

    newTable->type = type;

    newTable->copy = copy;
    newTable->destroy = destroy;
    newTable->compare = compare;
    newTable->visit = visit;
    newTable->hash = hash;

    return newTable;
}


/**
 * Destroy the hash table
*/
void hash_destroy(Hash table) {

    if (not table) return;

    // Iterate through the hash table
    for (int i = 0; i < table->capacity; i++) {

        // If data exist
        if (table->array[i]->data exist) {

            // Delete data
            table->destroy(table->array[i]->data);
        }

        // Free the cell
        free(table->array[i]);
    }
    
    // Free the array
    free(table->array);

    // Free the table
    free(table);
}


/**
 * Return the capacity of the hash table
 */
int hash_capacity(Hash table) { return table->capacity; }


/**
 * Return the amount of stuffed cells in the hash table
 */
int hash_stuffed(Hash table) { return table->stuffed; }


/**
 * Search given data in the hash table
*/
void* hash_search(Hash table, void* data) {

    if (not table) return  NULL;

    // Calculate key of data
    int idx = table->hash(data) % table->capacity;

    // Search data by probing
    for (int limit = 0, i = 0;
        limit < table->capacity and (table->array[idx]->deleted or
        (table->array[idx]->data exist and table->compare(table->array[idx]->data, data) != 0));
        limit++, idx = probing(idx, ++i, table->capacity, table->type));

    
    // If data found
    if (table->array[idx]->data exist and table->compare(table->array[idx]->data, data) == 0) {

        return table->array[idx]->data;
    }

    // Data doesnt found
    else {

        return NULL;
    }
} 


/**
 * Add given data to the hash table
*/
void hash_add(Hash table, void* data) {

    if (not table) return;

    // Calculate the charge factor and evaluate if needs to rehash
    if ((float) table->stuffed / (float) table->capacity > OVERLOAD_CHARGE_FACTOR) {

        // Rehash
        hash_rehash(table);
    }

    // Calculate key of data
    int idx = table->hash(data) % table->capacity;

    // If data already exist in the table
    if (hash_search(table, data) exist) {

        // Search data by probing
        for (int i = 0;
            table->array[idx]->deleted or 
            (table->array[idx]->data exist and table->compare(table->array[idx]->data, data) != 0);
            idx = probing(idx, ++i, table->capacity, table->type));

        // Destroy to replace without lose memory
        table->destroy(table->array[idx]->data);
        
        // Replace data
        table->array[idx]->data = table->copy(data);
    }    

    // If data doesnt exist in the table
    else {

        // Search an empty cell by probing
        for (int i = 0; table->array[idx]->data exist; idx = probing(idx, ++i, table->capacity, table->type));
    
        // Add data
        table->array[idx]->data = table->copy(data);
        table->array[idx]->deleted = false;
        table->stuffed++;
    }
}


/**
 * Delete given data from the hash table
*/
void hash_delete(Hash table, void* data) {

    if (not table) return;

    // If data already exist in the hash table
    if (hash_search(table, data) exist) {

        // Calculate key of data
        int idx = table->hash(data) % table->capacity;

        // Search data by probing
        for (int i = 0;
            table->array[idx]->deleted or 
            (table->array[idx]->data exist and table->compare(table->array[idx]->data, data) != 0);
            idx = probing(idx, ++i, table->capacity, table->type));

        // Delete data
        table->destroy(table->array[idx]->data);
        table->array[idx]->data = NULL;
        table->array[idx]->deleted = true;
        table->stuffed--;
    }    
} 


void hash_rehash(Hash table) {

    if (not table) return;

    // Auxiliar array to delete
    Cell *oldArray = table->array;

    // Resize the array of the table
    table->capacity *= 2 ;
    table->stuffed = 0;
    table->array = malloc(sizeof(Cell) * table->capacity);

    // Initialize the new array
    for (int i = 0; i < table->capacity; i++) {

        // Ask memory for each cell
        table->array[i] = malloc(sizeof(struct _Cell));
        table->array[i]->data = NULL;
        table->array[i]->deleted = false;
    }

    // Rehash each element
    for (int i = 0; i < table->capacity / 2; i++) {

        // If exist in the old array
        if (oldArray[i]->data exist) {

            // Rehash
            hash_add(table, oldArray[i]->data);

            // Delete from the old array
            table->destroy(oldArray[i]->data);
        }

        // Free old cell
        free(oldArray[i]);
    }

    // Free old array
    free(oldArray);
}


/**
 * Print the hash table
*/
void hash_print(Hash table) {

    if (not table) return;

    // Iterate through the hash table
    for (int i = 0; i < table->capacity; i++) {

        // At each cell
        printf("[%i]: ", i);
        
        // If data exist
        if (table->array[i]->data exist) {

            // Print data
            table->visit(table->array[i]->data);
        }

        // If doesnt exist
        else {

            printf("NULL");
        }

        // Print the delete state of the cell
        printf(" Deleted: %i\n", table->array[i]->deleted);
    }
}