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

// UnRecursive验证二叉树是不是满二叉树
bool isCBT1(Node* head)
{
  if (head == NULL)
  {
    return true;
  }
  queue<Node*> queue;
  // 是否遇到过左右两个孩子不双全的节点
  bool leaf = false;
  Node* l = NULL;
  Node* r = NULL;
  queue.push(head);
  while (!queue.empty())
  {
    head = queue.front();
    queue.pop();
    l = head->left;
    r = head->right;
    if (
    // 如果遇到了不双全的节点之后，又发现当前节点不是叶节点
        (leaf && (l != NULL || r != NULL)) // 从遇到的第二个叶子节点开始，就必须左右都没有节点
        ||
        (l == NULL && r != NULL)  // 不双全的必须先要有左节点，再有右节点
        )
    {
      return false;
    }
    if (l != NULL)
    {
      queue.push(l);
    }
    if (r != NULL)
    {
      queue.push(r);
    }
    if (l == NULL || r == NULL)  // 第一次遇到叶子节点开始，leaf就变成了true
    {
      leaf = true;
    }
  }
  return true;
}

// 对每一棵子树，是否是满二叉树、是否是完全二叉树、高度
typedef struct Info
{
  bool isFull;
  bool isCBT;
  int height;
  Info(bool full, bool cbt, int h)
  {
    isFull = full;
    isCBT = cbt;
    height = h;
  }
}Info;

Info process(Node* X)
{
  if (X == NULL)
  {
    return Info(true, true, 0);
  }
  Info leftInfo = process(X->left);
  Info rightInfo = process(X->right);

  int height = std::max(leftInfo.height, rightInfo.height) + 1;

  bool isFull = leftInfo.isFull
      &&
      rightInfo.isFull
      && leftInfo.height == rightInfo.height;

  bool isCBT = false;
  if (isFull)  // 如果是满二叉树，一定是完全二叉树
  {
    isCBT = true;
  }
  else
  {
    // 以x为头整棵树，不满
    if (leftInfo.isCBT && rightInfo.isCBT)
    {
      if (leftInfo.isCBT
          && rightInfo.isFull
          && leftInfo.height == rightInfo.height + 1)
      {
        // 左子树是完全、右子树是满，且左子树高度比右子树高1
        isCBT = true;
      }
      if (leftInfo.isFull
          &&
          rightInfo.isFull
          && leftInfo.height == rightInfo.height + 1)
      {
        // 左子树是满，右子树是满，且左子树高度比右子树高1
        isCBT = true;
      }
      if (leftInfo.isFull
          && rightInfo.isCBT && leftInfo.height == rightInfo.height)
      {
        // 左子树是满，右子树是完全，且左右子树高度相等
        isCBT = true;
      }
    }
  }
  return Info(isFull, isCBT, height);
}

// Recursive验证树是否是满二叉树或者完全二叉树
bool isCBT2(Node* head)
{
  if (head == NULL)
  {
    return true;
  }
  return process(head).isCBT;
}
