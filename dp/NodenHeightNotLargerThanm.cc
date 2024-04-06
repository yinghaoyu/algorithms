#include <vector>

using namespace std;

// 节点数为n高度不大于m的二叉树个数
// 现在有n个节点，计算出有多少个不同结构的二叉树
// 满足节点个数为n且树的高度不超过m的方案
// 因为答案很大，所以答案需要模上1000000007后输出
// 测试链接 : https://www.nowcoder.com/practice/aaefe5896cce4204b276e213e725f3ea
// @sa https://www.bilibili.com/video/BV1WQ4y1W7d1/
class NodenHeightNotLargerThanm
{
  static const int MOD = 1e9 + 7;

 public:
  int f1(int n, int m, vector<vector<int>>& dp)
  {
    if (n == 0)
    {
      return 1;
    }
    // n > 0
    if (m == 0)
    {
      return 0;
    }
    if (dp[n][m] != -1)
    {
      return (int) dp[n][m];
    }
    long ans = 0;
    // n个点，头占掉1个
    for (int k = 0; k < n; k++)
    {
      // 一共n个节点，头节点已经占用了1个名额
      // 如果左树占用k个，那么右树就占用i-k-1个
      ans = (ans + ((long) f1(k, m - 1, dp) * f1(n - k - 1, m - 1, dp)) % MOD) % MOD;
    }
    dp[n][m] = ans;
    return (int) ans;
  }

  int nodenHeightNotLargerThanm1(int n, int m)
  {
    vector<vector<int>> dp(n, vector<int>(m, -1));
    return f1(n, m, dp);
  }

  // 动态规划，严格位置依赖
  int nodenHeightNotLargerThanm2(int n, int m)
  {
    vector<vector<int>> dp(n, vector<int>(m, -1));
    for (int j = 0; j <= m; j++)
    {
      dp[0][j] = 1;
    }
    for (int i = 1; i <= n; i++)
    {
      for (int j = 1; j <= m; j++)
      {
        dp[i][j] = 0;
        for (int k = 0; k < i; k++)
        {
          // 一共i个节点，头节点已经占用了1个名额
          // 如果左树占用k个，那么右树就占用i-k-1个
          dp[i][j] = (dp[i][j] + dp[k][j - 1] * dp[i - k - 1][j - 1] % MOD) % MOD;
        }
      }
    }
    return (int) dp[n][m];
  }

  // 动态规划，空间压缩
  int nodenHeightNotLargerThanm3(int n, int m)
  {
    // 按列从左到右更新
    vector<int> dp(n);
    dp[0] = 1;
    for (int i = 1; i <= n; i++)
    {
      dp[i] = 0;
    }
    for (int j = 1; j <= m; j++)
    {
      // 根据依赖，一定要先枚举列
      for (int i = n; i >= 1; i--)
      {
        // 再枚举行，而且i不需要到达0，i>=1即可
        dp[i] = 0;
        for (int k = 0; k < i; k++)
        {
          // 枚举
          dp[i] = (dp[i] + dp[k] * dp[i - k - 1] % MOD) % MOD;
        }
      }
    }
    return (int) dp[n];
  }
};
