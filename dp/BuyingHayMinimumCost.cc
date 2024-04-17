#include <stdint.h>
#include <algorithm>
#include <vector>

using namespace std;

// 购买足量干草的最小花费
// 有n个提供干草的公司，每个公司都有两个信息
// cost[i]代表购买1次产品需要花的钱
// val[i]代表购买1次产品所获得的干草数量
// 每个公司的产品都可以购买任意次
// 你一定要至少购买h数量的干草，返回最少要花多少钱

// 完全背包问题
// @sa https://www.bilibili.com/video/BV1UM411f7YL/
class Code06_BuyingHayMinimumCost
{
 public:
  static const int MAXN = 101;

  static const int MAXM = 55001;

  static int val[MAXN];

  static int cost[MAXN];

  static int dp[MAXM];

  static int n, h, maxv, m;

  // dp[i][j] : 1...i里挑公司，购买严格j磅干草，需要的最少花费
  // 1) dp[i-1][j]
  // 2) dp[i][j-val[i]] + cost[i]
  // 两种可能性中选最小
  // 但是关于j需要进行一定的扩充，原因视频里讲了
  int compute1()
  {
    // 最核心的一句
    // 包含重要分析
    m = h + *max_element(cost, cost + MAXN);
    vector<vector<int>> dp(n + 1, vector<int>(m + 1));
    std::fill_n(dp[0], m + 1, INT32_MAX);
    for (int i = 1; i <= n; i++)
    {
      for (int j = 0; j <= m; j++)
      {
        dp[i][j] = dp[i - 1][j];
        if (j - val[i] >= 0 && dp[i][j - val[i]] != INT32_MAX)
        {
          dp[i][j] = std::min(dp[i][j], dp[i][j - val[i]] + cost[i]);
        }
      }
    }
    int ans = INT32_MAX;
    // >= h
    // h h+1 h+2 ... m
    for (int j = h; j <= m; j++)
    {
      ans = std::min(ans, dp[n][j]);
    }
    return ans;
  }

  // 空间压缩
  int compute2()
  {
    std::fill_n(dp, m + 1, INT32_MAX);
    for (int i = 1; i <= n; i++)
    {
      for (int j = val[i]; j <= m; j++)
      {
        if (dp[j - val[i]] != INT32_MAX)
        {
          dp[j] = std::min(dp[j], dp[j - val[i]] + cost[i]);
        }
      }
    }
    int ans = INT32_MAX;
    for (int j = h; j <= m; j++)
    {
      ans = std::min(ans, dp[j]);
    }
    return ans;
  }
};
