#ifndef __HASH_H__
#define __HASH_H__

#include <stdlib.h>
#include "void.h"
#include "sugar.h"


/**
 * Hash table
*/

/**
 * Colission types:
 * 
 * Open hashing
 * * Separate chaining
 * * Mixed list
 *
 * Closed hashing
 * * Linear probing
 * * Quadratic probing 
 * * Double hashing
*/


/**
 * Overload charge factor to decide when to rehash
*/
#define OVERLOAD_CHARGE_FACTOR 0.75


/**
 * Built in linked list
*/
typedef struct _LNode {

    void* data;
    struct _LNode *next;

} *List;


/**
 * Open hashing: separate chaining
*/
typedef List OC_Cell;


/**
 * Hash table
*/
typedef struct _HashOpenChaining {

    OC_Cell *array;

    int capacity;
    int stuffed;
    
    FunctionCopy copy;
    FunctionCompare compare;
    FunctionDestroy destroy;
    FunctionHash hash;

} *HashOC;


/**
 * Crete an empty hash table
*/
HashOC hash_oc_create(int, FunctionCopy, FunctionCompare, FunctionDestroy, FunctionHash);


/**
 * Return the capacity of the hash table
 */
int hash_oc_capacity(HashOC);


/**
 * Return the amount of stuffed cells in the hash table
 */
int hash_oc_stuffed(HashOC);


/**
 * Destroy the hash table
 */
void hash_oc_destroy(HashOC);


/**
 * Insert given data in the table managing collisions with separate chaning
 */
void hash_oc_insert(HashOC, void*);


/**
 * Search given data on the hash table, return data if its found, NULL otherwise
 */
void* hash_oc_search(HashOC, void*);


/**
 * Delete given data from the hash table if exist on it
 */
void hash_oc_delete(HashOC, void*);


/**
 * Resize the hash table at double of its capacity and rehash each of its elements
 */
void hash_oc_rehash(HashOC);


/**
 * Open hashing: mixed list
*/
typedef struct _ML_Cell {

    void* data;
    struct _ML_Cell* next;
} *ML_Cell;


/**
 * Hash table
*/
typedef struct _HashMixedList {

    ML_Cell *array;

    int capacity;
    int stuffed;
    
    FunctionCopy copy;
    FunctionCompare compare;
    FunctionDestroy destroy;
    FunctionHash hash;

} *HashML;


/**
 * Crete an empty hash table
*/
HashML hash_ml_create(int, FunctionCopy, FunctionCompare, FunctionDestroy, FunctionHash);


/**
 * Return the capacity of the hash table
 */
int hash_ml_capacity(HashML);


/**
 * Return the amount of stuffed cells in the hash table
 */
int hash_ml_stuffed(HashML);


/**
 * Destroy the hash table
 */
void hash_ml_destroy(HashML);


/**
 * Insert given data in the table managing collisions with separate chaning
 */
void hash_ml_insert(HashML, void*);


/**
 * Search given data on the hash table, return data if its found, NULL otherwise
 */
void* hash_ml_search(HashML, void*);


/**
 * Delete given data from the hash table if exist on it
 */
void hash_ml_delete(HashML, void*);


/**
 * Resize the hash table at double of its capacity and rehash each of its elements
 */
void hash_ml_rehash(HashML);


/**
 * Closed hashing: linear probing
*/

/**
 * Cell
*/
typedef struct _LP_Cell {

    void* data;
    int deleted;

} LP_Cell;


/**
 * Hash table
*/
typedef struct _HashLinearProbing {

    LP_Cell *array;

    int capacity;
    int stuffed;
    
    FunctionCopy copy;
    FunctionCompare compare;
    FunctionDestroy destroy;
    FunctionHash hash;

} *HashLP;


/**
 * Crete an empty hash table
*/
HashLP hash_lp_create(int, FunctionCopy, FunctionCompare, FunctionDestroy, FunctionHash);


/**
 * Return the capacity of the hash table
 */
int hash_lp_capacity(HashLP);


/**
 * Return the amount of stuffed cells in the hash table
 */
int hash_lp_stuffed(HashLP);


/**
 * Destroy the hash table
 */
void hash_lp_destroy(HashLP);


/**
 * Insert given data in the table managing collisions with separate chaning
 */
void hash_lp_insert(HashLP, void*);


/**
 * Search given data on the hash table, return data if its found, NULL otherwise
 */
void* hash_lp_search(HashLP, void*);


/**
 * Delete given data from the hash table if exist on it
 */
void hash_lp_delete(HashLP, void*);


/**
 * Resize the hash table at double of its capacity and rehash each of its elements
 */
void hash_lp_rehash(HashLP);


#endif