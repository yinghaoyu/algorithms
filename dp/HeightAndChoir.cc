#include <vector>

using namespace std;

// 区间dp

// 合唱队
// 测试链接 : https://www.luogu.com.cn/problem/P3205
class HeightAndChoir
{
 public:
  static const int MAXN = 1001;

  static int nums[MAXN];

  static int dp[MAXN][2];

  static int n;

  static const int MOD = 19650827;

  // 时间复杂度O(n^2)
  // 严格位置依赖的动态规划
  int compute1()
  {
    // 人的编号范围 : 1...n
    // dp[l][r][0] : 形成l...r的状况的方法数，同时要求l位置的数字是最后出现的
    // dp[l][r][1] : 形成l...r的状况的方法数，同时要求r位置的数字是最后出现的
    vector<vector<vector<int>>> dp(n + 1, vector<vector<int>>(n + 1, vector<int>(2)));
    for (int i = 1; i < n; i++)
    {
      if (nums[i] < nums[i + 1])
      {
        dp[i][i + 1][0] = 1;
        dp[i][i + 1][1] = 1;
      }
    }
    for (int l = n - 2; l >= 1; l--)
    {
      for (int r = l + 2; r <= n; r++)
      {
        if (nums[l] < nums[l + 1])
        {
          dp[l][r][0] = (dp[l][r][0] + dp[l + 1][r][0]) % MOD;
        }
        if (nums[l] < nums[r])
        {
          dp[l][r][0] = (dp[l][r][0] + dp[l + 1][r][1]) % MOD;
        }
        if (nums[r] > nums[l])
        {
          dp[l][r][1] = (dp[l][r][1] + dp[l][r - 1][0]) % MOD;
        }
        if (nums[r] > nums[r - 1])
        {
          dp[l][r][1] = (dp[l][r][1] + dp[l][r - 1][1]) % MOD;
        }
      }
    }
    return (dp[1][n][0] + dp[1][n][1]) % MOD;
  }

  // 时间复杂度O(n^2)
  // 空间压缩
  int compute2()
  {
    if (nums[n - 1] < nums[n])
    {
      dp[n][0] = 1;
      dp[n][1] = 1;
    }
    for (int l = n - 2; l >= 1; l--)
    {
      if (nums[l] < nums[l + 1])
      {
        dp[l + 1][0] = 1;
        dp[l + 1][1] = 1;
      }
      else
      {
        dp[l + 1][0] = 0;
        dp[l + 1][1] = 0;
      }
      for (int r = l + 2; r <= n; r++)
      {
        int a = 0;
        int b = 0;
        if (nums[l] < nums[l + 1])
        {
          a = (a + dp[r][0]) % MOD;
        }
        if (nums[l] < nums[r])
        {
          a = (a + dp[r][1]) % MOD;
        }
        if (nums[r] > nums[l])
        {
          b = (b + dp[r - 1][0]) % MOD;
        }
        if (nums[r] > nums[r - 1])
        {
          b = (b + dp[r - 1][1]) % MOD;
        }
        dp[r][0] = a;
        dp[r][1] = b;
      }
    }
    return (dp[n][0] + dp[n][1]) % MOD;
  }
};
