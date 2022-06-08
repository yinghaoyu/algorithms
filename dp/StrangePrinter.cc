#include <iostream>
#include <string>
#include <vector>

using namespace std;

using TdArray = vector<vector<int>>;

// 本题测试链接 : https://leetcode.com/problems/strange-printer/
//
// 题目描述：
// 有台奇怪的打印机有以下两个特殊要求：

// 1、打印机每次只能打印由 同一个字符 组成的序列。
// 2、每次可以在从起始到结束的任意位置打印新字符，并且会覆盖掉原来已有的字符。
// 给你一个字符串 s ，你的任务是计算这个打印机打印它需要的最少打印次数。

// 示例 1：
// 输入：s = "aaabbb"
// 输出：2
// 解释：首先打印 "aaa" 然后打印 "bbb"。

// 示例 2：
// 输入：s = "aba"
// 输出：2
// 解释：首先打印 "aaa" 然后在第二个位置打印 "b" 覆盖掉原来的字符 'a'。

class StrangePrinter
{
 public:
  static int strangePrinter1(string str)
  {
    if (str.length() == 0)
    {
      return 0;
    }
    return process1(str, 0, str.length() - 1);
  }

  // 要想刷出str[L...R]的样子！
  // 返回最少的转数
  static int process1(string str, int L, int R)
  {
    if (L == R)  // base case
    {
      // 只有一个字符，打印一次就够了
      return 1;
    }
    // L...R
    int ans = R - L + 1;  // 至多要打印的次数
    for (int k = L + 1; k <= R; k++)
    {
      // L...k-1 k....R
      // 按照规律aba，分成ab和a，ab两次，a一次，实际减去1次
      ans = std::min(ans, process1(str, L, k - 1) + process1(str, k, R) - (str[L] == str[k] ? 1 : 0));
    }
    return ans;
  }

  static int strangePrinter2(string str)
  {
    if (str.length() == 0)
    {
      return 0;
    }
    int N = str.length();
    TdArray dp(N, vector<int>(N));
    return process2(str, 0, N - 1, dp);
  }

  // 记忆化搜索
  static int process2(string str, int L, int R, TdArray &dp)
  {
    if (dp[L][R] != 0)
    {
      return dp[L][R];
    }
    int ans = R - L + 1;
    if (L == R)
    {
      ans = 1;
    }
    else
    {
      for (int k = L + 1; k <= R; k++)
      {
        ans = std::min(ans, process2(str, L, k - 1, dp) + process2(str, k, R, dp) - (str[L] == str[k] ? 1 : 0));
      }
    }
    dp[L][R] = ans;
    return ans;
  }

  // 动态规划
  static int strangePrinter3(string str)
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
      dp[i][i] = 1;
      dp[i][i + 1] = str[i] == str[i + 1] ? 1 : 2;
    }
    for (int L = N - 3; L >= 0; L--)
    {
      for (int R = L + 2; R < N; R++)
      {
        dp[L][R] = R - L + 1;
        for (int k = L + 1; k <= R; k++)
        {
          dp[L][R] = std::min(dp[L][R], dp[L][k - 1] + dp[k][R] - (str[L] == str[k] ? 1 : 0));
        }
      }
    }
    return dp[0][N - 1];
  }

  static void test()
  {
    string str = "aba";
    cout << strangePrinter1(str) << endl;
    cout << strangePrinter2(str) << endl;
    cout << strangePrinter3(str) << endl;
  }
};

int main()
{
  StrangePrinter::test();
  return 0;
}
