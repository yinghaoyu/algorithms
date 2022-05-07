#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>

using namespace std;

typedef struct Node
{
  int value;
  Node* left;
  Node* right;
}Node;

// key, value 子节点指针，父节点指针
void fillParentMap(Node* head, unordered_map<Node*, Node*>& parentMap)
{
  if (head->left != nullptr)
  {
    parentMap.emplace(head->left, head);
    fillParentMap(head->left, parentMap);
  }
  if (head->right != nullptr)
  {
    parentMap.emplace(head->right, head);
    fillParentMap(head->right, parentMap);
  }
}

// 查找两个节点最近的祖先
Node* lowestAncestor1(Node* head, Node* o1, Node* o2)
{
  if (head == nullptr)
  {
    return nullptr;
  }
  // key的父节点是value
  unordered_map<Node*, Node*> parentMap;
  parentMap.emplace(head, nullptr);
  fillParentMap(head, parentMap);
  unordered_set<Node*> o1Set;
  Node* cur = o1;
  o1Set.emplace(cur);
  while (parentMap.find(cur) != parentMap.end())
  {
    cur = parentMap.at(cur);  // 一直查找o1的父节点
    o1Set.emplace(cur);  // o1父节点的集合
  }
  cur = o2;
  while (o1Set.find(cur) == o1Set.end())
  {
    cur = parentMap.at(cur);  // 从o1的父节点集合查找o2的父节点
  }
  return cur;  // 最差返回根节点
}

typedef struct Info
{
  bool findA;
  bool findB;
  Node* ans;

  Info(bool fA, bool fB, Node* an)
  {
    findA = fA;
    findB = fB;
    ans = an;
  }
}Info;

Info process(Node* x, Node* a, Node* b)
{
  if (x == nullptr)
  {
    return Info(false, false, nullptr);
  }
  Info leftInfo = process(x->left, a, b);
  Info rightInfo = process(x->right, a, b);
  bool findA = (x == a) || leftInfo.findA || rightInfo.findA;
  bool findB = (x == b) || leftInfo.findB || rightInfo.findB;
  Node* ans = nullptr;
  if (leftInfo.ans != nullptr)
  {
    ans = leftInfo.ans;  // 向上传递左子树的结果
  }
  else if (rightInfo.ans != nullptr)
  {
    ans = rightInfo.ans;  // 向上传递右子树的结果
  }
  else
  {
    if (findA && findB)  // 当前节点是最近的公共祖先
    {
      ans = x;
    }
  }
  return Info(findA, findB, ans);
}

Node* lowestAncestor2(Node* head, Node* a, Node* b)
{
  return process(head, a, b).ans;
}
