#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

<<<<<<< HEAD
void insert_tree(Node * l, item x)
{
  if (l == NULL)
  {

    l = malloc(sizeof(Node));

    l->x = x;

    l->left = l->right = NULL;

    return;
  }

  else if ( x < l->x )
  {
    insert_tree(l->left,x);
=======
void insert_tree(Node ** l, item x, Node * parent)
{
  if (*l == NULL)
  {
    Node * insert = malloc(sizeof(Node));
    
    insert->x = x;

    insert->parent = parent;
    
    insert->left = insert->right = NULL;

    *l = insert;
    
    return;
  }

  else if ( x < (*l)->x )
  {
    insert_tree(&((*l)->left),x,*l);
>>>>>>> 1eedc0f8c585e92145cfa9b53cdc450a80501282
  }

  else // x >= l->x
  {
<<<<<<< HEAD
    insert_tree(l->right,x);
=======
    insert_tree(&((*l)->right),x,*l);
>>>>>>> 1eedc0f8c585e92145cfa9b53cdc450a80501282
  }

}

void print_item(item x)
{
  printf("%d\n",x);
}

Node * find_max(Node * superroot)
{

  if ( superroot->right != NULL )
  {
   return find_max(superroot->right);
  }

  return superroot;
}

Node * find_min(Node * superroot)
{
  
  if ( superroot->left != NULL )
  {
    return find_min(superroot->left);
  }

  return superroot;
}

void in_order_traverse(Node * l)
{
<<<<<<< HEAD
=======
  if ( l == NULL)
  {
    return;
  }
>>>>>>> 1eedc0f8c585e92145cfa9b53cdc450a80501282
  in_order_traverse(l->left);

  print_item(l->x);

  in_order_traverse(l->right);
}

void pre_order_traverse(Node *l)
{
<<<<<<< HEAD
=======
  if ( l == NULL )
  {
    return;
  }
  
>>>>>>> 1eedc0f8c585e92145cfa9b53cdc450a80501282
  print_item(l->x);

  pre_order_traverse(l->left);

  pre_order_traverse(l->right);
}

void post_order_traverse(Node * l)
{
<<<<<<< HEAD
=======

  if ( l == NULL )
  {
    return;
  }
>>>>>>> 1eedc0f8c585e92145cfa9b53cdc450a80501282
  post_order_traverse(l->left);

  post_order_traverse(l->right);

  print_item(l->x);
}


int main(void) 
{
  
  Node * superroot = malloc(sizeof(Node));

<<<<<<< HEAD
  superroot->x = 1;

 insert_tree(superroot,3);

 Node * hright = superroot->right;

 print_item(hright->x);
=======
 insert_tree(&superroot,3,NULL);

 insert_tree(&superroot,9,NULL);

 insert_tree(&superroot,12,NULL);

 insert_tree(&superroot,15,NULL);

 #if 0

 print_item(find_max(superroot)->x);

 insert_tree(&superroot,1,NULL);

 print_item(find_min(superroot)->x);

 in_order_traverse(superroot);

 pre_order_traverse(superroot);

 post_order_traverse(superroot);

 #endif

 

 

 


 

>>>>>>> 1eedc0f8c585e92145cfa9b53cdc450a80501282

}
