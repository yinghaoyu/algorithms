#include <iostream>

using namespace std;

// morris遍历可以将非递归遍历中的空间复杂度降为O(1)。
// 从而实现时间复杂度为O(N)，而空间复杂度为O(1)。
// morris遍历利用的是树的叶节点左右孩子为空（树的大量空闲指针），实现空间开销的极限缩减。

// 限制条件：可以暂时改动树的结构，然后恢复

// https://zhuanlan.zhihu.com/p/101321696

class Node
{
  public:
    int value;
    Node* left;
    Node* right;

    Node(int data)
    {
      this->value = data;
    }
};

// 一般的递归写法
void process(Node* root)
{
  if (root == nullptr)
  {
    return;
  }
  // 1
  process(root->left);
  // 2
  process(root->right);
  // 3
}

// morris遍历结构
void morris(Node* head)
{
  if (head == nullptr)
  {
    return;
  }

  Node* cur = head;
  Node* mostRight = nullptr;  // 指向左孩子的最右节点
  while (cur != nullptr)
  {
    mostRight = cur->left;
    if (mostRight != nullptr)  // 有左孩子
    {
      while (mostRight->right != nullptr && mostRight->right != cur)
      {
        mostRight = mostRight->right; // 找到左孩子最右的节点
      }
      // 左孩子的最右节点开始时是叶子节点
      if (mostRight->right == nullptr)  // 如果没有被改动过
      {
        mostRight->right = cur;  // 把这个指针指向当前节点
        cur = cur->left;  // 遍历左孩子
        continue;
      }
      else  // 如果我们改动过
      {
        mostRight->right = nullptr;  // 恢复为叶子节点
      }
    }
    cur = cur->right;  // 左孩子都遍历结束，轮到右孩子了
  }
}

// morris先序遍历
void morrisPre(Node* head)
{
  if (head == nullptr)
  {
    return;
  }
  Node* cur = head;
  Node* mostRight = nullptr;
  while (cur != nullptr)
  {
    mostRight = cur->left;
    if (mostRight != nullptr)
    {
      while (mostRight->right != nullptr && mostRight->right != cur)
      {
        mostRight = mostRight->right;
      }
      if (mostRight->right == nullptr)
      {
        // Morris遍历有些节点会遍历2次
        cout << cur->value << " ";  // 第一次遍历cur为找到左孩子的最右节点为空的时候
        mostRight->right = cur;
        cur = cur->left;
        continue;
      }
      else
      {
        // 恢复节点指针是第二次遍历cur就不处理
        mostRight->right = nullptr;
      }
    }
    else // 没有左孩子的时候，这个节点只会遍历一次
    {
      cout << cur->value << " ";
    }
    cur = cur->right;
  }
  cout<< endl;
}

// morris中序遍历
void morrisIn(Node* head)
{
  if (head == nullptr)
  {
    return;
  }
  Node* cur = head;
  Node* mostRight = nullptr;
  while (cur != nullptr)
  {
    mostRight = cur->left;
    if (mostRight != nullptr) // 处理左孩子
    {
      while (mostRight->right != nullptr && mostRight->right != cur)
      {
        mostRight = mostRight->right;
      }
      if (mostRight->right == nullptr)
      {
        mostRight->right = cur;
        cur = cur->left;
        continue;
      }
      else
      {
        mostRight->right = nullptr;
      }
    }
    cout << cur->value << " ";  // 处理根
    cur = cur->right;  // 处理右孩子
  }
  cout << endl;
}

Node* reverseEdge(Node* from)
{
  Node* pre = nullptr;
  Node* next = nullptr;
  while (from != nullptr)
  {
    next = from->right;
    from->right = pre;
    pre = from;
    from = next;
  }
  return pre;
}

void printEdge(Node* head)
{
  Node* tail = reverseEdge(head);
  Node* cur = tail;
  while (cur != nullptr)
  {
    cout << cur->value << " ";
    cur = cur->right;
  }
  reverseEdge(tail);
}

void morrisPos(Node* head)
{
  if (head == nullptr)
  {
    return;
  }
  Node* cur = head;
  Node* mostRight = nullptr;
  while (cur != nullptr)
  {
    mostRight = cur->left;
    if (mostRight != nullptr)
    {
      while (mostRight->right != nullptr && mostRight->right != cur)
      {
        mostRight = mostRight->right;
      }
      if (mostRight->right == nullptr)
      {
        mostRight->right = cur;
        cur = cur->left;
        continue;
      }
      else
      {
        mostRight->right = nullptr;
        printEdge(cur->left);
      }
    }
    cur = cur->right;
  }
  printEdge(head);
  cout << endl;
}
