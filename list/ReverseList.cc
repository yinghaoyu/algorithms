#include <iostream>
class Node {
public:
  int value;
  Node* next;

  Node(int data)
  {
    value = data;
  }
};

class DoubleNode {
public:
  int value;
  DoubleNode* pre;
  DoubleNode* next;

  DoubleNode(int data)
  {
    value = data;
  }
};

//  head
//   a    ->   b    ->  c  ->  null
//   c    ->   b    ->  a  ->  null
Node* reverseLinkedList(Node* head) {
  Node *pre = NULL;
  Node *next = NULL;
  while (head != NULL)
  {
    next = head->next;
    head->next = pre;
    pre = head;
    head = next;
  }
  return pre;
}

DoubleNode* reverseDoubleList(DoubleNode* head)
{
  DoubleNode *pre = NULL;
  DoubleNode *next = NULL;
  while (head != NULL) {
    next = head->next;
    head->next = pre;
    head->pre = next;
    pre = head;
    head = next;
  }
  return pre;
}
