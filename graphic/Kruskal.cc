#include <iostream>
#include <queue>
#include <deque>
#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>

// 克鲁斯卡尔最小生成树，算法思想：选权值最小的边加入并查集

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

  bool operator < (const Edge& other)  // 小根堆的比较方式
  {
    return this->weight < other.weight;
  }
};

// 点结构的描述
class Node
{
public:
  int value;  // 节点带的值
  int in;  // 入度
  int out;  // 出度
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
// Union-Find Set
class UnionFind
{
private:
  // key 某一个节点， value key节点往上的节点
   unordered_map<Node*, Node*> fatherMap;
  // key 某一个集合的代表节点, value key所在集合的节点个数
  unordered_map<Node*, int> sizeMap;

public:
  void makeSets(list<Node*>& nodes)
  {
    fatherMap.clear();
    sizeMap.clear();
    for (auto node : nodes)
    {
      fatherMap[node] =  node;  // 初始时指定node的父节点是自己
      sizeMap[node] = 1;  // 节点所在的集合默认为1
    }
  }

  Node* findFather(Node* n)
  {
    vector<Node*> path;  // 栈
    while(n != fatherMap.at(n))
    {
      path.push_back(n);
      n = fatherMap.at(n);
    }
    while(!path.empty())
    {
      Node* temp = path.back();
      path.pop_back();
      fatherMap[temp] = n;  // 缩短路径
    }
    return n;
  }

  bool isSameSet(Node* a, Node* b)
  {
    return findFather(a) == findFather(b);
  }

  // 并查集unions
  void unions(Node* a, Node* b)
  {
    if (a == NULL || b == NULL)
    {
      return;
    }
    Node* aDai = findFather(a);
    Node* bDai = findFather(b);
    if (aDai != bDai)  // 集合元素多的吞并少的
    {
      int aSetSize = sizeMap.at(aDai);
      int bSetSize = sizeMap.at(bDai);
      if (aSetSize <= bSetSize)
      {
        fatherMap[aDai] = bDai;
        sizeMap[bDai] = aSetSize + bSetSize;
        sizeMap.erase(aDai);
      }
      else
      {
        fatherMap[bDai] = aDai;
        sizeMap[aDai] = aSetSize + bSetSize;
        sizeMap.erase(bDai);
      }
    }
  }
};

unordered_set<Edge*> kruskalMST(Graph graph)
{
  UnionFind unionFind;
  list<Node*> collect;
  for(auto node : graph.nodes)  // 先获取所有节点
  {
    collect.push_back(node.second);
  }
  unionFind.makeSets(collect);  // 初始化节点
  // 从小的边到大的边，依次弹出，小根堆！
  priority_queue<Edge*> priorityQueue;
  for (Edge* edge : graph.edges)
  {
    // M 条边
    priorityQueue.emplace(edge);  // O(logM)
  }
  unordered_set<Edge*> result;
  while (!priorityQueue.empty())
  {
    // M 条边
    Edge* edge = priorityQueue.top(); // O(logM)
    priorityQueue.pop();
    if (!unionFind.isSameSet(edge->from, edge->to))
    {
      // O(1)
      result.emplace(edge);
      unionFind.unions(edge->from, edge->to);
    }
  }
  return result;
}
