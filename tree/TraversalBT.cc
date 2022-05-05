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

// 在递归序的基础上，只打印第一次经过自己时的值，即是先序；
// 只打印第二次经过自己的值，即是中序；
// 只打印第三次经过自己的值，即是后序。

// 一个结论：已知一个二叉树的先序遍历和后序遍历，
// 某个节点X，X先序遍历之前的节点集合为A，
// X后序遍历之后的节点集合为B，
// 那么 A 和 B 的交集一定是X节点的所有祖先节点。

// Recursive先序
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

// Recursive中序
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

// Recursive后序
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

// UnRecursive先序
//（1）准备一个栈，压入当前节点（即头节点）
//（2）弹出栈顶元素，打印对应的值
//（3）此元素有右孩子往栈压入右孩子，有左孩子往栈压入左孩子（先右再左）
//（4）一直执行2、3步，直到栈为空。
void preUnRe(Node* head)
{
  if (head != NULL)
  {
    vector<Node*> stack;
    stack.push_back(head);
    while (!stack.empty())
    {
      head = stack.back();
      stack.pop_back();
      printf("%d\n", head->value); // 先处理根节点
      if (head->right != NULL)
      {
        stack.push_back(head->right);
      }
      if (head->left != NULL)
      {
        stack.push_back(head->left);
      }
    }
  }
}

// UnRecursive中序
//（1）准备一个栈
//（2）压入以当前节点current为头节点的整个左子树（入栈一个，current就移动到左孩子），直到为空
//（3）弹出栈顶元素，打印其值，以当前弹出元素的右孩子为current节点，重复第2步
//（4）当栈为空时结束
void inUnRe(Node* cur)
{
  if (cur != NULL) {
    vector<Node*> stack;
    while (!stack.empty() || cur != NULL)
    {
      if (cur != NULL)  // 先把左孩子全部入栈
      {
        stack.push_back(cur);
        cur = cur->left;
      }
      else
      {
        cur = stack.back();
        stack.pop_back();
        printf("%d\n", cur->value);  // 出栈的时候处理根节点
        cur = cur->right;  // 再把右孩子全部入栈
      }
    }
  }
}

// UnRecursive后序
//（1）准备两个栈stackA，stackB；stackA压入当前节点（即头节点）
//（2）弹出栈顶元素，压入stackB
//（3）此元素有左孩子往stackA栈压入左孩子，有右孩子往stackA栈压入右孩子（先左再右）
//（4）一直执行2、3步，直到stackA栈为空。
//（5）打印所有stackB栈的元素
// 相当于stackA出栈顺序是 根右左，最后stackB出栈顺序是 左右根。
void pos1(Node* head)
{
  if (head != NULL)
  {
    vector<Node*> s1;
    vector<Node*> s2;
    s1.push_back(head);
    while (!s1.empty())
    {
      head = s1.back();
      s1.pop_back(); // 跟 右 左
      s2.push_back(head);
      if (head->left != NULL)
      {
        s1.push_back(head->left);
      }
      if (head->right != NULL)
      {
        s1.push_back(head->right);
      }
    }
    // 左 右 跟
    while (!s2.empty()) {
      Node* cur = s2.back();
      s2.pop_back();
      printf("%d\n", cur->value);
    }
  }
}

// UnRecursive后序
void pos2(Node* head)
{
  if (head != NULL)
  {
    vector<Node*> stack;
    stack.push_back(head);
    Node* c = NULL;
    while (!stack.empty()) {
      c = stack.back();
      if (c->left != NULL && head != c->left && head != c->right)
      {
        stack.push_back(c->left);
      }
      else if (c->right != NULL && head != c->right)
      {
        stack.push_back(c->right);
      }
      else
      {
        stack.pop_back();
        printf("%d\n", c->value);
        head = c;  // 用于记录本次处理过的根节点
      }
    }
  }
}

// bfs层序遍历
void level(Node* head)
{
  if (head == NULL)
  {
    return;
  }
  queue<Node*> queue;
  queue.push(head);
  while (!queue.empty())
  {
    Node* cur = queue.front();
    queue.pop();
    printf("%d\n", cur->value);
    if (cur->left != NULL)
    {
      queue.push(cur->left);
    }
    if (cur->right != NULL)
    {
      queue.push(cur->right);
    }
  }
}
