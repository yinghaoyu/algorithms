#include <algorithm>
#include <vector>

using namespace std;

// 有效涂色问题
// 给定n、m两个参数
// 一共有n个格子，每个格子可以涂上一种颜色，颜色在m种里选
// 当涂满n个格子，并且m种颜色都使用了，叫一种有效方法
// 求一共有多少种有效的涂色方法
// 1 <= n, m <= 5000
// 结果比较大请 % 1000000007 之后返回
class FillCellsUseAllColorsWays
{
 public:
  // 暴力方法
  int ways1(int n, int m)
  {
    vector<int> path(n);
    vector<bool> set(m);
    return f(path, set, 0, n, m);
  }

  // 把所有填色的方法暴力枚举
  // 然后一个一个验证是否有效
  // 这是一个带路径的递归
  // 无法改成动态规划
  int f(vector<int>& path, vector<bool> set, int i, int n, int m)
  {
    if (i == n)
    {
      std::fill(set.begin(), set.end(), false);
      int colors = 0;
      for (int c : path)
      {
        if (!set[c])
        {
          set[c] = true;
          colors++;
        }
      }
      return colors == m ? 1 : 0;
    }
    else
    {
      int ans = 0;
      for (int j = 1; j <= m; j++)
      {
        path[i] = j;
        ans += f(path, set, i + 1, n, m);
      }
      return ans;
    }
  }

  // 动态规划
  int ways2(int n, int m)
  {
    static const int mod = 1e9 + 7;
    vector<vector<int>> dp(n, vector<int>(m));
    // dp[i][j]:
    // 一共有m种颜色
    // 前i个格子涂满j种颜色的方法数
    for (int i = 1; i <= n; i++)
    {
      dp[i][1] = m;
    }
    for (int i = 2; i <= n; i++)
    {
      for (int j = 2; j <= m; j++)
      {
        // 前 i - 1 个格子已经使用了 j 种颜色，那么第 i 个格子重复使用这 j 种颜色的其中一种
        dp[i][j] = (int) (((long) dp[i - 1][j] * j) % mod);
        // 前 i - 1 个格子已经使用了 j - 1 种颜色，那么第 i 个格子使用剩余 m - (j - 1) 种颜色的其中一种
        dp[i][j] = (int) ((((long) dp[i - 1][j - 1] * (m - j + 1)) + dp[i][j]) % mod);
      }
    }
    return dp[n][m];
  }
};
