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
 * Search given data in the hash table 
 * Return index of the table if found, 
 * otherwise return the index of the first free cell
*/
int hash_search_idx(Hash table, void* data) {

    if (not table) return -1;

    // Calculate key of data
    int idx = table->hash(data) % table->capacity;

    // Save the index of the first deleted cell
    int first_deleted = idx;

    // Search data by probing
    for (int limit = 0, i = 0;
        limit < table->capacity and (table->array[idx]->deleted or
        (table->array[idx]->data exist and table->compare(table->array[idx]->data, data) != 0));
        limit++, idx = probing(idx, ++i, table->capacity, table->type)) {

            if (table->array[first_deleted]->data exist and not table->array[idx]->data) {
                first_deleted = idx;
            }
        }

    
    // If data found
    if (table->array[idx]->data exist and table->compare(table->array[idx]->data, data) == 0) {

        // Return index of data
        return idx;
    }

    // Data doesnt found
    else {

        // If there was some cell deleted
        if (table->array[first_deleted]->deleted) {

            // Return the index of the first deleted cell
            return first_deleted;
        }
        
        // There wasnt deleted cells
        else {

            // Return the index of the first empty cell
            return idx;
        }
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

    // Search index of data in the table
    int idx = hash_search_idx(table, data);

    // If data already exist in the table
    if (table->array[idx]->data exist) {

        // Destroy to replace without lose memory
        table->destroy(table->array[idx]->data);
        
        // Replace data
        table->array[idx]->data = table->copy(data);
    }    

    // If data doesnt exist in the table
    else {

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

    // Search index of data in the table
    int idx = hash_search_idx(table, data);

    // If data already exist in the hash table
    if (table->array[idx]->data exist) {

        // Delete data
        table->destroy(table->array[idx]->data);
        table->array[idx]->data = NULL;
        table->array[idx]->deleted = true;
        table->stuffed--;
    }    
} 


/**
 * Auxiliar "copy" function to rehash
 * (do not copy actually)
*/
void* pointer(void* data) {

    return data;
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

    // Save the copy function
    FunctionCopy copy = table->copy;

    table->copy = pointer;

    // Rehash each element
    for (int i = 0; i < table->capacity / 2; i++) {

        // If exist in the old array
        if (oldArray[i]->data exist) {

            // Rehash
            hash_add(table, oldArray[i]->data);
        }

        // Free old cell
        free(oldArray[i]);
    }

    // Get the copy function back
    table->copy = copy;

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