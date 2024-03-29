#include <deque>
#include <iostream>
#include <list>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Dijkstra单源最短路径
// 这种方法可以针对有向图

using namespace std;
using TdArray = vector<vector<int>>;

class Node;
class Edge
{
 public:
  int weight;  // 边的权重
  Node *from;  // 边的起点
  Node *to;    // 边的终点

  Edge(int weight, Node *from, Node *to)
  {
    this->weight = weight;
    this->from = from;
    this->to = to;
  }

  bool operator<(const Edge &other)  // 小根堆的比较方式
  {
    return this->weight < other.weight;
  }
};
// 点结构的描述
class Node
{
 public:
  int value;           // 节点带的值，注意这里不是边的权值
  int in;              // 入度，表示有多少条入边
  int out;             // 出度，表示有多少条出边
  list<Node *> nexts;  // 可达的节点
  list<Edge *> edges;  // 关联的边，注意这里表示出边

  Node(int value)
  {
    this->value = value;
    in = 0;
    out = 0;
  }
};

// 从distanceMap中找出不在touchedNodes的距离最短的点
Node *getMinDistanceAndUnselectedNode(unordered_map<Node *, int> &distanceMap, unordered_set<Node *> &touchedNodes)
{
  Node *minNode = nullptr;
  int minDistance = INT32_MAX;
  for (auto entry : distanceMap)
  {
    Node *node = entry.first;
    int distance = entry.second;
    if (touchedNodes.find(node) == touchedNodes.end() && distance < minDistance)
    {
      minNode = node;
      minDistance = distance;
    }
  }
  return minNode;
}

// 返回从from开始到其他节点的最短距离
unordered_map<Node *, int> dijkstra1(Node *from)
{
  unordered_map<Node *, int> distanceMap;
  distanceMap[from] = 0;  // 自己到自己肯定是0
  // 打过对号的点
  unordered_set<Node *> selectedNodes;
  // 选出未选中的距离最短的可达节点
  Node *minNode = getMinDistanceAndUnselectedNode(distanceMap, selectedNodes);
  while (minNode != nullptr)
  {
    //  原始点  ->  minNode(跳转点)   最小距离distance
    int distance = distanceMap.at(minNode);
    for (Edge *edge : minNode->edges)
    {
      Node *toNode = edge->to;
      if (distanceMap.find(toNode) == distanceMap.end())  // 第一次计算可达节点
      {
        distanceMap[toNode] = distance + edge->weight;  // 加入可达点及距离
      }
      else
      {
        // A -> minNode -> B 可能比 A -> B 的距离更短
        distanceMap[toNode] = std::min(distanceMap.at(toNode), distance + edge->weight);
      }
    }
    selectedNodes.emplace(minNode);  // 加入可达节点集合
    minNode = getMinDistanceAndUnselectedNode(distanceMap, selectedNodes);
  }
  return distanceMap;
}

class NodeRecord
{
 public:
  Node *node;
  int distance;

  NodeRecord(Node *node, int distance)
  {
    this->node = node;
    this->distance = distance;
  }
};

class NodeHeap
{
 private:
  vector<Node *> nodes;  // 实际的堆结构
  // key 某一个node， value 上面堆中的位置
  unordered_map<Node *, int> heapIndexMap;
  // key 某一个节点， value 从源节点出发到该节点的目前最小距离
  unordered_map<Node *, int> distanceMap;
  int size;  // 堆上有多少个点
 public:
  NodeHeap(int size)
  {
    nodes = vector<Node *>(size, nullptr);
    this->size = 0;
  }
  bool isEmpty() { return size == 0; }

  // 有一个点叫node，现在发现了一个从源节点出发到达node的距离为distance
  // 判断要不要更新，如果需要的话，就更新
  void addOrUpdateOrIgnore(Node *node, int distance)
  {
    if (inHeap(node))
    {
      distanceMap[node] = std::min(distanceMap.at(node), distance);
      // 小根堆必须满足规则：所有父节点都小于子节点
      // 因为这里只会取更小的值，这样不会破坏这个节点及子树的规则，所以只需要向上调整
      // 如果这里取的是最大值，那么必须向下调整
      insertHeapify(node, heapIndexMap.at(node));
    }
    if (!isEntered(node))
    {
      // 第一次添加，添加到尾部，向上调整堆
      nodes[size] = node;
      heapIndexMap[node] = size;
      distanceMap[node] = distance;
      insertHeapify(node, size++);
    }
  }

  NodeRecord pop()
  {
    NodeRecord nodeRecord(nodes[0], distanceMap.at(nodes[0]));
    swap(0, size - 1);
    heapIndexMap[nodes[size - 1]] = -1;
    distanceMap.erase(nodes[size - 1]);
    // free C++还要把原本堆顶节点析构
    nodes[size - 1] = nullptr;
    heapify(0, --size);
    return nodeRecord;
  }

  void insertHeapify(Node *node, int index)
  {
    while (distanceMap.at(nodes[index]) < distanceMap.at(nodes[(index - 1) / 2]))
    {
      swap(index, (index - 1) / 2);
      index = (index - 1) / 2;
    }
  }

  void heapify(int index, int size)
  {
    int left = index * 2 + 1;
    while (left < size)
    {
      int smallest = left + 1 < size && distanceMap.at(nodes[left + 1]) < distanceMap.at(nodes[left]) ? left + 1 : left;
      smallest = distanceMap.at(nodes[smallest]) < distanceMap.at(nodes[index]) ? smallest : index;
      if (smallest == index)
      {
        break;
      }
      swap(smallest, index);
      index = smallest;
      left = index * 2 + 1;
    }
  }

  bool isEntered(Node *node) { return heapIndexMap.find(node) != heapIndexMap.end(); }

  bool inHeap(Node *node) { return isEntered(node) && heapIndexMap.at(node) != -1; }
  void swap(int index1, int index2)
  {
    heapIndexMap[nodes[index1]] = index2;
    heapIndexMap[nodes[index2]] = index1;
    Node *tmp = nodes[index1];
    nodes[index1] = nodes[index2];
    nodes[index2] = tmp;
  }
};

// 改进后的dijkstra算法
// 从head出发，所有head能到达的节点，生成到达每个节点的最小路径记录并返回
// 将从Map中找出到未锁定目标点距离最短的点的时间复杂度从O(N)调整到O(logN)水平
unordered_map<Node *, int> dijkstra2(Node *head, int size)
{
  NodeHeap nodeHeap(size);
  nodeHeap.addOrUpdateOrIgnore(head, 0);
  unordered_map<Node *, int> result;
  while (!nodeHeap.isEmpty())
  {
    NodeRecord record = nodeHeap.pop();
    Node *cur = record.node;
    int distance = record.distance;
    for (Edge *edge : cur->edges)
    {
      nodeHeap.addOrUpdateOrIgnore(edge->to, edge->weight + distance);
    }
    result[cur] = distance;
  }
  return result;
}

class Graph
{
 public:
  // matrix 所有的边
  // N*3 的矩阵
  // [from节点上面的值，to节点上面的值，weight]
  //
  // [ 1, 3, 5 ]
  // [ 1, 4, 6 ]
  Graph(TdArray matrix)
  {
    for (int i = 0; i < matrix.size(); i++)
    {
      // 拿到每一条边， matrix[i]
      int from = matrix[i][0];
      int to = matrix[i][1];
      int weight = matrix[i][2];
      if (nodes.find(from) == nodes.end())
      {
        nodes.emplace(from, new Node(from));
      }
      if (nodes.find(to) == nodes.end())
      {
        nodes.emplace(to, new Node(to));
      }
      Node *fromNode = nodes.at(from);
      Node *toNode = nodes.at(to);
      Edge *newEdge = new Edge(weight, fromNode, toNode);
      fromNode->nexts.push_back(toNode);
      fromNode->out++;
      toNode->in++;
      fromNode->edges.push_back(newEdge);
      edges.emplace(newEdge);
    }
  }

 public:
  unordered_map<int, Node *> nodes;  // 图的所有节点
  unordered_set<Edge *> edges;       // 图的所有边
};

int main()
{
  TdArray sa{{1, 2, 3}, {1, 3, 50}, {2, 3, 2}, {3, 4, 10}};
  Graph g(sa);
  unordered_map<Node *, int> res = dijkstra1(g.nodes.at(1));

  cout << dijkstra1(g.nodes.at(1)).at(g.nodes.at(3)) << endl;
  cout << dijkstra2(g.nodes.at(1), g.nodes.size()).at(g.nodes.at(3)) << endl;
  return 0;
}
