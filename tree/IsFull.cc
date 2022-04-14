#include <iostream>
#include <vector>
#include <queue>

using namespace std;

typedef struct Node
{
  int value;
  Node* left;
  Node* right;
}Node;

int h(Node* head)
{
  if (head == NULL)
  {
    return 0;
  }
  return std::max(h(head->left), h(head->right)) + 1;
}

int n(Node* head)
{
  if (head == NULL)
  {
    return 0;
  }
  return n(head->left) + n(head->right) + 1;
}

bool isFull1(Node* head)
{
  if (head == NULL)
  {
    return true;
  }
  int height = h(head);
  int nodes = n(head);
  return (1 << height) - 1 == nodes; // 满二叉树的条件是2^h - 1 = n
}

typedef struct Info
{
  int height;
  int nodes;

  Info(int h, int n)
  {
    height = h;
    nodes = n;
  }
}Info;

Info process(Node* head)
{
  if (head == NULL)
  {
    return Info(0, 0);
  }
  Info leftInfo = process(head->left);
  Info rightInfo = process(head->right);
  int height = std::max(leftInfo.height, rightInfo.height) + 1;
  int nodes = leftInfo.nodes + rightInfo.nodes + 1;
  return Info(height, nodes);
}

bool isFull2(Node* head)
{
  if (head == NULL)
  {
    return true;
  }
  Info all = process(head);
  return (1 << all.height) - 1 == all.nodes;
}
