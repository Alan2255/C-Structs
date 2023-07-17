#ifndef __TREE_H__
#define __TREE_H__

#include <stdlib.h>
#include <stdio.h>
#include "void.h"
#include "sugar.h"

/**
 * Types of tree:
 * 
 * Binary tree
 * 
 * Binary search tree
 * 
 * AVL tree
 * 
 * General tree
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
typedef struct _BST {

  BTree root;
  
  FunctionCopy copy;
  FunctionDestroy destroy;
  FunctionCompare compare;
  FunctionVisit visit;

} *BST;


/**
 * Create an empty binary search tree
*/
BST bst_create(FunctionCopy, FunctionDestroy, FunctionCompare, FunctionVisit);


/**
 * Insert data in the binary search tree
*/
void bst_add(BST, void*);


/**
 * Destroy the binary search tree
*/
void bst_destroy(BST);


/**
 * Search data in the binary search tree, return true if finds it and false otherwise
*/
int bst_search(BST, void*);


/**
 * Delete data from the binary search tree if exist in it
*/
void bst_delete(BST, void*);


/**
 * Travel the binary search tree in some order
*/
void bst_travel(BST, BTreeOrder);


/**
 * AVL node
*/
typedef struct _ATree {

  void* data;
  int height;
  struct _ATree* left;
  struct _ATree* right;

} *ATree;


/**
 * AVL tree
*/
typedef struct _AVL {

  ATree root;
  FunctionCopy copy;
  FunctionDestroy destroy;
  FunctionCompare compare;
  FunctionVisit visit;

} *AVL;


/**
 * Create an empty AVL search tree
*/
AVL avl_create(FunctionCopy, FunctionDestroy, FunctionCompare, FunctionVisit);


/**
 * Insert data in the AVL tree
*/
void avl_add(AVL, void*);


/**
 * Destroy the AVL tree
*/
void avl_destroy(AVL);


/**
 * Search data in the AVL tree, return true if finds it and false otherwise
*/
int avl_search_bool(AVL, void*);


/**
 * Delete data from the AVL tree if exist in it
*/
void avl_delete(AVL, void*);


/**
 * Print the avl tree
*/
void avl_travel(AVL, BTreeOrder);


/**
 * General node
*/
typedef struct _GNode {

  void* data;
  struct _GNode* child;
  struct _GNode* brother;

} *GNode;


/**
 * General tree
*/
typedef struct _GTree {

  GNode root;
  FunctionCopy copy;
  FunctionDestroy destroy;
  FunctionCompare compare;
  FunctionVisit visit;

} *GTree;


/**
 * Create an empty general tree
*/
GTree gtree_create(FunctionCopy, FunctionDestroy, FunctionCompare, FunctionVisit);


/**
 * Destroy the general tree
*/
void gtree_destroy(GTree);


/**
 * Print the general tree
 * 
 * Like this:
 * father - childs
*/
void gtree_print(GTree);


#endif