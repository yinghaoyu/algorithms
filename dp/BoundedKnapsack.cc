#include <vector>

using namespace std;

// 多重背包不进行枚举优化
// 宝物筛选
// 一共有n种货物, 背包容量为t
// 每种货物的价值(v[i])、重量(w[i])、数量(c[i])都给出
// 请返回选择货物不超过背包容量的情况下，能得到的最大的价值

// @sa https://www.bilibili.com/video/BV1Nz4y1c71M/
class BoundedKnapsack
{
 public:
  static const int MAXN = 101;

  static const int MAXW = 40001;

  static int v[MAXN];

  static int w[MAXN];

  static int c[MAXN];

  static int dp[MAXW];

  static int n, t;

  // 严格位置依赖的动态规划
  // 时间复杂度O(n * t * 每种商品的平均个数)
  int compute1()
  {
    // dp[0][....] = 0，表示没有货物的情况下，背包容量不管是多少，最大价值都是0
    vector<vector<int>> dp(n + 1, vector<int>(t + 1));
    for (int i = 1; i <= n; i++)
    {
      for (int j = 0; j <= t; j++)
      {
        dp[i][j] = dp[i - 1][j];
        for (int k = 1; k <= c[i] && w[i] * k <= j; k++)
        {
          dp[i][j] = std::max(dp[i][j], dp[i - 1][j - k * w[i]] + k * v[i]);
        }
      }
    }
    return dp[n][t];
  }

  // 空间压缩
  // 部分测试用例超时
  // 因为没有优化枚举
  // 时间复杂度O(n * t * 每种商品的平均个数)
  int compute2()
  {
    for (int i = 1; i <= n; i++)
    {
      for (int j = t; j >= 0; j--)
      {
        for (int k = 1; k <= c[i] && w[i] * k <= j; k++)
        {
          dp[j] = std::max(dp[j], dp[j - k * w[i]] + k * v[i]);
        }
      }
    }
    return dp[t];
  }
};
