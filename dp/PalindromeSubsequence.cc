#include <iostream>
#include <stdbool.h>
#include <string>
#include <string.h>

using namespace std;

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
    static int lpsl1(string s)
    {
      if (s.length() == 0)
      {
        return 0;
      }
      const char* str = s.c_str();
      int len = strlen(str);
      return f(str, 0, len - 1);
    }

    // str[L..R]最长回文子序列长度返回
    static int f(const char* str, int L, int R)
    {
      if (L == R) // 只有一个字符
      {
        return 1;
      }
      if (L == R - 1) // 两个字符
      {
        return str[L] == str[R] ? 2 : 1;
      }
      int p1 = f(str, L + 1, R - 1);  // 排除最左边、最右边
      int p2 = f(str, L, R - 1);  // 排除最右边
      int p3 = f(str, L + 1, R);  // 排除最左边
      int p4 = str[L] != str[R] ? 0 : (2 + f(str, L + 1, R - 1));  // 包含最左最右
      return std::max(std::max(p1, p2), std::max(p3, p4));
    }

    static int lpsl2(string s)
    {
      if (s.length() == 0)
      {
        return 0;
      }
      const char* str = s.c_str();
      int N = strlen(str);
      int** dp = (int**)malloc(sizeof(int*) * N);
      for(int i = 0; i < N; i++)
      {
        dp[i] = (int*)malloc(sizeof(int) * N);
        memset(dp[i], 0, sizeof(int) * N);
      }
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
      int ans = dp[0][N - 1];
      free(dp);
      return ans;
    }

    static int longestPalindromeSubseq1(string s)
    {
      if (s.length() == 0)
      {
        return 0;
      }

      if (s.length() == 1)
      {
        return 1;
      }

      const char* str = s.c_str();
      const char* rev = reverse(str);
      // 最长回文子序列就是该字符串与反转后的字符串的最长公共子序列
      return longestCommonSubsequence(str, rev);
    }

    static const char* reverse(const char* str)
    {
      int N = strlen(str);
      char* reverse = new char[N];
      for (int i = 0; i < N; i++)
      {
        reverse[--N] = str[i];
      }
      return reverse;
    }

    static int longestCommonSubsequence(const char* str1, const char* str2)
    {
      int N = strlen(str1);
      int M = strlen(str2);
      int** dp = (int**)malloc(sizeof(int*) * N);
      for(int i = 0; i < N; i++)
      {
        dp[i] = (int*)malloc(sizeof(int) * M);
        memset(dp[i], 0, sizeof(int) * M);
      }
      dp[0][0] = str1[0] == str2[0] ? 1 : 0;
      for (int i = 1; i < N; i++)
      {
        dp[i][0] = str1[i] == str2[0] ? 1 : dp[i - 1][0];
      }
      for (int j = 1; j < M; j++)
      {
        dp[0][j] = str1[0] == str2[j] ? 1 : dp[0][j - 1];
      }
      for (int i = 1; i < N; i++)
      {
        for (int j = 1; j < M; j++)
        {
          dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
          if (str1[i] == str2[j])
          {
            dp[i][j] = std::max(dp[i][j], dp[i - 1][j - 1] + 1);
          }
        }
      }
      return dp[N - 1][M - 1];
    }

    static int longestPalindromeSubseq2(string s)
    {
      if (s.length() == 0)
      {
        return 0;
      }
      if (s.length() == 1)
      {
        return 1;
      }
      const char* str = s.c_str();
      int N = strlen(str);
      int** dp = (int**)malloc(sizeof(int*) * N);
      for(int i = 0; i < N; i++)
      {
        dp[i] = (int*)malloc(sizeof(int) * N);
        memset(dp[i], 0, sizeof(int) * N);
      }
      dp[N - 1][N - 1] = 1;
      for (int i = 0; i < N - 1; i++)
      {
        dp[i][i] = 1;
        dp[i][i + 1] = str[i] == str[i + 1] ? 2 : 1;
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
      return dp[0][N - 1];
    }
};

int main()
{
  string str = "";
  cout << PalindromeSubsequence::lpsl1(str) << endl;
  return 0;
}
