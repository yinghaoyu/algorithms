#include <string>
#include <vector>

using namespace std;

// 删除至少几个字符可以变成另一个字符串的子串
// 给定两个字符串s1和s2
// 返回s1至少删除多少字符可以成为s2的子串
// @sa https://www.bilibili.com/video/BV1cg4y1o719/
class MinimumDeleteBecomeSubstring
{
 public:
  int minDelete(string s1, string s2)
  {
    int n = s1.length();
    int m = s2.length();
    // dp[len1][len2] :
    // s1[前缀长度为i]至少删除多少字符，可以变成s2[前缀长度为j]的任意后缀串
    vector<vector<int>> dp(n + 1, vector<int>(m + 1));
    for (int i = 1; i <= n; i++)
    {
      dp[i][0] = i;
      for (int j = 1; j <= m; j++)
      {
        if (s1[i - 1] == s2[j - 1])
        {
          dp[i][j] = dp[i - 1][j - 1];
        }
        else
        {
          dp[i][j] = dp[i - 1][j] + 1;
        }
      }
    }
    int ans = INT32_MAX;
    for (int j = 0; j <= m; j++)
    {
      ans = std::min(ans, dp[n][j]);
    }
    return ans;
  }
};
