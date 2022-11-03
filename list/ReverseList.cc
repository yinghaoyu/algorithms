#include <iostream>
class Node
{
 public:
  int value;
  Node *next;

  Node(int data) { value = data; }
};

class DoubleNode
{
 public:
  int value;
  DoubleNode *pre;
  DoubleNode *next;

  DoubleNode(int data) { value = data; }
};

//  head
//   a    ->   b    ->  c  ->  null
//   c    ->   b    ->  a  ->  null
Node *reverseLinkedList(Node *head)
{
  Node *pre = nullptr;
  Node *next = nullptr;
  while (head != nullptr)
  {
    next = head->next;  // 先记录next节点
    head->next = pre;   // 当前节点的next改为pre
    pre = head;         // 记录 pre
    head = next;        // head往后走
  }
  return pre;
}

DoubleNode *reverseDoubleList(DoubleNode *head)
{
  DoubleNode *pre = nullptr;
  DoubleNode *next = nullptr;
  while (head != nullptr)
  {
    next = head->next;
    head->next = pre;
    head->pre = next;
    pre = head;
    head = next;
  }
  return pre;
}
