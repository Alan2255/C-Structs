#ifndef __HASH_PROBING_H__
#define __HASH_PROBING_H__

#include <stdlib.h>
#include <stdio.h>
#include "void.h"
#include "sugar.h"


/**
 * Hash table
 * 
 * Closed hashing: probing
*/


/**
 * Type of probing
*/
typedef enum {

    LINEAR,
    CUADRATIC,
    DOUBLE_HASHING,
    // RANDOM,

} ProbingType;


/**
 * Struct of each cell of the hash table
*/
typedef struct _Cell {

    void* data;
    int deleted;

} *Cell;


/**
 * Struct of the hash table
*/
typedef struct _Hash {

    Cell *array;

    int capacity;
    int stuffed;

    ProbingType type;

    FunctionCopy copy;
    FunctionDestroy destroy;
    FunctionCompare compare;
    FunctionVisit visit;
    FunctionHash hash;

} *Hash;


/**
 * Overload charge factor to decide when to rehash
*/
#define OVERLOAD_CHARGE_FACTOR 0.75


/**
 * Constant primes to probing
*/
#define C1 7
#define C2 19

// For double hashing, may take a prime lower than the table size
// Also may be co-prime with the table size
#define CO_PRIME 31


/**
 * Create an empty hash table
*/
Hash hash_create(int, ProbingType, FunctionCopy, FunctionDestroy, FunctionCompare, FunctionVisit, FunctionHash);


/**
 * Destroy the hash table
*/
void hash_destroy(Hash);


/**
 * Return the capacity of the hash table
 */
int hash_capacity(Hash);


/**
 * Return the amount of stuffed cells in the hash table
 */
int hash_stuffed(Hash);


/**
 * Search given data in the hash table
*/
void* hash_search(Hash, void*);


/**
 * Add given data to the hash table
*/
void hash_add(Hash, void*);


/**
 * Delete given data from the hash table
*/
void hash_delete(Hash, void*);


/**
 * Resize the hash table at double of its capacity and rehash each of its elements
*/
void hash_rehash(Hash);


/**
 * Print the hash table
*/
void hash_print(Hash);


#endif