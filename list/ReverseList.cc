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
  Node *pre = nullptr;
  Node *next = nullptr;
  while (head != nullptr)
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
  DoubleNode *pre = nullptr;
  DoubleNode *next = nullptr;
  while (head != nullptr) {
    next = head->next;
    head->next = pre;
    head->pre = next;
    pre = head;
    head = next;
  }
  return pre;
}
