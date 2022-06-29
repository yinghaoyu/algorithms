#include <iostream>
#include <queue>
#include <vector>

using namespace std;

// 问题描述：
// 给你 n 个项目。对于每个项目 i ，它都有一个纯利润 profits[i] ，和启动该项目需要的最小资本 capital[i] 。
// 最初，你的资本为 w 。当你完成一个项目时，你将获得纯利润，且利润将被添加到你的总资本中。
// 总而言之，从给定项目中选择 最多 k 个不同项目的列表，以最大化最终资本 ，并输出最终可获得的最多资本。

// 链接：https://leetcode-cn.com/problems/ipo

class IPO
{
 public:
  // 最多K个项目
  // W是初始资金
  // Profits[] Capital[] 一定等长
  // 返回最终最大的资金
  static int findMaximizedCapital(int K, int W, vector<int> &Profits, vector<int> &Capital)
  {
    priority_queue<Program, vector<Program>, Greater> minCostQ;  // 资金用小根堆
    priority_queue<Program, vector<Program>, Less> maxProfitQ;   // 利润用大根堆
    for (int i = 0; i < Profits.size(); i++)
    {
      minCostQ.emplace(Program(Profits[i], Capital[i]));
    }
    for (int i = 0; i < K; i++)  // 取出k个项目
    {
      while (!minCostQ.empty() && minCostQ.top().c <= W)
      {
        // 资金小于W的项目加入利润大根堆
        maxProfitQ.emplace(minCostQ.top());
        minCostQ.pop();
      }
      if (maxProfitQ.empty())
      {
        return W;
      }
      W += maxProfitQ.top().p;  // 利润累计
      maxProfitQ.pop();
    }
    return W;
  }

  class Program
  {
   public:
    int p;
    int c;

    Program()  // 为了兼容vector<Program>
    {
      this->p = 0;
      this->c = 0;
    }

    Program(int p, int c)
    {
      this->p = p;
      this->c = c;
    }
  };

  class Greater
  {
   public:
    bool operator()(const Program &l, const Program &r) { return l.c > r.c; }
  };

  class Less
  {
   public:
    bool operator()(const Program &l, const Program &r) { return l.p < r.p; }
  };
};

int main()
{
  vector<int> c{2, 1, 3, 4, 6, 5, 8};  // 资本
  vector<int> p{2, 1, 3, 4, 6, 5, 8};  // 利润
  int w = 3;
  int k = 4;
  cout << IPO::findMaximizedCapital(k, w, p, c) << endl;
  return 0;
}
