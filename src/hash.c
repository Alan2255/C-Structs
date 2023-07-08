#include "hash.h"


/**
 * Hash table
*/

/**
 * Colission types:
 * 
 * Open hashing
 * * Separate chaining
 * * Mixed list (to implement)
 *
 * Closed hashing
 * * Linear probing
 * * Quadratic probing (to implement) (only change one function)
 * * Double hashing (to implement)
*/



/**
 * Built in linked list for separate chaining
*/

/**
 * Create an empty linked list
*/

List list_create() { return NULL; }


/**
 * Add data to the begin of the list
*/
List list_add(List list, void* data, FunctionCopy copy) {

    List newNode = malloc(sizeof(struct _LNode));
    newNode->data = copy(data);
    newNode->next = list;

    return newNode;
}


/**
 * Destroy the list
*/
void list_destroy(List list, FunctionDestroy destroy) {

    if (not list) return;

    list_destroy(list->next, destroy);
    
    destroy(list->data);
    free(list);
}


/**
 * Print the list
*/
void list_print(List list, FunctionVisit visit) {

    if (not list) return;

    visit(list->data);
    list_print(list->next, visit);
}


/**
 * Delete data at some given index in the list
*/
List list_delete(List list, int index, FunctionDestroy destroy) {

    if (not list) return NULL;

    // Node to delete data
    List nodeDelete = list;

    // If we want to delete the first node
    if (index == 0) {

        list = list->next;
        destroy(nodeDelete->data);
        free(nodeDelete);

        return list;
    }

    // Otherwise we want to move through the list till the index (or the end)
    List node = list;
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
 * Search data in the list, return data (pointer on list) if its in, NULL otherwise
*/
void* list_search_data(List list, void* data, FunctionCompare compare) {

    if (not list) return NULL;

    if (compare(list->data, data) == 0) return list->data;
    
    else return list_search_data(list->next, data, compare);
}


/**
 * Search data in the list, return the node of data if its on it, NULL otherwise
*/
List list_search_node(List list, void* data, FunctionCompare compare) {

    if (not list) return NULL;

    if (compare(list->data, data) == 0) return list;
    
    else return list_search_node(list->next, data, compare);
}


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
        for (; node exist and table->compare(node->data, data) != 0; node = node->next, index_list++);

        // If we found data
        if (node exist) {

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
            for (List node = oldArray[i]; node exist; node = node->next) {

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
 * Function to apply linear probing
*/
int linear_probing(int x, int capacity) {

    return (x + 1) % capacity;
}


/**
 * Crete an empty hash table
*/
HashLP hash_lp_create(int capacity, FunctionCopy copy, FunctionCompare compare, FunctionDestroy destroy, FunctionHash hash) {
    
    HashLP table = malloc(sizeof(struct _HashLinearProbing));
    
    table->array = malloc(sizeof(LP_Cell) * capacity);
    
    // Initialize the array
    for (int i = 0; i < capacity; i++) {

        table->array[i].data = NULL;
        table->array[i].deleted = false;
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
int hash_lp_capacity(HashLP table) { return table->capacity; }


/**
 * Return the amount of stuffed cells in the hash table
 */
int hash_lp_stuffed(HashLP table) { return table->stuffed; }


/**
 * Destroy the hash table
 */
void hash_lp_destroy(HashLP table) {

    if (not table) return;

    for (int i = 0; i < table->capacity; i++) {

        if (table->array[i].data exist) {

            table->destroy(table->array[i].data);
        }
    }

    free(table->array);
    free(table);
}


/**
 * Insert given data in the table managing collisions with separate chaning
 */
void hash_lp_insert(HashLP table, void* data) {

    if (not table) return;

    // Calculate the charge factor and evaluate if needs to rehash
    if ((float) table->stuffed / (float) table->capacity > OVERLOAD_CHARGE_FACTOR) {
    
        // Rehash
        hash_lp_rehash(table);
    }

    // Calculate key of data
    int probing = table->hash(data) % table->capacity;

    // Search data in the table
    void* dataSearch = hash_lp_search(table, data);


    // If data is already in the table
    if (dataSearch exist) {

        // Search data to replace by linear probing
        for (; table->array[probing].deleted or table->compare(table->array[probing].data, data) != 0; 
            probing = linear_probing(probing, table->capacity));

        // Replace it
        table->destroy(table->array[probing].data);
        table->array[probing].data = table->copy(data);
    }

    // Otherwise if data is not in the table
    else {

        // Search an empty or deleted cell by linear probing
        for (; table->array[probing].data exist and not table->array[probing].deleted; 
            probing = linear_probing(probing, table->capacity));
    
        // Insert data
        table->array[probing].data = table->copy(data); 
        table->array[probing].deleted = false;
        table->stuffed++;
    }
}


/**
 * Search given data on the hash table, return data if its found, NULL otherwise
 */
void* hash_lp_search(HashLP table, void* data) {

    if (not table) return NULL;

    // Calculate key of data
    int probing = table->hash(data) % table->capacity;

    // Search data by linear probing
    for (int maxProbing = 0; 
        maxProbing < table->capacity and
        ((table->array[probing].data exist and table->compare(table->array[probing].data, data) != 0) or
        table->array[probing].deleted);
        probing = linear_probing(probing, table->capacity), maxProbing++);
    
    // Data found
    if (table->array[probing].data exist and table->compare(table->array[probing].data, data) == 0) {
        
        return table->array[probing].data;
    }
    // Data not found
    else {
        
        return NULL;
    }
}


/**
 * Delete given data from the hash table if exist on it
 */
void hash_lp_delete(HashLP table, void* data) {

    if (not table) return;

    // Calculate key of data
    int probing = table->hash(data) % table->capacity;

    // Search data by linear probing
    for (int maxProbing = 0; 
        maxProbing < table->capacity and
        ((table->array[probing].data exist and table->compare(table->array[probing].data, data) != 0) or
        table->array[probing].deleted);
        probing = linear_probing(probing, table->capacity), maxProbing++);
    
    // If it was found
    if (table->array[probing].data exist and table->compare(table->array[probing].data, data) == 0) {
    
        // Delete data and mark as deleted
        table->destroy(table->array[probing].data);
        table->array[probing].data = NULL;
        table->array[probing].deleted = true;
        table->stuffed--;
    }
}   


/**
 * Resize the hash table at double of its capacity and rehash each of its elements
 */
void hash_lp_rehash(HashLP table) {

    if (not table) return;

    LP_Cell *oldArray = table->array;

    // Ask memory for the new double size array
    table->capacity = table->capacity * 2;
    table->array = malloc(sizeof(LP_Cell) * table->capacity);

    // Initialize the new array
    for (int i = 0; i < table->capacity; i++) {

        table->array[i].data = NULL;
        table->array[i].deleted = false;
    }

    // Rehash
    table->stuffed = 0;
    for (int i = 0; i < table->capacity / 2; i++) {

        // If there is data not deleted
        if (oldArray[i].data exist and not oldArray[i].deleted) {

            // Rehash
            hash_lp_insert(table, oldArray[i].data);
        }

        // Anyway we destroy the deleted data
        if (oldArray[i].data exist) {

            table->destroy(oldArray[i].data);
        }
    }

    free(oldArray);
}