#include "hash_chaining.h"


/**
 * Hash table
 * 
 * Open hashing: separate chaining
*/


/**
 * Built in linked list for separate chaining
*/

/**
 * Create an empty linked list
*/
HList hlist_create() { return NULL; }


/**
 * Add data to the begin of the list
*/
HList hlist_add(HList list, void* data, FunctionCopy copy) {

    HList newNode = malloc(sizeof(struct _HNode));
    newNode->data = copy(data);
    newNode->next = list;

    return newNode;
}


/**
 * Destroy the list
*/
void hlist_destroy(HList list, FunctionDestroy destroy) {

    if (not list) return;

    hlist_destroy(list->next, destroy);
    
    destroy(list->data);
    free(list);
}


/**
 * Print the list
*/
void hlist_print(HList list, FunctionVisit visit) {

    if (not list) return;

    visit(list->data);
    hlist_print(list->next, visit);
}


/**
 * Delete data at some given index in the list
*/
HList hlist_delete(HList list, int index, FunctionDestroy destroy) {

    if (not list) return NULL;

    // Node to delete data
    HList nodeDelete = list;

    // If we want to delete the first node
    if (index == 0) {

        list = list->next;
        destroy(nodeDelete->data);
        free(nodeDelete);

        return list;
    }

    // Otherwise we want to move through the list till the index (or the end)
    HList node = list;
    for (int i = 0; i < index-1 and node->next != NULL; i++, node = node->next);

    // If we reach the index
    if (node->next != NULL) {

        nodeDelete = node->next;
        node->next = node->next->next; // Relink
        
        destroy(nodeDelete->data);
        free(nodeDelete);
    }

    return list;
}


/**
 * Search data in the list, return the index of data if its on it, -1 otherwise
*/
int hlist_search_index(HList list, void* data, FunctionCompare compare) {

    if (not list) return -1;

    if (compare(list->data, data) == 0) return 0;
    
    int index = hlist_search_index(list->next, data, compare);

    if (index == -1) return -1;
    else return 1 + index;
}



/**
 * Search data in the list, return data (pointer on list) if its in, NULL otherwise
*/
void* hlist_search_data(HList list, void* data, FunctionCompare compare) {

    if (not list) return NULL;

    if (compare(list->data, data) == 0) return list->data;
    
    else return hlist_search_data(list->next, data, compare);
}


/**
 * Search data in the list, return the node of data if its on it, NULL otherwise
*/
HList hlist_search_node(HList list, void* data, FunctionCompare compare) {

    if (not list) return NULL;

    if (compare(list->data, data) == 0) return list;
    
    else return hlist_search_node(list->next, data, compare);
}


/**
 * Create an empty hash table
*/
Hash hash_create(int capacity, FunctionCopy copy, FunctionDestroy destroy, FunctionCompare compare, FunctionVisit visit, FunctionHash hash) {

    Hash newTable = malloc(sizeof(struct _Hash));

    newTable->capacity = capacity;
    newTable->stuffed = 0;
    
    newTable->array = malloc(sizeof(Cell) * newTable->capacity);

    for (int i = 0; i < newTable->capacity; i++) {

        newTable->array[i] = hlist_create();
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

        // If there is a list on the cell
        if (table->array[i] exist) {

            // Destroy the list
            hlist_destroy(table->array[i], table->destroy);
        }
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

    // Search data in the cell of the key
    return hlist_search_data(table->array[idx], data, table->compare);
}


/**
 * Add given data to the hash table
*/
void hash_add(Hash table, void* data) {

    // Calculate the charge factor and evaluate if needs to rehash
    if ((float) table->stuffed / (float) table->capacity > OVERLOAD_CHARGE_FACTOR) {

        // Rehash
        hash_rehash(table);
    }

    // Calculate key of data
    int idx = table->hash(data) % table->capacity;
    
    // Search the node of data in the hash table
    Cell searchNode = hlist_search_node(table->array[idx], data, table->compare);
    
    // If data already exist in the hash table
    if (searchNode exist) {

        // Destroy to replace without lose memory
        table->destroy(searchNode->data);

        // Replace data
        searchNode->data = table->copy(data);
    }

    // If data doesnt exist in the hash table
    else {

        // Add data
        table->array[idx] = hlist_add(table->array[idx], data, table->copy);
        table->stuffed++;
    }
}


/**
 * Delete given data from the hash table
*/
void hash_delete(Hash table, void* data) {

    if (not table) return;

    // Calculate key of data
    int idx = table->hash(data) % table->capacity;

    
    // Search index of data
    int searchIndex = hlist_search_index(table->array[idx], data, table->compare);

    // If data already exist in the hash table
    if (searchIndex != -1) {

        // Delete data
        table->array[idx] = hlist_delete(table->array[idx], searchIndex, table->destroy);
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
    table->capacity *= 2;
    table->stuffed = 0;
    table->array = malloc(sizeof(Cell) * table->capacity);

    // Initialize the new array
    for (int i = 0; i < table->capacity; i++) {

        table->array[i] = hlist_create();
    }

    // Rehash each element
    for(int i = 0; i < table->capacity / 2; i++) {

        // If there is a list on the cell
        if (oldArray[i] exist) {

            // Iterate through the list
            for (HList aux = oldArray[i]; aux exist; aux = aux->next) {

                // Rehash
                hash_add(table, aux->data);
            }

            // Destroy the list
            hlist_destroy(oldArray[i], table->destroy);
        }
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
        
        // If there is a list on the cell
        if (table->array[i] exist) {

            // Travel through the list
            for (HList aux = table->array[i]; aux exist; aux = aux->next) {

                // Print data
                table->visit(aux->data);
                printf(" ");
            }
        }

        // If doesnt exist
        else {

            printf("NULL");
        }

        puts("");
    }
}