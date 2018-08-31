#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
  struct Node * link;

  int x;
} Node;

Node * sll_reverse(Node * first)
{
  
  if ( first == NULL)
  { return NULL;}
  
  
  Node * first_counter = first;
  
  first_counter = first;

  Node * node_arr[1000]; Node ** node_arr_p = node_arr;

  while ( first_counter != NULL )
  {
    *node_arr_p++ = first_counter;

    first_counter = first_counter->link;

  }
  
  *node_arr_p = NULL;

  node_arr_p--; 
  
  Node * return_node = *node_arr_p;

  while ( node_arr_p > &node_arr[0] )
  {
    
    if (node_arr_p == &node_arr[0])
    {
      (*node_arr_p)->link = NULL;
    }

    else
    {
      (*node_arr_p)->link = (*(node_arr_p-1));
    }

    node_arr_p--;
  }

  return return_node;
}

int main(void) {
  
  Node * a = (Node *)malloc(sizeof(Node));

  a->x = 3;

  Node * b = (Node *)malloc(sizeof(Node));

  b->x = 5;

  Node * c = (Node *)malloc(sizeof(Node));

  c->x = 6;

  Node * d = (Node *)malloc(sizeof(Node));

  d->x = 7;

  a->link = b;

  b->link = c;

  c->link = d;

  d->link = NULL;

  Node * rev = sll_reverse(a);

  printf("%d\n",rev->x);
  
}
