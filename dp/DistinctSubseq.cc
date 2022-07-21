#include <iostream>
#include <string>
#include <vector>

using namespace std;
using TdArray = vector<vector<int>>;

// 问题描述：
// 给定一个字符串 s 和一个字符串 t ，计算在 s 的子序列中 t 出现的个数。
// 字符串的一个 子序列 是指，通过删除一些（也可以不删除）字符且不干扰剩余字符相对位置所组成的新字符串。（例如，"ACE" 是 "ABCDE" 的一个子序列，而 "AEC" 不是）

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
    // dp[i][j]的含义为，任取s中0...i位置的子序列，有多少能组成前缀为0...j的t
    // s[0..0] T[0..0] dp[0][0]
    dp[0][0] = s[0] == t[0] ? 1 : 0;  // 只有一个字符，相等的时候有1种方法
    for (int i = 1; i < N; i++)
    {
      // s有i个字符，t只有1个字符，只要新的字符和t[0]相等，方法数一直递增
      // 比如s="aaa"，t="a"
      // dp[0][0] = 1，dp[1][0] = 2，dp[2][0]=3
      dp[i][0] = s[i] == t[0] ? (dp[i - 1][0] + 1) : dp[i - 1][0];
    }
    for (int i = 1; i < N; i++)
    {
      // 当s长度小于t时，s怎么都不能组成t，因此i > j时，dp[i][j] = 0，这里取i和M-1是为了防止越界
      for (int j = 1; j <= std::min(i, M - 1); j++)
      {
        // 情况1：0...i-1位置s的子序列就可能组成前缀0...j的t
        dp[i][j] = dp[i - 1][j];
        // 情况2：s[i]和t[j]字符相同，那么只要0...i-1位置s的子序列可以组成0...j-1的t
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
