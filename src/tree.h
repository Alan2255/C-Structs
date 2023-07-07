#ifndef __TREE_H__
#define __TREE_H__

#include <stdlib.h>
#include "void.h"
#include "sugar.h"

/**
 * Types of tree:
 * 
 * Binary tree
 * 
 * Binary search tree (to implement)
 * 
 * AVL tree (to implement)
 * 
 * Tree (to implement)
*/


/**
 * Binary tree
*/
typedef struct _BTNode {

    void* data;
    struct _BTNode* left;
    struct _BTNode* right;

} *BTree;


/**
 * Order to travel through a binary tree
*/
typedef enum {

  IN,  /* Inorder */
  PRE, /* Preorder */
  POST /* Postorder */

} BTreeOrder;


/**
 * Create an empty tree
*/
BTree btree_create();


/**
 * Make a tree with data, left and right subtrees
*/
BTree btree_union(void*, BTree, BTree, FunctionCopy);


/**
 * Destroy the tree
*/
void btree_destroy(BTree, FunctionDestroy);


/**
 * Print the tree in some order
*/
void btree_travel(BTree, BTreeOrder, FunctionVisit);


/**
 * Return the height of the binary tree
*/
int btree_height(BTree tree);


/**
 * Binary search tree
*/
typedef struct _BSTree {

  BTree root;
  FunctionCopy copy;
  FunctionDestroy destroy;
  FunctionCompare compare;

} *BSTree;


/**
 * Create an empty binary search tree
*/
BSTree bstree_create(FunctionCopy, FunctionDestroy, FunctionCompare);


/**
 * Insert data in the binary search tree
*/
void bstree_insert(BSTree, void*);


/**
 * Destroy the binary search tree
*/
void bstree_destroy(BSTree);


/**
 * Search data in the binary search tree, return true if finds it and false otherwise
*/
int bstree_search(BSTree, void*);


/**
 * Delete data from the binary search tree if exist in it
*/
void bstree_delete(BSTree, void*);


/**
 * AVL tree
*/
typedef struct _ANode {

    void* data;
    int height;
    struct _ANode* left;
    struct _ANode* right;

} *AVLTree;



#endif