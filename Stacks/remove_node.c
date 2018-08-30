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

  Node * first = *rootp;

  while ( (*rootp) != NULL )
  {
    if ( (*rootp) == node )
    {
      if ( (*rootp) == first /* && ( (*rootp)->link != NULL ) */)
      { 
        (*rootp)->x = (*(rootp+1))->x; 

        (*rootp)->link = (*(rootp+1))->link;
        
      }

      #if 0
      else if ( ( (*rootp) == first ) && ( (*rootp)->link == NULL ) )
      {
        (*rootp)->x = NULL;
      }
      #endif

      else if ( (*rootp)->link == NULL )
      { (*(rootp - 1))->link = NULL; }
      
      else
      {
        (*(rootp - 1))->link =  (*(rootp + 1));
      }

      return 1;
    }

    rootp++;
  }

  return 0;

}

#if 0
Node * reverse_sll(Node * first)
{
 {
    
 } Node ** arr_p = &arr[0] - 1;

  while (first != NULL )
  {
    *++arr_p = first;

    first = first->link;
  }

  Node * new_head = *arr_p;

  while (arr_p > &arr[0])
  {
    (*arr_p)->link = *(arr_p - 1);

    arr_p--;
  }
  (*arr_p)->link = NULL;

  return new_head;
}
#endif

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

#if 0
  printf("%d\n",(*node_arr_p++)->x);
  
  printf("%d\n",(*node_arr_p++)->x);

  printf("%d\n",(*node_arr_p)->x);

#endif


remove_node(node_arr,a);
printf("%d\n",a->link->x);


Node * alone = (Node *)malloc(sizeof(Node));

alone->x = 125;

alone->link = NULL;

Node * node_arr_two[2];

node_arr_two[0] = alone;

node_arr_two[1] = NULL;

remove_node(node_arr_two,alone);

printf("%p\n",alone);
  
}
