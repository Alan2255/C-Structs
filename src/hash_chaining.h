#ifndef __HASH_CHAINING_H__
#define __HASH_CHAINING_H__

#include <stdlib.h>
#include <stdio.h>
#include "void.h"
#include "sugar.h"


/**
 * Hash table
 * 
 * Open hashing: separate chaining
*/


/**
 * Built in linked list
*/
typedef struct _HNode {

    void* data;
    struct _HNode *next;

} *HList;


/**
 * Struct of each cell of the hash table
*/
typedef HList Cell;


/**
 * Struct of the hash table
*/
typedef struct _Hash {

    Cell *array;

    int capacity;
    int stuffed;
    
    FunctionCopy copy;
    FunctionCompare compare;
    FunctionDestroy destroy;
    FunctionVisit visit;
    FunctionHash hash;

} *Hash;


/**
 * Overload charge factor to decide when to rehash
*/
#define OVERLOAD_CHARGE_FACTOR 0.75


/**
 * Create an empty hash table
*/
Hash hash_create(int capacity, FunctionCopy copy, FunctionDestroy destroy, FunctionCompare compare, FunctionVisit visit, FunctionHash hash);


/**
 * Destroy the hash table
*/
void hash_destroy(Hash table);


/**
 * Return the capacity of the hash table
 */
int hash_capacity(Hash table);


/**
 * Return the amount of stuffed cells in the hash table
 */
int hash_stuffed(Hash table);


/**
 * Search given data in the hash table
*/
void* hash_search(Hash table, void* data);


/**
 * Add given data to the hash table
*/
void hash_add(Hash table, void* data);


/**
 * Delete given data from the hash table
*/
void hash_delete(Hash table, void* data);


/**
 * Resize the hash table at double of its capacity and rehash each of its elements
*/
void hash_rehash(Hash table);


/**
 * Print the hash table
*/
void hash_print(Hash);


#endif