#include <stdio.h>
#include <stdlib.h>
#define item int

typedef struct Node
{ struct Node * link;

  item x;
} Node;

int sll_remove(Node ** rootp, Node * node)
{
  Node * first = (*rootp);

  Node * previous = (Node *)malloc(sizeof(Node));

  while ( (*rootp) != NULL )
  {
    if ( (*rootp) == node )
    {
      
      if ( (*rootp) == first && first->link == NULL )
      {
      
        (*rootp) = NULL;

      }
    

    else if ( (*rootp)->link == NULL )
    {
      previous->link = NULL;
    }

    else //node is between first and last nodes
    {
      (*rootp)->x = (*rootp)->link->x;

      (*rootp)->link = (*rootp)->link->link;
    }

    return 1;

    }

    previous = (*rootp); (*rootp) = (*rootp)->link;

  }

  return 0;

}

int main(void) {
  Node * head = malloc(sizeof(Node));

  head->x = 3;

  Node * b = malloc(sizeof(Node));

  b->x = 5;

  Node * c = malloc(sizeof(Node));

  c->x = 6;

  head->link = b;

  b->link = c;

  c->link = NULL;

  #if 0

  sll_remove(head,head);

  printf("%d\n",head->x);

  printf("%d\n",head->link->x);

  sll_remove(head,b);

  printf("%d\n",head->link->x);

  sll_remove(head,c);

  printf("%d\n",head->link->x);

  printf("%d\n",head->link->link->x);

  #endif

  Node * e = (Node *)malloc(sizeof(Node));

  Node * f = e;

  sll_remove(&e,e);

  printf("%d\n",e->x);

  #endif

  return 0;
}
