#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

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
  }

  else // x >= l->x
  {
    insert_tree(&((*l)->right),x,*l);
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
  if ( l == NULL)
  {
    return;
  }
  in_order_traverse(l->left);

  print_item(l->x);

  in_order_traverse(l->right);
}

void pre_order_traverse(Node *l)
{
  if ( l == NULL )
  {
    return;
  }
  
  print_item(l->x);

  pre_order_traverse(l->left);

  pre_order_traverse(l->right);
}

void post_order_traverse(Node * l)
{

  if ( l == NULL )
  {
    return;
  }
  post_order_traverse(l->left);

  post_order_traverse(l->right);

  print_item(l->x);
}


int main(void) 
{
  
  Node * superroot = malloc(sizeof(Node));

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

 

 

 


 


}
