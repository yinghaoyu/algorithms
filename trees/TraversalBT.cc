#include <iostream>

typedef struct Node
{
  int value;
  Node* left;
  Node* right;
}Node;

// 先序
void pre(Node* head)
{
  if(head == NULL)
  {
    return;
  }
  printf("%d\n", head->value);
  pre(head->left);
  pre(head->right);
}

// 中序
void in(Node* head)
{
  if(head == NULL)
  {
    return;
  }
  pre(head->left);
  printf("%d\n", head->value);
  pre(head->right);
}

// 后序
void pos(Node* head)
{
  if(head == NULL)
  {
    return;
  }
  pre(head->left);
  pre(head->right);
  printf("%d\n", head->value);
}
