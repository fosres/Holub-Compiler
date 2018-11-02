#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

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
  }

  else // x >= l->x
  {
    insert_tree(l->right,x);
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
  in_order_traverse(l->left);

  print_item(l->x);

  in_order_traverse(l->right);
}

void pre_order_traverse(Node *l)
{
  print_item(l->x);

  pre_order_traverse(l->left);

  pre_order_traverse(l->right);
}

void post_order_traverse(Node * l)
{
  post_order_traverse(l->left);

  post_order_traverse(l->right);

  print_item(l->x);
}


int main(void) 
{
  
  Node * superroot = malloc(sizeof(Node));

  superroot->x = 1;

 insert_tree(superroot,3);

 Node * hright = superroot->right;

 print_item(hright->x);

}
