#include <iostream>
#include <queue>
#include <deque>
#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>

// 普里姆最小生成树
// 算法思想：
//（1）可以从任意节点出发来寻找最小生成树
//（2）某个点加入到被选取的点中后，解锁这个点出发的所有新边
//（3）在所有解锁的边中选最小的边，然后看这个边加入到被解锁的点中后会不会形成环
//（4）如果会，舍弃当前边，返回第3步；如果不会，保留当前边，将该边的指向点解锁，此边加入到结果中，返回第2步
//（5）当所有点都被解锁时，最小生成树就得到了

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

unordered_set<Edge*> primMST(Graph graph)
{
  // 解锁的边进入小根堆
  priority_queue<Edge*> priorityQueue;

  // 哪些点被解锁出来了
  unordered_set<Node*> nodeSet;

  unordered_set<Edge*> result; // 依次挑选的的边在result里

  for (auto node : graph.nodes)
  {
    // 随便挑了一个点
    // node 是开始点
    Node* cur = node.second;
    if (nodeSet.find(cur) == nodeSet.end())
    {
      nodeSet.emplace(cur);
      for (Edge* edge : cur->edges)
      {
        // 由一个点，解锁所有相连的边
        priorityQueue.push(edge);
      }
      while (!priorityQueue.empty())
      {
        Edge* edge = priorityQueue.top(); // 弹出解锁的边中，最小的边
        priorityQueue.pop();
        Node* toNode = edge->to; // 可能的一个新的点
        if (nodeSet.find(toNode) == nodeSet.end())
        {
          // 不含有的时候，就是新的点
          nodeSet.emplace(toNode);
          result.emplace(edge);
          for (Edge* nextEdge : toNode->edges)
          {
            priorityQueue.emplace(nextEdge);
          }
        }
      }
    }
    // break;
  }
  return result;
}
// 请保证graph是连通图
// graph[i][j]表示点i到点j的距离，如果是系统最大值代表无路
// 返回值是最小连通图的路径之和
int prim(int** graph, int n)
{
  int size = n;
  int* distances = new int[size];
  bool* visit = new bool[size];
  visit[0] = true;
  for (int i = 0; i < size; i++)
  {
    distances[i] = graph[0][i];  // 表示节点i加入连通图最小生成树需要的最短距离
  }
  int sum = 0;
  for (int i = 1; i < size; i++)
  {
    int minPath = INT32_MAX;
    int minIndex = -1;
    for (int j = 0; j < size; j++)
    {
      if (!visit[j] && distances[j] < minPath)  // 找到距离最小的点
      {
        minPath = distances[j];
        minIndex = j;
      }
    }
    if (minIndex == -1)  // 所有边遍历结束
    {
      return sum;
    }
    visit[minIndex] = true;  // 标记已访问
    sum += minPath;  // 记录距离
    for (int j = 0; j < size; j++)
    {
      // 因为点minIndex的加入，需要更新j加入连通图最小生成树的最短距离
      if (!visit[j] && distances[j] > graph[minIndex][j])
      {
        distances[j] = graph[minIndex][j];
      }
    }
  }
  return sum;
}
