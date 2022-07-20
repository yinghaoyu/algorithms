#include <iostream>
#include <string>
#include <vector>

using namespace std;
using TdArray = vector<vector<int>>;

// 测试链接 : https://leetcode-cn.com/problems/21dk04/
class DistinctSubseq
{
 public:
  static int numDistinct1(string s, string t) { return process(s, t, s.length(), t.length()); }

  static int process(string s, string t, int i, int j)
  {
    if (j == 0)
    {
      return 1;
    }
    if (i == 0)
    {
      return 0;
    }
    int res = process(s, t, i - 1, j);
    if (s[i - 1] == t[j - 1])
    {
      res += process(s, t, i - 1, j - 1);
    }
    return res;
  }

  // S[...i]的所有子序列中，包含多少个字面值等于T[...j]这个字符串的子序列
  // 记为dp[i][j]
  // 可能性1）S[...i]的所有子序列中，都不以s[i]结尾，则dp[i][j]肯定包含dp[i-1][j]
  // 可能性2）S[...i]的所有子序列中，都必须以s[i]结尾，
  // 这要求S[i] == T[j]，则dp[i][j]包含dp[i-1][j-1]
  static int numDistinct2(string s, string t)
  {
    // dp[i][j] : s[0..i] T[0...j]

    // dp[i][j] : s只拿前i个字符做子序列，有多少个子序列，字面值等于T的前j个字符的前缀串
    TdArray dp(s.length() + 1, vector<int>(t.length() + 1));
    // dp[0][0]
    // dp[0][j] = s只拿前0个字符做子序列, T前j个字符
    for (int j = 0; j <= t.length(); j++)
    {
      dp[0][j] = 0;
    }
    for (int i = 0; i <= s.length(); i++)
    {
      dp[i][0] = 1;
    }
    for (int i = 1; i <= s.length(); i++)
    {
      for (int j = 1; j <= t.length(); j++)
      {
        dp[i][j] = dp[i - 1][j] + (s[i - 1] == t[j - 1] ? dp[i - 1][j - 1] : 0);
      }
    }
    return dp[s.length()][t.length()];
  }

  static int numDistinct3(string s, string t)
  {
    vector<int> dp(t.length() + 1);
    dp[0] = 1;
    for (int j = 1; j <= t.length(); j++)
    {
      dp[j] = 0;
    }
    for (int i = 1; i <= s.length(); i++)
    {
      for (int j = t.length(); j >= 1; j--)
      {
        dp[j] += s[i - 1] == t[j - 1] ? dp[j - 1] : 0;
      }
    }
    return dp[t.length()];
  }

  static int dp(string s, string t)
  {
    int N = s.length();
    int M = t.length();
    TdArray dp(N, vector<int>(M));
    // s[0..0] T[0..0] dp[0][0]
    dp[0][0] = s[0] == t[0] ? 1 : 0;
    for (int i = 1; i < N; i++)
    {
      dp[i][0] = s[i] == t[0] ? (dp[i - 1][0] + 1) : dp[i - 1][0];
    }
    for (int i = 1; i < N; i++)
    {
      for (int j = 1; j <= std::min(i, M - 1); j++)
      {
        dp[i][j] = dp[i - 1][j];
        if (s[i] == t[j])
        {
          dp[i][j] += dp[i - 1][j - 1];
        }
      }
    }
    return dp[N - 1][M - 1];
  }

  static void test()
  {
    string S = "1212311112121132";
    string T = "13";

    cout << numDistinct3(S, T) << endl;
    cout << dp(S, T) << endl;
  }
};

int main()
{
  DistinctSubseq::test();
  return 0;
}
