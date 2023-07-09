#include "list.h"


/**
 * Type of lists:
 * 
 * Linked list
 * 
 * Doubly linked list
 *
 * Circular linked list (to implement)
 * 
 * Doubly circular linked list (to implement)
*/


/**
 * Linked list
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
 * Return the length of the list
*/
int list_length(List list) {

    if (not list) return 0;

    return 1 + list_length(list->next);
}


/**
 * Concatenates two lists
 * Modify the first list
 * (?)
*/
List list_concatenate(List list1, List list2, FunctionCopy copy) {

    if (not list2) return list1;

    list1 = list_add(list1, list2->data, copy);
    
    return list_concatenate(list1, list2->next, copy);
}


/**
 * Insert data at some given index in the list
*/
List list_insert(List list, void* data, int index, FunctionCopy copy) {

    // If there is no list or we want to insert at the begin
    if (not list or index == 0)
        return list_add(list, data, copy);

    // Otherwise we want to move through the list till the index (or the end)
    List node = list;
    for (int i = 0; i < index-1 and node->next != NULL; i++, node = node->next);

    // Insert data
    node->next = list_add(node->next, data, copy);

    return list;
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
 * Search data in the list, return true if its in, false otherwise
*/
int list_search_bool(List list, void* data, FunctionCompare compare) {

    if (not list) return false;

    if (compare(list->data, data) == 0) return true;
    
    else return list_search_bool(list->next, data, compare);
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
 * Doubly linked list
*/

/**
 * Create an empty doubly list
*/
DList dlist_create() {

    DList newList = malloc(sizeof(struct _DList));
    
    newList->begin = NULL;
    newList->end = NULL;

    return newList;
}


/**
 * Add data to the doubly list
*/
DList dlist_add(DList list, void* data, DoublyOrder order, FunctionCopy copy) {

    // Create a new node
    DNode newNode = malloc(sizeof(struct _DNode));
    newNode->data = copy(data);
    newNode->next = NULL;
    newNode->prev = NULL;

    // If list its empty
    if (not list->begin) {

        list->begin = newNode;
        list->end = newNode;
    }

    // Add in the end
    else if (order == FORDWARD) {
 
        list->end->next = newNode;
        newNode->prev = list->end;
        list->end = newNode;        
    }

    // Add in the begin
    else if (order == BACKWARD) {

        list->begin->prev = newNode;
        newNode->next = list->begin;
        list->begin = newNode;
    }

    return list;
}


/**
 * Destroy the doubly list
*/
void dlist_destroy_aux(DNode node, FunctionDestroy destroy) {

    if (not node) return;

    dlist_destroy_aux(node->next, destroy);

    destroy(node->data);
    free(node);
}


/**
 * Destroy the doubly list
*/
void dlist_destroy(DList list, FunctionDestroy destroy) {

    if (not list) return;

    dlist_destroy_aux(list->begin, destroy);
    free(list);
}


/**
 * Print the list
*/
void dlist_print_aux(DNode node, DoublyOrder order, FunctionVisit visit) {

    if (not node) return;

    visit(node->data);

    // Print in the given order
    if (order == FORDWARD)
        dlist_print_aux(node->next, order, visit);
    else 
        dlist_print_aux(node->prev, order, visit);
}


/**
 * Print the doubly list
*/
void dlist_print(DList list, DoublyOrder order, FunctionVisit visit) {

    if (not list) return;

    // Print fordward
    if (order == FORDWARD) {

        dlist_print_aux(list->begin, order, visit);
    }

    // Print backward
    else if (order == BACKWARD) {

        dlist_print_aux(list->end, order, visit);
    }
}


/**
 * Return the length of the dlist
*/
int dlist_length_aux(DNode node) {

    if (not node) return 0;

    return 1 + list_length(node->next);
}


/**
 * Return the length of the dlist
*/
int dlist_length(DList list) {

    if (not list) return 0;

    return dlist_length_aux(list->begin);
}