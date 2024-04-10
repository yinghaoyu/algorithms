#include <vector>

using namespace std;

// 子序列累加和必须被7整除的最大累加和
// 给定一个非负数组nums，
// 可以任意选择数字组成子序列，但是子序列的累加和必须被7整除
// 返回最大累加和
class MaxSumDividedBy7
{
 public:
  // 暴力方法
  int maxSum1(vector<int>& nums)
  {
    // nums形成的所有子序列的累加和都求出来
    // 其中%7==0的那些累加和中，返回最大的
    // 就是如下f函数的功能
    return f(nums, 0, 0);
  }

  int f(vector<int>& nums, int i, int s)
  {
    if (i == nums.size())
    {
      return s % 7 == 0 ? s : 0;
    }
    return std::max(f(nums, i + 1, s), f(nums, i + 1, s + nums[i]));
  }

  // 正式方法
  // 时间复杂度O(n)
  int maxSum2(vector<int>& nums)
  {
    int n = nums.size();
    // dp[i][j] : nums[0...i-1]
    // nums前i个数形成的子序列一定要做到，子序列累加和%7 == j
    // 这样的子序列最大累加和是多少
    // 注意 : dp[i][j] == -1代表不存在这样的子序列
    vector<vector<int>> dp(n + 1, vector<int>(7));
    dp[0][0] = 0;
    for (int j = 1; j < 7; j++)
    {
      dp[0][j] = -1;
    }
    for (int i = 1, x, cur, need; i <= n; i++)
    {
      x = nums[i - 1];
      cur = nums[i - 1] % 7;
      for (int j = 0; j < 7; j++)
      {
        dp[i][j] = dp[i - 1][j];
        // 这里求need是核心
        need = cur <= j ? (j - cur) : (j - cur + 7);
        // 或者如下这种写法也对
        // need = (7 + j - cur) % 7;
        if (dp[i - 1][need] != -1)
        {
          dp[i][j] = std::max(dp[i][j], dp[i - 1][need] + x);
        }
      }
    }
    return dp[n][0];
  }
};
