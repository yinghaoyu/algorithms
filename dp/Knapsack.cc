#include <iostream>
#include <vector>

using namespace std;
using TdArray = vector<vector<int>>;

// 背包问题
//
// 有n种物品，w数组表示物品重量、v数组表示物品的价值，
// 物品 i 的重量为 w[i]，价值为 v[i]，
// 假定所有物品的重量和价值都是非负的，背包所能承受的最大重量为bag，
// 在背包承重范围内，如何挑选物品使得价值最大，返回最大的价值。

class Knapsack
{
 public:
  static int maxValue(vector<int> &w, vector<int> &v, int bag)
  {
    if (w.size() != v.size() || w.size() == 0)
    {
      return 0;
    }
    // 尝试函数！
    return process(w, v, 0, bag);
  }

  // index 0~N
  // rest 负~bag
  static int process(vector<int> &w, vector<int> &v, int index, int rest)
  {
    if (rest < 0)
    {
      return -1;
    }
    if (index == w.size())
    {
      return 0;
    }
    // 不要当前位置的货物
    int p1 = process(w, v, index + 1, rest);
    // 要当前位置的货物
    int p2 = 0;
    // 需要判断当前背包减去当前货物后的容量，小于0则也不能要当前货物
    int next = process(w, v, index + 1, rest - w[index]);
    if (next != -1)
    {
      p2 = v[index] + next;
    }
    return std::max(p1, p2);
  }

  // 递归尝试是有重复调用过程的，例如：
  // 例如w = 5、2、3、4 v = 10、4、6、8
  // 0位置货物不要，1和2位置货物要，此时背包价值为10，准备决策3位置
  // 0位置货物要，1和2位置货物要，此时背包价值为10，准备决策3位置
  // 可见决策3位置后是重复调用的，这就给优化提供了空间，比如用缓存表。
  static int dp(vector<int> &w, vector<int> &v, int bag)
  {
    if (w.size() != v.size() || w.size() == 0)
    {
      return 0;
    }
    int N = w.size();
    // 根据可变参数index和bag定义动态规划数组
    TdArray dp(N + 1, vector<int>(bag + 1));
    // bag < 0 时，返回值-1表示无效值可以不用管，因为bag最小值就是0，在数组下标就不会小于0
    // index == wlen时，返回值0
    // 根据递归过程可知，当前index位置的值总是依赖index+1（下一行）位置的值，所以行index从下往上
    // 同一行的bag之间相互不依赖，所以从任意位置遍历bag（列）均可
    for (int index = N - 1; index >= 0; index--)
    {
      for (int rest = 0; rest <= bag; rest++)
      {
        int p1 = dp[index + 1][rest];
        int p2 = 0;
        int next = rest - w[index] < 0 ? -1 : dp[index + 1][rest - w[index]];
        if (next != -1)
        {
          p2 = v[index] + next;
        }
        dp[index][rest] = std::max(p1, p2);
      }
    }
    return dp[0][bag];
  }
};

int main()
{
  vector<int> weights = {3, 2, 4, 7, 3, 1, 7};
  vector<int> values = {5, 6, 3, 19, 12, 4, 2};
  int bag = 15;
  cout << Knapsack::maxValue(weights, values, bag) << endl;
  cout << Knapsack::dp(weights, values, bag) << endl;
  return 0;
}
