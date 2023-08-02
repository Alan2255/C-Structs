#include "tree.h"
#include "int.h"


GNode new(void* data) {

  GNode newNode = malloc(sizeof(struct _GNode));

  newNode->data = copy_int(data);
  newNode->child = NULL;
  newNode->brother = NULL;

  return newNode;
}



/**
 * I just found this code to print the tree (only for ints)
 * 
 * Source: https://stackoverflow.com/questions/801740/c-how-to-draw-a-binary-tree-to-the-console
*/

typedef struct asciinode_struct asciinode;

struct asciinode_struct
{
  asciinode * child, * brother;

  //length of the edge from this node to its children
  int edge_length; 
    
  int height;      

  int lablen;

  //-1=I am child, 0=I am tree, 1=brother   
  int parent_dir;   
                         
  //max supported unit32 in dec, 10 digits max
  char label[11];  
};


#define MAX_HEIGHT 1000
int lprofile[MAX_HEIGHT];
int rprofile[MAX_HEIGHT];
#define INFINITY (1<<20)

//adjust gap between child and brother nodes
int gap = 3;  

//used for printing next node in the same level, 
//this is the x coordinate of the next char printed
int print_next;    

int MIN (int X, int Y)  
{
  return ((X) < (Y)) ? (X) : (Y);
}

int MAX (int X, int Y)  
{
  return ((X) > (Y)) ? (X) : (Y);
}

asciinode * build_ascii_tree_recursive(GNode  t) 
{
  asciinode * node;
  
  if (t == NULL) return NULL;

  node = malloc(sizeof(asciinode));
  node->child = build_ascii_tree_recursive(t->child);
  node->brother = build_ascii_tree_recursive(t->brother);
  
  if (node->child != NULL) 
  {
    node->child->parent_dir = -1;
  }

  if (node->brother != NULL) 
  {
    node->brother->parent_dir = 1;
  }

  sprintf(node->label, "%d", *((int*)t->data));
  node->lablen = strlen(node->label);

  return node;
}


//Copy the tree into the ascii node structre
asciinode * build_ascii_tree(GNode  t) 
{
  asciinode *node;
  if (t == NULL) return NULL;
  node = build_ascii_tree_recursive(t);
  node->parent_dir = 0;
  return node;
}

//Free all the nodes of the given tree
void free_ascii_tree(asciinode *node) 
{
  if (node == NULL) return;
  free_ascii_tree(node->child);
  free_ascii_tree(node->brother);
  free(node);
}

//The following function fills in the lprofile array for the given tree.
//It assumes that the center of the label of the tree of this tree
//is located at a position (x,y).  It assumes that the edge_length
//fields have been computed for this tree.
void compute_lprofile(asciinode *node, int x, int y) 
{
  int i, ischild;
  if (node == NULL) return;
  ischild = (node->parent_dir == -1);
  lprofile[y] = MIN(lprofile[y], x-((node->lablen-ischild)/2));
  if (node->child != NULL) 
  {
	  for (i=1; i <= node->edge_length && y+i < MAX_HEIGHT; i++) 
    {
	    lprofile[y+i] = MIN(lprofile[y+i], x-i);
    }
  }
  compute_lprofile(node->child, x-node->edge_length-1, y+node->edge_length+1);
  compute_lprofile(node->brother, x+node->edge_length+1, y+node->edge_length+1);
}

void compute_rprofile(asciinode *node, int x, int y) 
{
  int i, notchild;
  if (node == NULL) return;
  notchild = (node->parent_dir != -1);
  rprofile[y] = MAX(rprofile[y], x+((node->lablen-notchild)/2));
  if (node->brother != NULL) 
  {
	  for (i=1; i <= node->edge_length && y+i < MAX_HEIGHT; i++) 
    {
	    rprofile[y+i] = MAX(rprofile[y+i], x+i);
    }
  }
  compute_rprofile(node->child, x-node->edge_length-1, y+node->edge_length+1);
  compute_rprofile(node->brother, x+node->edge_length+1, y+node->edge_length+1);
}

//This function fills in the edge_length and 
//height fields of the specified tree
void compute_edge_lengths(asciinode *node) 
{
  int h, hmin, i, delta;
  if (node == NULL) return;
  compute_edge_lengths(node->child);
  compute_edge_lengths(node->brother);

  /* first fill in the edge_length of node */
  if (node->brother == NULL && node->child == NULL) 
  {
	  node->edge_length = 0;
  } 
  else 
  {
    if (node->child != NULL) 
    {
	    for (i=0; i<node->child->height && i < MAX_HEIGHT; i++) 
      {
		    rprofile[i] = -INFINITY;
	    }
	    compute_rprofile(node->child, 0, 0);
	    hmin = node->child->height;
    } 
    else 
    {
	    hmin = 0;
    }
	  if (node->brother != NULL) 
    {
	    for (i=0; i<node->brother->height && i < MAX_HEIGHT; i++) 
      {
		    lprofile[i] = INFINITY;
	    }
	    compute_lprofile(node->brother, 0, 0);
	    hmin = MIN(node->brother->height, hmin);
    } 
    else 
    {
	    hmin = 0;
    }
	  delta = 4;
	  for (i=0; i<hmin; i++) 
    {
	    delta = MAX(delta, gap + 1 + rprofile[i] - lprofile[i]);
    }
	  
    //If the node has two children of height 1, then we allow the
    //two leaves to be within 1, instead of 2 
	  if (((node->child != NULL && node->child->height == 1) ||
	      (node->brother != NULL && node->brother->height == 1))&&delta>4) 
    {
      delta--;
    }
	    
    node->edge_length = ((delta+1)/2) - 1;
  }

  //now fill in the height of node
  h = 1;
  if (node->child != NULL) 
  {
	  h = MAX(node->child->height + node->edge_length + 1, h);
  }
  if (node->brother != NULL) 
  {
	  h = MAX(node->brother->height + node->edge_length + 1, h);
  }
  node->height = h;
}

//This function prints the given level of the given tree, assuming
//that the node has the given x cordinate.
void print_level(asciinode *node, int x, int level) 
{
  int i, ischild;
  if (node == NULL) return;
  ischild = (node->parent_dir == -1);
  if (level == 0) 
  {
	  for (i=0; i<(x-print_next-((node->lablen-ischild)/2)); i++) 
    {
	    printf(" ");
    }
	  print_next += i;
	  printf("%s", node->label);
	  print_next += node->lablen;
  } 
  else if (node->edge_length >= level) 
  {
	  if (node->child != NULL) 
    {
	    for (i=0; i<(x-print_next-(level)); i++) 
      {
		    printf(" ");
	    }
	    print_next += i;
	    printf("/");
	    print_next++;
    }
	  if (node->brother != NULL) 
    {
	    for (i=0; i<(x-print_next+(level)); i++) 
      {
		    printf(" ");
	    }
	    print_next += i;
	    printf("\\");
	    print_next++;
    }
  } 
  else 
  {
	  print_level(node->child, 
                x-node->edge_length-1, 
                level-node->edge_length-1);
	  print_level(node->brother, 
                x+node->edge_length+1, 
                level-node->edge_length-1);
  }
}

//prints ascii tree for given GNode structure
void print_ascii_tree(GNode  t) 
{
  asciinode *proot;
  int xmin, i;
  if (t == NULL) return;
  proot = build_ascii_tree(t);
  compute_edge_lengths(proot);
  for (i=0; i<proot->height && i < MAX_HEIGHT; i++) 
  {
	  lprofile[i] = INFINITY;
  }
  compute_lprofile(proot, 0, 0);
  xmin = 0;
  for (i = 0; i < proot->height && i < MAX_HEIGHT; i++) 
  {
	  xmin = MIN(xmin, lprofile[i]);
  }
  for (i = 0; i < proot->height; i++) 
  {
	  print_next = 0;
	  print_level(proot, -xmin, i);
	  printf("\n");
  }
  if (proot->height >= MAX_HEIGHT) 
  {
	  printf("(This tree is taller than %d, and may be drawn incorrectly.)\n", MAX_HEIGHT);
  }
  free_ascii_tree(proot); 
}


int main() {


  /*
  100
  |
  |----1
  |    |----0
  |    |----0
  |    |----0
  |
  |----2
  |    |----4
  |
  |----3
       |----19
       |----22
  */


  GTree tree = gtree_create(copy_int, destroy_int, compare_int, visit_int);

  int n = 100;
  tree->root = new(&n);
  
  GNode t = tree->root;

  n = 1;
  t->child = new(&n);

  n = 2;
  t->child->brother = new(&n);

  n = 3;
  t->child->brother->brother = new(&n);

  // 1
  n = 0;
  t->child->child = new(&n);
  t->child->child->brother = new(&n);
  t->child->child->brother->brother = new(&n);

  // 2
  n = 4;
  t->child->brother->child = new(&n);

  // 3
  n = 19;
  t->child->brother->brother->child = new(&n);
  n = 22;
  t->child->brother->brother->child->brother = new(&n);

  print_ascii_tree(t);

  puts("");

  gtree_print(tree);

  gtree_destroy(tree);

  puts("");
  return 0;
}