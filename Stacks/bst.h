// bst.h

#define item int

typedef struct Node
{
  struct Node * left;

  struct Node * parent;

  struct Node * right;

  item x;
} Node;

void print_item(item x);
