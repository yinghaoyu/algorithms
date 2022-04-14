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
      s1.pop_back(); // 头 右 左
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
    // 左 右 头
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

// bfs
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
