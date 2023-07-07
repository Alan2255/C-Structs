#include "tree.h"


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

/**
 * Create an empty binary tree
*/
BTree btree_create() { return NULL; }


/**
 * Make a binary tree with data, left and right subtrees
*/
BTree btree_union(void* data, BTree left, BTree right, FunctionCopy copy) {

    BTree newNode = malloc(sizeof(struct _BTNode));

    newNode->data = copy(data);
    newNode->left = left;
    newNode->right = right;

    return newNode;
}


/**
 * Destroy the binary tree
*/
void btree_destroy(BTree tree, FunctionDestroy destroy) {

    if (not tree) return;

    btree_destroy(tree->left, destroy);
    btree_destroy(tree->right, destroy);
    
    destroy(tree->data);
    free(tree);
}


/**
 * Travel through the binary tree in some order
*/
void btree_travel(BTree tree, BTreeOrder order, FunctionVisit visit) {

    if (not tree) return;

    // Preorder
    if (order == PRE) 
        visit(tree->data);

    btree_travel(tree->left, order, visit);
    
    // Inorder
    if (order == IN) 
        visit(tree->data);
    
    btree_travel(tree->right, order, visit);
    
    // Postorder
    if (order == POST) 
        visit(tree->data);
}


/**
 * Return the height of the binary tree
*/
int btree_height(BTree tree) {

    if (not tree) return -1;

    int leftHeight = 1 + btree_height(tree->left);
    int rightHeight = 1 + btree_height(tree->right);

    return leftHeight > rightHeight ? leftHeight : rightHeight;
}


/**
 * Binary search tree
*/

/**
 * Create an empty binary search tree
*/
BSTree bstree_create(FunctionCopy copy, FunctionDestroy destroy, FunctionCompare compare) {

    BSTree newTree = malloc(sizeof(struct _BSTree));

    newTree->root = NULL;
    newTree->copy = copy;
    newTree->destroy = destroy;
    newTree->compare = compare;

    return newTree;
}


/**
 * Return a binary search tree after insert data
*/
BTree bstree_insert_aux(BTree tree, void* data, FunctionCopy copy, FunctionCompare compare) {

    if (not tree) {

        return btree_union(data, NULL, NULL, copy);
    }

    // Check if needs to insert in the right subtree
    if (compare(data, tree->data) > 0) {

        tree->right = bstree_insert_aux(tree->right, data, copy, compare);
        return tree;
    }
    
    // Check if needs to insert in the left subtree
    else if (compare(data, tree->data) < 0) {

        tree->left = bstree_insert_aux(tree->left, data, copy, compare);
        return tree;
    }

    // Otherwise data is equal to the root
    // If data already exist in the tree, do nothing
    return tree;
}


/**
 * Insert data in the binary search tree
*/
void bstree_insert(BSTree tree, void* data) {

    if (not tree) return;

    tree->root = bstree_insert_aux(tree->root, data, tree->copy, tree->compare);
}


/**
 * Destroy the binary search tree
*/
void bstree_destroy(BSTree tree) {

    if (not tree) return;

    btree_destroy(tree->root, tree->destroy);
    free(tree);
}


/**
 * Search data in the binary search tree, return true if finds it and false otherwise
*/
int bstree_search_aux(BTree tree, void* data, FunctionCompare compare) {

    if (not tree) return false;

    // If data is greater than root
    if (compare(data, tree->data) > 0) {

        // Only search in the right subtree
        return bstree_search_aux(tree->right, data, compare);
    }

    // If data is lower than root
    else if (compare(data, tree->data) < 0) {

        // Only search in the right subtree
        return bstree_search_aux(tree->left, data, compare);
    }

    // Otherwise data is equal to the root
    else {

        return true;
    }
}


/**
 * Search data in the binary search tree, return true if finds it and false otherwise
*/
int bstree_search(BSTree tree, void* data) {

    if (not tree) return false;

    return bstree_search_aux(tree->root, data, tree->compare);
}


/**
 * Return the node which is the previous one to the maximun of 
 * the minimuns from the given node
*/
BTree previous_max_of_min(BTree tree) {

    if (not tree) return NULL;
    if(not tree->left) return NULL;

    // From the minimun
    tree = tree->left;

    // The maximun
    for(; tree->right != NULL and tree->right->right != NULL; tree = tree->right);

    return tree;
}


/**
 * Delete data from the binary search tree if exist on it
*/
BTree bstree_delete_aux(BTree tree, void* data, FunctionCompare compare, FunctionDestroy destroy) {

    if (not tree) return NULL;

    // If its in the right subtree
    if (compare(data, tree->data) > 0) {

        tree->right = bstree_delete_aux(tree->right, data, compare, destroy);
        return tree;
    }

    // If its in the right subtree
    if (compare(data, tree->data) < 0) {

        tree->left = bstree_delete_aux(tree->left, data, compare, destroy);
        return tree;
    }

    // Data found
    else {

        // If it has no subtrees
        if(not tree->left and not tree->right) {

            destroy(tree->data);
            free(tree);
            return NULL;
        }

        // If only have a right subtree
        else if (tree->right) {

            // Node to delete data and relink the right subtree
            BTree node = tree;
            tree = tree->right;
            destroy(node->data);
            free(node);

            return tree;
        }

        // If only have a left subtree
        else if (tree->left) {

            // Node to delete data and relink the left subtree
            BTree node = tree;
            tree = tree->left;
            destroy(node->data);
            free(node);

            return tree;
        }

        // Has both subtrees
        else {
            
            destroy(tree->data);
            BTree previousMaxMin = previous_max_of_min(tree);

            if(not previousMaxMin->right) {
                
                tree->data = previousMaxMin->data;
                tree->left = previousMaxMin->left;
                free(previousMaxMin);
            }

            else {

                // Node to delete data and relink
                BTree node = previousMaxMin->right;
                tree->data = node->data;
                previousMaxMin->right = previousMaxMin->right->left;
                free(node);
            }

            return tree;
        }
    }
}


/**
 * Delete data from the binary search tree if exist on it
*/
void bstree_delete(BSTree tree, void* data) {

    if (not tree) return;

    tree->root = bstree_delete_aux(tree->root, data, tree->compare, tree->destroy);
}
