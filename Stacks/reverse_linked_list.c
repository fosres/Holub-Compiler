#include <stdio.h>
#include <stdlib.h>
#include "sll.h"

Node * reverse_sll(Node * first)
{
  Node * arr[1000]; Node ** arr_p = &arr[0] - 1;

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

  Node * rev = reverse_sll(a);

  printf("%d\n",rev->link->link->link->link->x);
}
