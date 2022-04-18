#include <iostream>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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
  int deep;  // 记录每个节点能达到的最大深度

  Record(DirectedGraphNode* n, int o)
  {
    node = n;
    deep = o;
  }
  //bool operator < (Record& other) // 重载<就是重载默认sort规则
  //{
  //  return this->deep < other.deep;
  //}
};

// 比较器
bool cmp(const Record& a, const Record& b)
{
  return a.deep > b.deep;
}

Record f(DirectedGraphNode* cur, unordered_map<DirectedGraphNode*, Record>& order)
{
  if (order.find(cur) != order.end())  // 节点已记录，就返回当前节点的记录
  {
    return order.at(cur);
  }
  int follow = 0;
  for (DirectedGraphNode* next : cur->neighbors)
  {
    follow = std::max(follow, f(next, order).deep);  // 取最大的深度
  }
  Record ans(cur, follow + 1);
  order[cur] = ans;
  return ans;
}

list<DirectedGraphNode*> topSort(list<DirectedGraphNode*>& graph)
{
  unordered_map<DirectedGraphNode*, Record> order;
  for (DirectedGraphNode* cur : graph)
  {
    f(cur, order);  // 得到所有节点能达到的深度
  }
  list<Record> recordArr;
  for (auto r : order)
  {
    recordArr.push_back(r.second);
  }
  recordArr.sort(cmp);  // 按深度从大到小排序，即为所得解
  list<DirectedGraphNode*> ans;
  for (auto r : recordArr)
  {
    ans.push_back(r.node);
  }
  return ans;
}

