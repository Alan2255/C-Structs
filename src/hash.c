#include "hash.h"


/**
 * Hash table
*/

/**
 * Colission types:
 * 
 * Open hashing
 * * Separate chaining (to implement)
 * * Mixed list (to implement)
 *
 * Closed hashing
 * * Linear probing (to implement)
 * * Quadratic probing (to implement) (only change one function)
 * * Double hashing (to implement)
*/


/**
 * Open hashing: separate chaining
*/


/**
 * Crete an empty hash table
*/
HashOC hash_oc_create(int capacity, FunctionCopy copy, FunctionCompare compare, FunctionDestroy destroy, FunctionHash hash) {

    HashOC table = malloc(sizeof(struct _HashOpenChaining));
    
    table->array = malloc(sizeof(OC_Cell) * capacity);
    for (int i = 0; i < capacity; i++) {

        table->array[i] = list_create();
    }

    table->capacity = capacity;
    table->stuffed = 0;
    table->copy = copy;
    table->compare = compare;
    table->destroy = destroy;
    table->hash = hash;

    return table;
}


/**
 * Return the capacity of the hash table
 */
int hash_oc_capacity(HashOC table) { return table->capacity; }


/**
 * Return the amount of stuffed cells in the hash table
 */
int hash_oc_stuffed(HashOC table) { return table->stuffed; }


/**
 * Destroy the hash table
 */
void hash_oc_destroy(HashOC table) {

    if (not table) return;

    for (int i = 0; i < table->capacity; i++) {

        list_destroy(table->array[i], table->destroy);
    }

    free(table->array);
    free(table);
}


/**
 * Insert given data in the table managing collisions with separate chaning
 */
void hash_oc_insert(HashOC table, void* data) {

    if (not table) return;


    // Calculate the charge factor and evaluate if needs to rehash
    if ((float) table->stuffed / (float) table->capacity > OVERLOAD_CHARGE_FACTOR) {

        hash_oc_rehash(table);
    }

    // Calculate key of data
    int idx = table->hash(data) % table->capacity;

    // If the cell its empty
    if (not table->array[idx]) {

        table->array[idx] = list_add(table->array[idx], data, table->copy);
        table->stuffed++;
    }

    // If the cell already have some data
    else {

        // Try to find data in the list
        List dataSearch = list_search_node(table->array[idx], data, table->compare);

        // If its found
        if (dataSearch) {

            // Destroy the data to replace it without loss memory
            table->destroy(dataSearch->data);
            dataSearch->data = table->copy(data);
            table->stuffed--;
        }

        // Otherwise add data to the list
        else {

            table->array[idx] = list_add(table->array[idx], data, table->copy);
            table->stuffed++;
        }
    }
}


/**
 * Search given data on the hash table, return data if its found, NULL otherwise
 */
void* hash_oc_search(HashOC table, void* data) {

    if (not table) return NULL;

    // Calculate key of data
    int idx = table->hash(data) % table->capacity;

    // Search data
    return list_search_data(table->array[idx], data, table->compare);
}


/**
 * Delete given data from the hash table if exist on it
 */
void hash_oc_delete(HashOC table, void* data) {

    if (not table) return;

    // Calculate key of data
    int idx = table->hash(data) % table->capacity;
    
    // If there is no data
    if (not table->array[idx]) {

        return;
    }

    // There is a list in which we want to search data
    else {

        // We want to know the index of the list to delete data in order to 
        // only manage the list with the provided functions
        // (*) Otherwise we can manage the list manually and only go through the list 
        // one time but anyways the time cost is O(n) whenever we iterate one or two times
        int index_list = 0;
        List node = table->array[idx];
        for (; node != NULL and table->compare(node->data, data) != 0; node = node->next, index_list++);

        // If we found data
        if (node != NULL) {

            // Delete data
            table->array[idx] = list_delete(table->array[idx], index_list, table->destroy);
            table->stuffed--;
        }
    }
}


/**
 * Resize the hash table at double of its capacity and rehash each of its elements
 */
void hash_oc_rehash(HashOC table) {

    if (not table) return;

    OC_Cell *oldArray = table->array;

    // Ask memory for the new double size array
    table->capacity = table->capacity * 2;
    table->array = malloc(sizeof(OC_Cell) * table->capacity);

    // Initialize the new array
    for (int i = 0; i < table->capacity; i++) {

        table->array[i] = list_create();
    }

    // Rehash
    table->stuffed = 0;
    for (int i = 0; i < table->capacity / 2; i++) {

        // There is a list in the cell
        if (oldArray[i]) {

            // Insert each data in the list
            for (List node = oldArray[i]; node != NULL; node = node->next) {

                hash_oc_insert(table, node->data);
            }
        }

        list_destroy(oldArray[i], table->destroy);
    }

    free(oldArray);
}


/**
 * Open hashing: mixed list
*/


/**
 * Crete an empty hash table
*/
HashML hash_ml_create(int capacity, FunctionCopy copy, FunctionCompare compare, FunctionDestroy destroy, FunctionHash hash);


/**
 * Return the capacity of the hash table
 */
int hash_ml_capacity(HashML table);


/**
 * Return the amount of stuffed cells in the hash table
 */
int hash_ml_stuffed(HashML table);


/**
 * Destroy the hash table
 */
void hash_ml_destroy(HashML table);


/**
 * Insert given data in the table managing collisions with separate chaning
 */
void hash_ml_insert(HashML table, void* data);


/**
 * Search given data on the hash table, return data if its found, NULL otherwise
 */
void* hash_ml_search(HashML table, void* data);


/**
 * Delete given data from the hash table if exist on it
 */
void hash_ml_delete(HashML table, void* data);


/**
 * Resize the hash table at double of its capacity and rehash each of its elements
 */
void hash_ml_rehash(HashML table);


/**
 * Closed hashing: linear probing
*/


/**
 * Crete an empty hash table
*/
HashLP hash_lp_create(int capacity, FunctionCopy copy, FunctionCompare compare, FunctionDestroy destroy, FunctionHash hash);


/**
 * Return the capacity of the hash table
 */
int hash_lp_capacity(HashLP table);


/**
 * Return the amount of stuffed cells in the hash table
 */
int hash_lp_stuffed(HashLP table);


/**
 * Destroy the hash table
 */
void hash_lp_destroy(HashLP table);


/**
 * Insert given data in the table managing collisions with separate chaning
 */
void hash_lp_insert(HashLP table, void* data);


/**
 * Search given data on the hash table, return data if its found, NULL otherwise
 */
void* hash_lp_search(HashLP table, void* data);


/**
 * Delete given data from the hash table if exist on it
 */
void hash_lp_delete(HashLP table, void* data);


/**
 * Resize the hash table at double of its capacity and rehash each of its elements
 */
void hash_lp_rehash(HashLP table);

