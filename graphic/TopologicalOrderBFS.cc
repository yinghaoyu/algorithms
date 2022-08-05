#include <deque>
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
  list<DirectedGraphNode *> neighbors;

  DirectedGraphNode(int x) { label = x; }
};

// 有向图拓扑排序
list<DirectedGraphNode *> topSort(list<DirectedGraphNode *> graph)
{
  unordered_map<DirectedGraphNode *, int> indegreeMap;
  for (DirectedGraphNode *cur : graph)
  {
    indegreeMap.emplace(cur, 0);  // 初始时入度为0
  }
  for (DirectedGraphNode *cur : graph)  // 统计所有节点的入度
  {
    for (DirectedGraphNode *next : cur->neighbors)
    {
      indegreeMap[next]++;  // 入度 + 1
    }
  }
  deque<DirectedGraphNode *> zeroQueue;
  for (auto s : indegreeMap)
  {
    DirectedGraphNode *cur = s.first;
    if (indegreeMap.at(cur) == 0)  // 把入度为0的进队列
    {
      zeroQueue.push_back(cur);
    }
  }
  list<DirectedGraphNode *> ans;
  while (!zeroQueue.empty())
  {
    DirectedGraphNode *cur = zeroQueue.front();
    zeroQueue.pop_front();
    ans.push_back(cur);  // 取队头加入结果集
    for (DirectedGraphNode *next : cur->neighbors)
    {
      indegreeMap[next]--;            // 相邻节点的入度减1
      if (indegreeMap.at(next) == 0)  // 入度为0就加入队列
      {
        zeroQueue.push_back(next);  // next是非空指针
      }
    }
  }
  return ans;
}
