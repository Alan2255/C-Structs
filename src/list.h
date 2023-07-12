#ifndef __LIST_H__
#define __LIST_H__

#include <stdlib.h>
#include "void.h"
#include "sugar.h"

/**
 * Type of lists:
 * 
 * Linked list
 * 
 * Doubly linked list
 *
 * Circular linked list
 * 
 * Doubly circular linked list
*/


/**
 * Linked list
*/
typedef struct _LNode {

    void* data;
    struct _LNode *next;

} *List;


/**
 * Create an empty linked list
*/
List list_create();


/**
 * Add data to the begin of the list
*/
List list_add(List, void*, FunctionCopy);


/**
 * Destroy the list
*/
void list_destroy(List, FunctionDestroy);


/**
 * Print the list
*/
void list_print(List, FunctionVisit);


/**
 * Return the length of the list
*/
int list_length(List);


/**
 * Insert data at some given index in the list
*/
List list_insert(List, void*, int, FunctionCopy);


/**
 * Delete data at some given index in the list
*/
List list_delete(List, int, FunctionDestroy);


/**
 * Search data in the list, return true if its in, false otherwise
*/
int list_search_bool(List, void*, FunctionCompare);


/**
 * Search data in the list, return data (pointer on list) if its in, NULL otherwise
*/
void* list_search_data(List, void*, FunctionCompare);


/**
 * Search data in the list, return the node of data if its on it, NULL otherwise
*/
List list_search_node(List, void*, FunctionCompare);


/**
 * Doubly linked list
*/

typedef enum {

    FORDWARD,
    BACKWARD,

} ListOrder;


typedef struct _DNode {
    
    void* data;
    struct _DNode* prev;
    struct _DNode* next;

} *DNode;


typedef struct _DList {

    DNode begin;
    DNode end;    

} *DList;


/**
 * Create an empty doubly list
*/
DList dlist_create();


/**
 * Add data to the begin of the doubly list
*/
DList dlist_add(DList, void*, ListOrder, FunctionCopy);


/**
 * Destroy the doubly list
*/
void dlist_destroy(DList, FunctionDestroy);


/**
 * Print the doubly list
*/
void dlist_print(DList, ListOrder, FunctionVisit);


/**
 * Return the length of the dlist
*/
int dlist_length(DList);


/**
 * Circular linked list
*/
typedef struct _CList {

    List begin;
    List end;

} *CList;


/**
 * Create an empty circular list
*/
CList clist_create();


/**
 * Add data to the circular list
*/
CList clist_add(CList, void*, ListOrder, FunctionCopy);


/**
 * Delete data from the circular list
*/
void clist_delete(CList, ListOrder, FunctionDestroy);


/**
 * Destroy the circular list
*/
void clist_destroy(CList, FunctionDestroy);


/**
 * Print the circular list
*/
void clist_print(CList, FunctionVisit);


/**
 * Return the length of the circular list
*/
int clist_length(CList);


/**
 * Doubly circular linked list
*/
typedef DNode DCList;


/**
 * Create an empty doubly circular list
*/
DCList dclist_create();


/**
 * Add data in some order to the doubly circular list
*/
DCList dclist_add(DCList, void*, ListOrder, FunctionCopy);


/**
 * Delete data in some order from the doubly circular list
*/
DCList dclist_delete(DCList, ListOrder, FunctionDestroy);


/**
 * Print the doubly circular list
*/
void dclist_print(DCList, ListOrder, FunctionVisit);


/**
 * Destroy the doubly circular list
*/
void dclist_destroy(DCList, FunctionDestroy);


/**
 * Return the length of the doubly circular list
*/
int dclist_length(DCList);


#endif