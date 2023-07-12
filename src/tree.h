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
typedef struct _BSTree {

  BTree root;
  
  FunctionCopy copy;
  FunctionDestroy destroy;
  FunctionCompare compare;
  FunctionVisit visit;

} *BSTree;


/**
 * Create an empty binary search tree
*/
BSTree bstree_create(FunctionCopy, FunctionDestroy, FunctionCompare, FunctionVisit);


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
 * Travel the binary search tree in some order
*/
void bstree_travel(BSTree, BTreeOrder);


/**
 * AVL node
*/
typedef struct _ANode {

  void* data;
  int height;
  struct _ANode* left;
  struct _ANode* right;

} *ATree;


/**
 * AVL tree
*/
typedef struct _AVLTree {

  ATree root;
  FunctionCopy copy;
  FunctionDestroy destroy;
  FunctionCompare compare;
  FunctionVisit visit;

} *AVLTree;


/**
 * Create an empty AVL search tree
*/
AVLTree avl_create(FunctionCopy, FunctionDestroy, FunctionCompare, FunctionVisit);


/**
 * Insert data in the AVL tree
*/
void avl_insert(AVLTree, void*);


/**
 * Destroy the AVL tree
*/
void avl_destroy(AVLTree);


/**
 * Search data in the AVL tree, return true if finds it and false otherwise
*/
int avl_search_bool(AVLTree, void*);


/**
 * Delete data from the AVL tree if exist in it
*/
void avl_delete(AVLTree, void*);


/**
 * Print the avl tree
*/
void avl_travel(AVLTree, BTreeOrder);


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