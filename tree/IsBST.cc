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

void in(Node* head, vector<Node*>& arr) {
  if (head == NULL)
  {
    return;
  }
  in(head->left, arr);
  arr.push_back(head);
  in(head->right, arr);
}

//Recursive验证是否是二叉查找树
bool isBST1(Node* head)
{
  if (head == NULL)
  {
    return true;
  }
  vector<Node*> arr;
  in(head, arr);
  for (int i = 1; i < arr.size(); i++)
  {
    // 中序遍历一定是有序的
    if (arr.at(i)->value <= arr.at(i - 1)->value)
    {
      return false;
    }
  }
  return true;
}

typedef struct Info
{
  bool isBST;
  int max;
  int min;

  Info(bool i, int ma, int mi)
  {
    isBST = i;
    max = ma;
    min = mi;
  }
}Info;

Info process(Node* x)
{
  if (x == NULL)
  {
    return Info(true, INT32_MIN, INT32_MAX);
  }
  Info leftInfo = process(x->left);
  Info rightInfo = process(x->right);

  int max = x->value;
  max = std::max(max, leftInfo.max);
  max = std::max(max, rightInfo.max);

  int min = x->value;
  min = std::min(min, leftInfo.min);
  min = std::min(min, rightInfo.min);

  bool isBST = true;
  if (!leftInfo.isBST)  // 左子树必须有序
  {
    isBST = false;
  }
  if (!rightInfo.isBST)  // 右子树必须有序
  {
    isBST = false;
  }
  if (leftInfo.max >= x->value) // 左子树的最大值必须小于当前节点的值
  {
    isBST = false;
  }
  if (rightInfo.min <= x->value) // 右子树的最小值必须大于当前节点的值
  {
    isBST = false;
  }
  return Info(isBST, max, min);
}

bool isBST2(Node* head)
{
  if (head == NULL)
  {
    return true;
  }
  return process(head).isBST;
}
