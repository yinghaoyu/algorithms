#include <iostream>
#include <deque>
#include <list>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class Node;
class Edge
{
public:
  int weight;  // 边的权重
  Node* from;  // 边的起点
  Node* to;   // 边的终点

  Edge(int weight, Node* from, Node* to)
  {
    this->weight = weight;
    this->from = from;
    this->to = to;
  }
};

// 点结构的描述
class Node
{
public:
  int value;  // 节点带的值
  int in;
  int out;
  list<Node*> nexts;  // 可达的节点
  list<Edge*> edges;  // 关联的边

  Node(int value)
  {
    this->value = value;
    in = 0;
    out = 0;
  }
};

class Graph
{
public:
  unordered_map<int, Node*> nodes; // 所有点集合
  unordered_set<Edge*> edges;  // 所有边集合
};
void dfs(Node* node)
{
  if (node == nullptr)
  {
    return;
  }
  vector<Node*> stack;
  unordered_set<Node*> set;
  stack.push_back(node);
  set.emplace(node);
  printf("%d\n", node->value);
  while (!stack.empty())
  {
    Node* cur = stack.back();
    stack.pop_back();
    for (Node* next : cur->nexts)
    {
      if (set.find(next) == set.end())
      {
        stack.push_back(cur);
        stack.push_back(next);
        set.emplace(next);
        printf("%d\n", next->value);
        break;
      }
    }
  }
}
