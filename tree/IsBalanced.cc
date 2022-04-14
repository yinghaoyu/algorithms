#include <iostream>
#include <stdbool.h>
#include <vector>
#include <queue>

using namespace std;

typedef struct Node
{
  int value;
  Node* left;
  Node* right;
}Node;

int process1(Node* head, bool& ans)
{
  if (!ans || head == NULL)
  {
    return -1;
  }
  int leftHeight = process1(head->left, ans);
  int rightHeight = process1(head->right, ans);
  if (std::abs(leftHeight - rightHeight) > 1)  // 只要高度差大于1就是非平衡二叉树
  {
    ans = false;
  }
  return std::max(leftHeight, rightHeight) + 1;
}

// Recursive验证是否平衡
bool isBalanced1(Node* head)
{
  bool ans = true;
  process1(head, ans);
  return ans;
}

typedef struct Info
{
  bool isBalanced;
  int height;

  Info(bool i, int h)
  {
    isBalanced = i;
    height = h;
  }
}Info;


Info process(Node* x) {
  if(x == NULL)
  {
    return Info(true, 0);
  }
  Info leftInfo = process(x->left);
  Info rightInfo = process(x->right);
  int height = std::max(leftInfo.height, rightInfo.height)  + 1;
  bool isBalanced = true;
  if(!leftInfo.isBalanced)
  {
    isBalanced = false;
  }
  if(!rightInfo.isBalanced)
  {
    isBalanced = false;
  }
  if(std::abs(leftInfo.height - rightInfo.height) > 1)
  {
    isBalanced = false;
  }
  return Info(isBalanced, height);
}

// Recursive验证是否平衡
bool isBalanced2(Node* head)
{
  return process(head).isBalanced;
}
