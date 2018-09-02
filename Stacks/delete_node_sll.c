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

      else if ( (*rootp) == first )
      {
        (*rootp) = NULL;
      }
      

      else if ( (*rootp)->link == NULL )
      {
        (*rootp) = NULL;

      }


      else //node is between first and last nodes before NULL, inclusive
      {
        (*rootp)->x = (*rootp)->link->x;

        (*rootp)->link = (*rootp)->link->link;
      }

    return 1;

    }

    rootp = &((*rootp)->link);
    
    

  }

  return 0;

}

int main(void) {
  
  
  Node * head = (Node *)malloc(sizeof(Node));

  head->x = 3;

  Node * b = (Node *)malloc(sizeof(Node));

  b->x = 5;

  Node * c = (Node *)malloc(sizeof(Node));

  c->x = 6;

  Node * d = (Node *)malloc(sizeof(Node));

  d->x = 100;

  head->link = b;

  b->link = c;

  c->link = d;

  d->link = NULL;

  
#if 0
  
  sll_remove(&head,head);

  printf("%d\n",b->link->link->link->x);

sll_remove(&head,d);

printf("%d\n",c->link->x);
  
#endif
  
  return 0;
}
