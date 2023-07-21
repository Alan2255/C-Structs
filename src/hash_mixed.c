#include "hash_mixed.h"


/**
 * Hash table
 * 
 * Openg hashing: mixed list
*/


/**
 * Overload charge factor to decide when to rehash
*/
#define OVERLOAD_CHARGE_FACTOR 0.75


/**
 * Create an empty hash table
*/
Hash hash_create(int capacity, FunctionCopy copy, FunctionDestroy destroy, FunctionCompare compare, FunctionVisit visit, FunctionHash hash) {

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
        newTable->array[i]->next= NULL;
    }

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

    if (not table) return NULL;

    // Calculate key of data
    int idx = table->hash(data) % table->capacity;

    // Search data in the cell and next to it
    Cell aux = table->array[idx];
    for (; aux exist and aux->data exist and table->compare(aux->data, data) != 0; aux = aux->next);

    // If data found
    if (aux exist) {

        return aux->data;
    }

    // Data not found
    else {

        return NULL;
    }
}


/**
 * Use any method to obtaing an empty cell
 * Just as an example, linear probing
*/
int guess(int x, int capacity) {

    return (x + 1) % capacity;
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

    // If data already exist in the hash table
    if (hash_search(table, data) exist) {

        // Iterate through the cells till data
        Cell aux = table->array[idx];
        for (; aux exist and table->compare(aux->data, data) != 0; aux = aux->next);

        // Destroy to replace without lose memory
        table->destroy(aux->data);

        // Replace data
        aux->data = table->copy(data);
    }

    // Data doesnt exist in the hash table
    else {

        // As data doesnt exist, after add it we have one more element
        table->stuffed++;

        // If there is nothing in the cell
        if (not table->array[idx]->data) {

            // Add data
            table->array[idx]->data = table->copy(data);
        }

        // If there was something
        else {

            int replaced = false;
            int keyCell = table->hash(table->array[idx]->data) % table->capacity;

            // Check if need to prioritize key over cell
            if (idx != keyCell) {

                // Replace data with the cell data
                void* auxData = table->array[idx]->data;

                table->array[idx]->data = table->copy(data);

                idx = keyCell;
                data = auxData;
                replaced = true;
            }

            // Go through the list till the end for linking
            Cell aux = table->array[idx];
            for (; aux->next exist; aux = aux->next);

            // Search any other empty cell
            for (; table->array[idx]->data exist; idx = guess(idx, table->capacity));

            // Add data and relink
            table->array[idx]->data = table->copy(data);
            aux->next = table->array[idx];

            // If the fist data was replaced to prioritize key
            if (replaced) {

                table->destroy(data);
            }
        }   
    }
}


/**
 * Delete given data from the hash table
*/
void hash_delete(Hash table, void* data) {

    if (not table) return;

    // If data already exist in the hash table
    if (hash_search(table, data) exist) {

        // As data exist and we are going to delete it, after that there is one element less
        table->stuffed--;

        // Calculate key of data
        int idx = table->hash(data) % table->capacity;
        
        // Auxiliar cell to go through the table
        Cell aux = table->array[idx];

        // If its the first
        if (table->compare(aux->data, data) == 0) {
            
            // Delete data
            table->destroy(aux->data);
            aux->data = NULL;

            // If it has some next
            if (aux->next exist) {
                
                // Relink
                Cell relink = aux->next;

                aux->data = relink->data;
                aux->next = relink->next;

                relink->data = NULL;
                relink->next = NULL;
            }
        }

        // It wasnt the first
        else {

            
            // Search data in the nexts cells to the first
            for (; aux->next exist and table->compare(aux->next->data, data) != 0; aux = aux->next);
            
            // If data found
            if (aux->next exist) {

                // Delete data and relink
                Cell relink = aux->next;

                aux->next = relink->next;

                table->destroy(relink->data);
                relink->data = NULL;
                relink->next = NULL;

            }
        }
    }
}


/**
 * Resize the hash table at double of its capacity and rehash each of its elements
*/
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
        table->array[i]->next= NULL;
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

        // Print the next cell
        printf(" Next: ");

        // If the next cell exist
        if (table->array[i]->next exist) {

            table->visit(table->array[i]->next->data);
        }

        // If doesnt exist
        else {

            printf("NULL");
        }

        puts("");
    }
}