#include <iostream>
#include <list>
#include <unordered_map>

using namespace std;

// OJ链接：https://www.lintcode.com/problem/topological-sorting

// 不要提交这个类
class DirectedGraphNode
{
public:
  int label;
  list<DirectedGraphNode*> neighbors;

  DirectedGraphNode(int x)
  {
    label = x;
  }
};

// 提交下面的
class Record
{
public:
  DirectedGraphNode* node;
  long nodes;

  Record(DirectedGraphNode* n, long o)
  {
    node = n;
    nodes = o;
  }
};

bool compare(Record o1, Record o2)
{
  return o1.nodes > o2.nodes;
}

// 当前来到cur点，请返回cur点所到之处，所有的点次！
// 返回（cur，点次）
// 缓存！！！！！order
//  key : 某一个点的点次，之前算过了！
//  value : 点次是多少
Record f(DirectedGraphNode* cur, unordered_map<DirectedGraphNode*, Record> order)
{
  if (order.find(cur) != order.end())
  {
    return order.at(cur);
  }
  // cur的点次之前没算过！
  long nodes = 0;
  for (DirectedGraphNode* next : cur->neighbors)
  {
    nodes += f(next, order).nodes;  // 这里统计的是累加，和计算节点最大深度异曲同工
  }
  Record ans(cur, nodes + 1);
  order[cur] = ans;
  return ans;
}

list<DirectedGraphNode*> topSort(list<DirectedGraphNode*>& graph)
{
  unordered_map<DirectedGraphNode*, Record> order;
  for (DirectedGraphNode* cur : graph)
  {
    f(cur, order);
  }
  list<Record> recordArr;
  for (auto r : order)
  {
    recordArr.push_back(r.second);
  }
  recordArr.sort(compare);
  list<DirectedGraphNode*> ans;
  for (auto r : recordArr)
  {
    ans.push_back(r.node);
  }
  return ans;
}
