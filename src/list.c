#include "list.h"


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
 * Insert data at some given index in the list
*/
List list_insert(List list, void* data, int index, FunctionCopy copy) {

    if (index < 0) return list;
    if (index == 0) return list_add(list, data, copy);

    List node = list;
    for (int i = 0; i < index-1 and node->next != NULL; i++, node = node->next);

    if (node->next exist) {

        node->next = list_add(node->next, data, copy);
    }

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
DList dlist_add(DList list, void* data, ListOrder order, FunctionCopy copy) {

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
void dlist_print_aux(DNode node, ListOrder order, FunctionVisit visit) {

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
void dlist_print(DList list, ListOrder order, FunctionVisit visit) {

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
 * Return the length of the doubly list
*/
int dlist_length_aux(DNode node) {

    if (not node) return 0;

    return 1 + dlist_length_aux(node->next);
}


/**
 * Return the length of the dobuly list
*/
int dlist_length(DList list) {

    if (not list) return 0;

    return dlist_length_aux(list->begin);
}


/**
 * Circular linked list
*/

/**
 * Create an empty circular linked list
*/
CList clist_create() {

    CList newList = malloc(sizeof(struct _CList));

    newList->begin = NULL;
    newList->end = NULL;

    return newList;
}

/**
 * Add data to the circular list
*/
CList clist_add(CList list, void* data, ListOrder order, FunctionCopy copy) {

    // Create a new node
    List newNode = malloc(sizeof(struct _LNode));
    newNode->data = copy(data);
    newNode->next = NULL;

    // If list its empty
    if (not list->begin) {

        list->begin = newNode;
        list->end = newNode;
    }

    // Add in the end
    else if (order == FORDWARD) {
 
        list->end->next = newNode;
        list->end = newNode;        
    }

    // Add in the begin
    else if (order == BACKWARD) {

        newNode->next = list->begin;
        list->begin = newNode;
    }

    return list;
}


/**
 * Destroy the circular linked list
*/
void clist_destroy(CList list, FunctionDestroy destroy) {

    if (not list) return;

    if (list->begin exist) {
    
        List start = list->begin, nodeDelete;
        for (; start != list->end ;) {

            nodeDelete = start;
            start = start->next;
            destroy(nodeDelete->data);
            free(nodeDelete);
        }

        if (list->end exist) {

            destroy(list->end->data);
            free(list->end);
        }

    }

    free(list);
}


/**
 * Print the circular linked list
*/
void clist_print(CList list, FunctionVisit visit) {

    if (not list) return;

    List start = list->begin;
    for (; start != list->end; start = start->next) {

        visit(start->data);
    }

    if (list->end exist)
        visit(list->end->data);
}


/**
 * Return the length of the circular linked
*/
int clist_length(CList list) {

    if (not list) return 0;

    int length = 1;
    List start = list->begin;
    for (; start != list->end; start = start->next, length++);
    
    return length;
}


/**
 * Doubly circular linked list
*/

/**
 * Create an empty doubly circular list
*/
DCList dclist_create() { return NULL; }


/**
 * Add data in some order to the doubly circular list
*/
DCList dclist_add(DCList list, void* data, ListOrder order, FunctionCopy copy) {

    DCList newNode = malloc(sizeof(struct _DNode));

    newNode->data = copy(data);
    
    if (list exist) {
        
        newNode->next = list;
        newNode->prev = list->prev;
        list->prev->next = newNode;
        list->prev = newNode;
        
        if (order == BACKWARD) {
        
           list = list->prev; 
        }
        
        return list;
    }
    
    else {
    
        newNode->next = newNode;
        newNode->prev = newNode;
        
        return newNode;
    }
}


/**
 * Delete data in some order from the doubly circular list
*/
DCList dclist_delete(DCList list, ListOrder order, FunctionDestroy destroy) {

    if (not list) return NULL;

    // It has only one element
    if (list->prev == list) {

        // Delete data
        destroy(list->data);
        free(list);

        return NULL;
    }

    // More than one element
    else {

        // If delete from the end
        if (order == BACKWARD) {
         
            // Move to the new first
            list = list->next;
        }

        // Auxiliar node to delete
        DNode node = list->prev;
        
        // The new last links to the first
        list->prev->prev->next = list;

        // The first links to the new last
        list->prev = list->prev->prev;

        // Delete data
        destroy(node->data);
        free(node);
    }

    return list;
}


/**
 * Print the doubly circular list
*/
void dclist_print(DCList list, ListOrder order, FunctionVisit visit) {

    if (not list) return;
    
    DNode stop;
    
    // Print fordward
    if (order == FORDWARD) {
    
        stop = list->prev;
        
        for (;list != stop; list = list->next) {
        
            visit(list->data);
        }
        
        visit(list->data);
    }
    
    // Print backward
    else if (order == BACKWARD) {
    
        stop = list;
        list = list->prev;
        
        for (;list != stop; list = list->prev) {
        
            visit(list->data);
        }
        
        visit(list->data);
    }
}


/**
 * Destroy the doubly circular list
*/
void dclist_destroy(DCList list, FunctionDestroy destroy) {

    if (not list) return;
    
    DNode stop = list->prev, node;
        
    for (;list != stop;) {

        // Auxiliar node to destroy the past element
        node = list;
        list = list->next;
        
        destroy(node->data);
        free(node); 
    }
    
    destroy(list->data);
    free(list);
}


/**
 * Return the length of the doubly circular list
*/
int dclist_length(DCList list) {

    if (not list) return 0;

    int length = 1;
    DNode stop = list->prev;
    for (; list != stop; list = list->next, length++);
    
    return length;

}