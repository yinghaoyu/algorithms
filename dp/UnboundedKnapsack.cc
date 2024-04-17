#include <vector>

using namespace std;

// 完全背包(模版)
// 给定一个正数t，表示背包的容量
// 有m种货物，每种货物可以选择任意个
// 每种货物都有体积costs[i]和价值values[i]
// 返回在不超过总容量的情况下，怎么挑选货物能达到价值最大
// 返回最大的价值
// 测试链接 : https://www.luogu.com.cn/problem/P1616
// @sa https://www.bilibili.com/video/BV1UM411f7YL/
class UnboundedKnapsack
{
 public:
  static const int MAXM = 10001;

  static const int MAXT = 10000001;

  static int cost[MAXM];

  static int val[MAXM];

  static long dp[MAXT];

  static int t, m;

  // 严格位置依赖的动态规划
  // 会空间不够，导致无法通过全部测试用例
  long compute1()
  {
    // dp[0][.....] = 0
    vector<vector<int>> dp(m + 1, vector<int>(t + 1));
    for (int i = 1; i <= m; i++)
    {
      for (int j = 0; j <= t; j++)
      {
        dp[i][j] = dp[i - 1][j];
        if (j - cost[i] >= 0)
        {
          dp[i][j] = std::max(dp[i][j], dp[i /*注意这里取的还是i，表示还可以再取当前商品*/][j - cost[i]] + val[i]);
        }
      }
    }
    return dp[m][t];
  }

  // 空间压缩
  // 可以通过全部测试用例
  long compute2()
  {
    std::fill_n(dp, t + 1, 0);
    for (int i = 1; i <= m; i++)
    {
      for (int j = cost[i]; j <= t; j++)
      {
        dp[j] = std::max(dp[j], dp[j - cost[i]] + val[i]);
      }
    }
    return dp[t];
  }
};
