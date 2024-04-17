#include <vector>

using namespace std;

// 分组背包(模版)
// 给定一个正数m表示背包的容量，有n个货物可供挑选
// 每个货物有自己的体积(容量消耗)、价值(获得收益)、组号(分组)
// 同一个组的物品只能挑选1件，所有挑选物品的体积总和不能超过背包容量
// 怎么挑选货物能达到价值最大，返回最大的价值
// 测试链接 : https://www.luogu.com.cn/problem/P1757

//@sa https://www.bilibili.com/video/BV1UM411f7YL/
class PartitionedKnapsack
{
 public:
  static const int MAXN = 1001;

  static const int MAXM = 1001;

  // arr[i][0] i号物品的体积
  // arr[i][1] i号物品的价值
  // arr[i][2] i号物品的组号
  static int arr[MAXN][3];

  static int dp[MAXM];

  static int m, n;

  // 严格位置依赖的动态规划
  int compute1()
  {
    // 统计多少个组
    int teams = 1;
    for (int i = 2; i <= n; i++)
    {
      if (arr[i - 1][2] != arr[i][2])
      {
        teams++;
      }
    }
    // 组的编号1~teams
    // dp[i][j] : 1~i是组的范围，每个组的物品挑一件，容量不超过j的情况下，最大收益
    vector<vector<int>> dp(teams + 1, vector<int>(m + 1));
    // dp[0][....] = 0
    for (int start = 1, end = 2, i = 1; start <= n; i++)
    {
      // 获取这组一共多少件商品
      while (end <= n && arr[end][2] == arr[start][2])
      {
        end++;
      }
      // start ... end-1 -> i组
      for (int j = 0; j <= m; j++)
      {
        // arr[start...end-1]是当前组，组号一样
        // 其中的每一件商品枚举一遍
        dp[i][j] = dp[i - 1][j];
        for (int k = start; k < end; k++)
        {
          // k是组内的一个商品编号
          if (j - arr[k][0] >= 0)
          {
            dp[i][j] = std::max(dp[i][j], dp[i - 1][j - arr[k][0]] + arr[k][1]);
          }
        }
      }
      // start去往下一组的第一个物品
      // 继续处理剩下的组
      start = end++;
    }
    return dp[teams][m];
  }

  // 空间压缩
  int compute2()
  {
    // dp[0][...] = 0
    std::fill_n(dp, m + 1, 0);
    for (int start = 1, end = 2; start <= n;)
    {
      while (end <= n && arr[end][2] == arr[start][2])
      {
        end++;
      }
      // start....end-1
      for (int j = m; j >= 0; j--)
      {
        for (int k = start; k < end; k++)
        {
          if (j - arr[k][0] >= 0)
          {
            dp[j] = std::max(dp[j], arr[k][1] + dp[j - arr[k][0]]);
          }
        }
      }
      start = end++;
    }
    return dp[m];
  }
};
