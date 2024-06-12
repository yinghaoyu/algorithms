#include <string>
#include <vector>

using namespace std;

// 从动态规划中得到具体决策方案的技巧

// 先求最长公共子序列长度，再输出任意一种最长公共子序列
// @sa https://www.nowcoder.com/practice/4727c06b9ee9446cab2e859b4bb86bb8
// @sa https://www.bilibili.com/video/BV1L94y1w72w/
class LCS
{
 public:
  static const int MAXN = 5001;

  vector<vector<int>> dp = vector<vector<int>>(MAXN, vector<int>(MAXN));

  string ans;

  string s1;

  string s2;

  int n, m, k;

  void lcs()
  {
    getDp();
    k = dp[n][m];
    if (k > 0)
    {
      for (int len = k, i = n, j = m; len > 0;)
      {
        if (s1[i - 1] == s2[j - 1])
        {
          ans[--len] = s1[i - 1];
          i--;
          j--;
        }
        else
        {
          if (dp[i - 1][j] >= dp[i][j - 1])
          {
            i--;
          }
          else
          {
            j--;
          }
        }
      }
    }
  }

  // 填好dp表
  void getDp()
  {
    for (int i = 1; i <= n; i++)
    {
      for (int j = 1; j <= m; j++)
      {
        if (s1[i - 1] == s2[j - 1])
        {
          dp[i][j] = 1 + dp[i - 1][j - 1];
        }
        else
        {
          dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
        }
      }
    }
  }
};
