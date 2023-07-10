#include "tree.h"


/**
 * Types of tree:
 * 
 * Binary tree
 * 
 * Binary search tree
 * 
 * AVL tree
 * 
 * Tree
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
        else if (tree->right exist and not tree->left) {

            // Node to delete data and relink the right subtree
            BTree node = tree;
            tree = tree->right;
            destroy(node->data);
            free(node);

            return tree;
        }

        // If only have a left subtree
        else if (tree->left exist and not tree->right) {

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


/**
 * AVL Tree
*/

/**
 * Return the node which is the previous one to the maximun of 
 * the minimuns from the given node
*/
ATree avl_previous_max_of_min(ATree tree) {

    if (not tree) return NULL;
    if(not tree->left) return NULL;

    // From the minimun
    tree = tree->left;

    // The maximun
    for(; tree->right != NULL and tree->right->right != NULL; tree = tree->right);

    return tree;
}


/**
 * Return the height of the AVL tree
*/
int avl_height(ATree tree) {

    if (not tree) return 0;

    int left = 1 + avl_height(tree->left);
    int right = 1 + avl_height(tree->right);

    return left > right ? left : right;
}


/**
 * Balance factor of the AVL tree
*/
int avl_balance_factor(ATree tree) {
    
    if (not tree) return 0;

    return avl_height(tree->left) - avl_height(tree->right);
}


/**
 * Rotation to the left
*/
ATree avl_rotation_left(ATree tree) {

    if (not tree) return  NULL;
    if (not tree->right) return tree;

    ATree rightSubtree = tree->right;
    tree->right = rightSubtree->left;
    rightSubtree->left = tree;

    rightSubtree->height = avl_height(rightSubtree);
    tree->height = avl_height(tree);

    return rightSubtree;
}


/**
 * Rotation to the right
*/
ATree avl_rotation_right(ATree tree) {

    if (not tree) return  NULL;
    if (not tree->left) return tree;

    ATree leftSubtree = tree->left;
    tree->left = leftSubtree->right;
    leftSubtree->right = tree;

    leftSubtree->height = avl_height(leftSubtree);
    tree->height = avl_height(tree);

    return leftSubtree;
}


/**
 * Check if the AVL tree is balanced, return true if it is, false otherwise
*/
int avl_check(ATree tree) {

    if (not tree) return true;
    
    // If its disbalanced
    if (abs(avl_balance_factor(tree)) > 1) return false;

    // Check both subtrees
    return avl_check(tree->left) and avl_check(tree->left);
}


/**
 * Fix the AVL tree in order to make it balanced
*/
ATree avl_fix(ATree tree) {

    if (not tree) return NULL;


    // Need to analize the left subtree
    if (avl_height(tree->left) > avl_height(tree->right)) {

        // Fix outside left
        if (avl_height(tree->left->left) > avl_height(tree->left->right)) {

            // Simple rotation
            tree = avl_rotation_right(tree);
        }

        // Fix inside left
        else {

            // Double rotation
            tree->left = avl_rotation_left(tree->left);
            tree = avl_rotation_right(tree);
        }
    }

    // Need to analize the right subtree
    else {

        // Fix outside right
        if (avl_height(tree->right->right) > avl_height(tree->right->left)) {

            // Simple rotation
            tree = avl_rotation_left(tree);
        }

        // Fix inside right
        else {

            // Double rotation
            tree->right = avl_rotation_right(tree->right);
            tree = avl_rotation_left(tree);
        }
    }

    return tree;
}


/**
 * Create an empty AVL search tree
*/
AVLTree avl_create(FunctionCopy copy, FunctionDestroy destroy, FunctionCompare compare) {

    AVLTree tree = malloc(sizeof(struct _AVLTree));

    tree->root = NULL;
    tree->copy = copy;
    tree->compare = compare;
    tree->destroy = destroy;

    return tree;
}


/**
 * Insert data in the AVL tree
*/
ATree avl_insert_aux(ATree tree, void* data, FunctionCopy copy, FunctionCompare compare) {

    if (not tree) {

        // Create new node in the tree
        ATree newNode = malloc(sizeof(struct _ANode));
        
        newNode->data = copy(data);
        newNode->height = 1;
        newNode->left = NULL;
        newNode->right = NULL;
        
        return newNode;
    }

    // Check if needs to insert in the right subtree
    if (compare(data, tree->data) > 0) {

        // Try to insert in the right subtree
        tree->right = avl_insert_aux(tree->right, data, copy, compare);

        // Update height
        tree->height = avl_height(tree);

        // If its not balanced
        if (abs(avl_balance_factor(tree)) > 1) {

            // Fix with the proper rotation
            tree = avl_fix(tree);
        }

        return tree;
    }
    
    // Check if needs to insert in the left subtree
    else if (compare(data, tree->data) < 0) {

        // Try to insert in the left subtree
        tree->left = avl_insert_aux(tree->left, data, copy, compare);

        // Update height
        tree->height = avl_height(tree);

        // If its not balanced
        if (abs(avl_balance_factor(tree)) > 1) {

            // Fix with the proper rotation
            tree = avl_fix(tree);
        }

        return tree;
    }

    // If data already exist in the tree, do nothing
    return tree;
}


/**
 * Insert data in the AVL tree
*/
void avl_insert(AVLTree tree, void* data) {

    if (not tree) return;

    // Insert data
    tree->root = avl_insert_aux(tree->root, data, tree->copy, tree->compare);
}


/**
 * Destroy the AVL tree
*/
void avl_destroy_aux(ATree tree, FunctionDestroy destroy) {

    if (not tree) return;

    avl_destroy_aux(tree->left, destroy);
    avl_destroy_aux(tree->right, destroy);

    destroy(tree->data);
    free(tree);
}


/**
 * Destroy the AVL tree
*/
void avl_destroy(AVLTree tree) {

    if (not tree) return;

    avl_destroy_aux(tree->root, tree->destroy);
    free(tree);
}


/**
 * Search data in the AVL tree, return true if finds it and false otherwise
*/
int avl_search_bool_aux(ATree tree, void* data, FunctionCompare compare) {

    if (not tree) return false;

    // Check if needs to search in the right subtree
    if (compare(data, tree->data) > 0) {

        // Search in the right subtree
        return avl_search_bool_aux(tree->right, data, compare);
    }

    // Check if needs to search in the left subtree
    else if (compare(data, tree->data) < 0) {

        // Search in the left subtree
        return avl_search_bool_aux(tree->left, data, compare);
    }

    // Data found
    else {

        return true;
    }
}


/**
 * Search data in the AVL tree, return true if finds it and false otherwise
*/
int avl_search_bool(AVLTree tree, void* data) {

    if (not tree) return false;

    return avl_search_bool_aux(tree->root, data, tree->compare);
}

/**
 * Delete data from the AVL tree if exist in it
*/
ATree avl_delete_aux(ATree tree, void* data, FunctionCompare compare, FunctionDestroy destroy) {

    if (not tree) return NULL;

    // Check if needs to delete in the right subtree
    if (compare(data, tree->data) > 0) {

        // Try to delete in the right subtree
        tree->right = avl_delete_aux(tree->right, data, compare, destroy);

        // Update height
        tree->height = avl_height(tree);

        // If its not balanced
        if (abs(avl_balance_factor(tree)) > 1) {

            // Fix with the proper rotation
            tree = avl_fix(tree);
        }

        return tree;
    }

    // Check if needs to delete in the left subtree
    if (compare(data, tree->data) < 0) {

        // Try to delete in the left subtree
        tree->left = avl_delete_aux(tree->left, data, compare, destroy);

        // Update height
        tree->height = avl_height(tree);

        // If its not balanced
        if (abs(avl_balance_factor(tree)) > 1) {

            // Fix with the proper rotation
            tree = avl_fix(tree);
        }

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
        else if (tree->right exist and not tree->left) {

            // Node to delete data and relink the right subtree
            ATree node = tree;
            tree = tree->right;
            destroy(node->data);
            free(node);

            return tree;
        }

        // If only have a left subtree
        else if (tree->left exist and not tree->right) {

            // Node to delete data and relink the left subtree
            ATree node = tree;
            tree = tree->left;
            destroy(node->data);
            free(node);

            return tree;
        }

        // Has both subtrees
        else {
            
            destroy(tree->data);
            ATree previousMaxMin = avl_previous_max_of_min(tree);

            if(not previousMaxMin->right) {
                
                tree->data = previousMaxMin->data;
                tree->left = previousMaxMin->left;
                free(previousMaxMin);
            }

            else {

                // Node to delete data and relink
                ATree node = previousMaxMin->right;
                tree->data = node->data;
                previousMaxMin->right = previousMaxMin->right->left;
                free(node);
            }

            return tree;
        }
    }
}


/**
 * Delete data from the AVL tree if exist in it
*/
void avl_delete(AVLTree tree, void* data) {

    if (not tree) return;

    // Delete data
    tree->root = avl_delete_aux(tree->root, data, tree->compare, tree->destroy);
}


/**
 * Print the AVL tree
*/
void avl_travel_aux(ATree tree, BTreeOrder order, FunctionVisit visit) {

    if (not tree) return;

    // Preorder
    if (order == PRE) 
        visit(tree->data);

    avl_travel_aux(tree->left, order, visit);
    
    // Inorder
    if (order == IN) 
        visit(tree->data);
    
    avl_travel_aux(tree->right, order, visit);
    
    // Postorder
    if (order == POST) 
        visit(tree->data);
}


/**
 * Travel through the AVL tree in some order
*/
void avl_travel(AVLTree tree, BTreeOrder order, FunctionVisit visit) {

    if (not tree) return;

    avl_travel_aux(tree->root, order, visit);
}