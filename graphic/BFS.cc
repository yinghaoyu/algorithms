#include <iostream>
#include <deque>
#include <list>
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

// 从node出发，进行宽度优先遍历
void bfs(Node* start)
{
  if (start == nullptr)
  {
    return;
  }
  deque<Node*> queue;
  unordered_set<Node*> set;
  queue.push_back(start);
  set.emplace(start);
  while (!queue.empty())
  {
    Node* cur = queue.front();
    queue.pop_front();
    printf("%d\n", cur->value);  // 处理
    for (Node* next : cur->nexts)  // 把所有子节点入队
    {
      if (set.find(next) == set.end())
      {
        set.emplace(next);
        queue.push_back(next);
      }
    }
  }
}
