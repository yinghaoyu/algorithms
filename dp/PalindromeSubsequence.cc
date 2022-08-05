#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using TdArray = vector<vector<int>>;

// 测试链接：https://leetcode.com/problems/longest-palindromic-subsequence/

// 最长回文子序列
// 给你一个字符串 s ，找出其中最长的回文子序列，并返回该序列的长度。
// 子序列定义为：不改变剩余字符顺序的情况下，删除某些字符或者不删除任何字符形成的一个序列
// 示例 1：
// 输入：s = "bbbab"
// 输出：4
// 解释：一个可能的最长回文子序列为 "bbbb" 。

// 测试链接：https://leetcode.cn/problems/longest-common-subsequence/

// 最长公共子序列
// 给定两个字符串 text1 和 text2，返回这两个字符串的最长 公共子序列 的长度。如果不存在 公共子序列 ，返回 0 。
// 一个字符串的 子序列 是指这样一个新的字符串：它是由原字符串在不改变字符的相对顺序的情况下删除某些字符（也可以不删除任何字符）后组成的新字符串。
//例如，"ace" 是 "abcde" 的子序列，但 "aec" 不是 "abcde" 的子序列。
//两个字符串的 公共子序列 是这两个字符串所共同拥有的子序列。

class PalindromeSubsequence
{
 public:
  static int lpsl1(string &s)
  {
    if (s.length() == 0)
    {
      return 0;
    }
    return f(s, 0, s.length() - 1);
  }

  // str[L..R]最长回文子序列长度返回
  static int f(string &str, int L, int R)
  {
    if (L == R)  // 只有一个字符
    {
      return 1;
    }
    if (L == R - 1)  // 两个字符
    {
      return str[L] == str[R] ? 2 : 1;
    }
    int p1 = f(str, L + 1, R - 1);                               // 排除最左边、最右边
    int p2 = f(str, L, R - 1);                                   // 排除最右边
    int p3 = f(str, L + 1, R);                                   // 排除最左边
    int p4 = str[L] != str[R] ? 0 : (2 + f(str, L + 1, R - 1));  // 包含最左最右
    return std::max(std::max(p1, p2), std::max(p3, p4));
  }

  static int lpsl2(string &str)
  {
    if (str.length() == 0)
    {
      return 0;
    }
    int N = str.length();
    TdArray dp(N, vector<int>(N));
    dp[N - 1][N - 1] = 1;
    for (int i = 0; i < N - 1; i++)
    {
      // base case
      dp[i][i] = 1;
      dp[i][i + 1] = str[i] == str[i + 1] ? 2 : 1;
    }
    // L <= R 说明dp表有一半区域无效
    for (int L = N - 3; L >= 0; L--)
    {
      for (int R = L + 2; R < N; R++)
      {
        dp[L][R] = std::max(dp[L][R - 1], dp[L + 1][R]);
        if (str[L] == str[R])
        {
          dp[L][R] = std::max(dp[L][R], 2 + dp[L + 1][R - 1]);
        }
      }
    }
    return dp[0][N - 1];
  }

  static int longestPalindromeSubseq1(string &s)
  {
    if (s.length() == 0)
    {
      return 0;
    }

    if (s.length() == 1)
    {
      return 1;
    }

    string rev = s;
    std::reverse(rev.begin(), rev.end());
    // 最长回文子序列就是该字符串与反转后的字符串的最长公共子序列
    return longestCommonSubsequence(s, rev);
  }

  // 其中 dp[i][j] 表示 str1[0:i] 和 str2[0:j] 的最长公共子序列的长度
  static int longestCommonSubsequence(string &str1, string &str2)
  {
    int N = str1.length();
    int M = str2.length();
    TdArray dp(N, vector<int>(M));
    // base case
    // str1[0:i]  str2[0:j] 都只有一个字符，相等就长度为1
    dp[0][0] = str1[0] == str2[0] ? 1 : 0;
    for (int i = 1; i < N; i++)
    {
      // str2[0:j] 只有一个字符
      dp[i][0] = str1[i] == str2[0] ? 1 : dp[i - 1][0];
    }
    for (int j = 1; j < M; j++)
    {
      // str[0:i] 只有一个字符
      dp[0][j] = str1[0] == str2[j] ? 1 : dp[0][j - 1];
    }
    for (int i = 1; i < N; i++)
    {
      for (int j = 1; j < M; j++)
      {
        // str1[0:i] 和 str2[0:j] 依赖于 str1[0:i-1] 和 str2[0:j-1]
        dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
        // 如果新加入的字符相等，则在原来的dp[i-1][j-1]基础上加上+1
        if (str1[i] == str2[j])
        {
          dp[i][j] = std::max(dp[i][j], dp[i - 1][j - 1] + 1);
        }
      }
    }
    return dp[N - 1][M - 1];
  }

  static int longestPalindromeSubseq2(string str)
  {
    if (str.length() == 0)
    {
      return 0;
    }
    if (str.length() == 1)
    {
      return 1;
    }
    int N = str.length();
    TdArray dp(N, vector<int>(N));
    dp[N - 1][N - 1] = 1;
    for (int i = 0; i < N - 1; i++)
    {
      dp[i][i] = 1;                                 // 只有一个字符
      dp[i][i + 1] = str[i] == str[i + 1] ? 2 : 1;  // 两个字符相等就有2种回文子序列
    }
    for (int i = N - 3; i >= 0; i--)
    {
      for (int j = i + 2; j < N; j++)
      {
        dp[i][j] = std::max(dp[i][j - 1], dp[i + 1][j]);
        if (str[i] == str[j])
        {
          dp[i][j] = std::max(dp[i][j], dp[i + 1][j - 1] + 2);
        }
      }
    }
    return dp[0][N - 1];  // 整个子串的最长回文子序列
  }
};

int main()
{
  string str = "bbbab";
  cout << PalindromeSubsequence::lpsl1(str) << endl;
  cout << PalindromeSubsequence::lpsl2(str) << endl;
  cout << PalindromeSubsequence::longestPalindromeSubseq1(str) << endl;
  cout << PalindromeSubsequence::longestPalindromeSubseq2(str) << endl;
  return 0;
}
