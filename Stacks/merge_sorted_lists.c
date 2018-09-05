#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
  struct Node * link;

  int x;
} Node;

void print_list(Node * list)
{
  while ( list != NULL )
  {
    printf("%d ",list->x);

    list = list->link;
  }
}
Node * merge_sorted_lists(Node * a, Node * b)
{
  int len = 0;

  Node * a_c = a;

  while ( a_c != NULL )
  {
    len++; a_c = a_c->link;
  }

  Node * b_c = b; 

  while ( b_c != NULL )
  {
    len++; b_c = b_c->link;
  }

  len++; //account for NULL in merged list

  Node * node_arr[len];

  Node ** node_arr_p = &node_arr[0];

  while ( a != NULL || b != NULL )
  {
    if ( a == NULL )
    {
      *node_arr_p++ = b;

      b = b->link;
    }

    else if ( b == NULL )
    {
      *node_arr_p++ = a;

      a = a->link;
    }

    else if ( (a->x) <= (b->x) )
    {
      *node_arr_p++ = a;

      *node_arr_p++ = b;

      a = a->link; b = b->link;
    }

    else // a->x > b->x
    {
      *node_arr_p++ = b;

      *node_arr_p++ = a;

      b = b->link;

      a = a->link;
    }

  }

  *node_arr_p = NULL;

  node_arr_p = &node_arr[0];

  while ( (*node_arr_p) != NULL )
  {
    (*node_arr_p)->link = (*(node_arr_p+1)); 

    node_arr_p++;
  }

  return *(&node_arr[0]);
}

int main(void) {
  Node * a = (Node *)malloc(sizeof(Node));

  a->x = 1;

  Node * b = (Node *)malloc(sizeof(Node));

  b->x = 2;

  Node * c = (Node *)malloc(sizeof(Node));

  c->x = 3;

  Node * d = (Node *)malloc(sizeof(Node));



   a->link = b; 

   b->link = c; 

   c->link = NULL;

  
  Node * a_2 = (Node *)malloc(sizeof(Node));

  a_2->x = 2;

  Node * b_2 = (Node *)malloc(sizeof(Node));

  b_2->x = 3;

  Node * c_2 = (Node *)malloc(sizeof(Node));

  c_2->x = 7;

  Node * d_2 = (Node *)malloc(sizeof(Node));
  
  a_2->link = b_2;

  b_2->link = c_2;

  c_2->link = NULL;

  

  Node * ans = merge_sorted_lists(a,a_2);

  print_list(ans);
}
