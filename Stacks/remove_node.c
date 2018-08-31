#include <stdio.h>
#include <stdlib.h>
#include "sll.h"


int sll_len(Node * first)
{
  int len = 0;
  
  while ( first != NULL )
  {
    len++;

    first = first->link;
  }

  return len;
}

int remove_node(Node ** rootp, Node * node)
{
  if ( (*rootp) == NULL )
  { return NULL; }

  Node * first = *rootp;

  while ( (*rootp) != NULL )
  {
    if ( (*rootp) == node)
    {
      if ( ((*rootp)->link == NULL) && (first->link != NULL) ) // last node in linked list
      { (*(rootp-1))->link = NULL; }

      else if (  ((*rootp)->link == NULL) && (first->link == NULL)) // first node AND last node in linked list
      { (*rootp)->x = 0; }

      else // either first node or in between first and last nodes
      {
        ((*rootp)->x) = ((*(rootp+1))->x);

        (*rootp)->link = (*(rootp+2));
      }
      return 1;
    }

    rootp++;

  }

  return 0;

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

  Node * first = a;

  #if 0
  
  Node * rev = reverse_sll(a);

  
  printf("%d\n",rev->link->link->link->link->x);
  
  #endif

  Node * node_arr[1000];

  node_arr[0] = a;

  node_arr[1] = b;

  node_arr[2] = c;

  node_arr[3] = d;

  node_arr[4] = NULL;

  Node ** node_arr_p = node_arr;



Node * alone = (Node *)malloc(sizeof(Node));

alone->x = 125;

alone->link = NULL;

Node * node_arr_two[2];

node_arr_two[0] = alone;

node_arr_two[1] = NULL;

remove_node(node_arr,b);

printf("%d\n",a->link->x);

remove_node(node_arr_two,alone);

printf("%d\n",alone->x);

remove_node(node_arr,a);

printf("%d\n",a->x);




  
}
