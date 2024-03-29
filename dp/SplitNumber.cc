#include <iostream>
#include <random>
#include <vector>

using namespace std;
using TdArray = vector<vector<int>>;

// 问题描述：
// 给定一个正数n，求n的裂开方法数。
// 规定：后面的数不能比前面的数小。
// 比如4的裂开方法有： 1+1+1+1、1+1+2、1+3、2+2、4，总共5种，所以返回5。

class SplitNumber
{
 public:
  // n为正数
  static int ways(int n)
  {
    if (n < 0)
    {
      return 0;
    }
    if (n == 1)
    {
      return 1;
    }
    return process(1, n);
  }

  // 上一个拆出来的数是pre
  // 还剩rest需要去拆
  // 返回拆解的方法数
  static int process(int pre, int rest)
  {
    if (rest == 0)  // 成功拆解
    {
      return 1;
    }
    if (pre > rest)  // 不符合递增
    {
      return 0;
    }
    int ways = 0;
    for (int first = pre; first <= rest; first++)  // first初值是pre才满足递增
    {
      ways += process(first, rest - first);
    }
    return ways;
  }

  static int dp1(int n)
  {
    if (n < 0)
    {
      return 0;
    }
    if (n == 1)
    {
      return 1;
    }
    TdArray dp(n + 1, vector<int>(n + 1));
    for (int pre = 1; pre <= n; pre++)
    {
      dp[pre][0] = 1;    // 拼完
      dp[pre][pre] = 1;  // pre == rest，此时rest不能再分
    }
    for (int pre = n - 1; pre >= 1; pre--)
    {
      for (int rest = pre + 1; rest <= n; rest++)
      {
        int ways = 0;
        for (int first = pre; first <= rest; first++)
        {
          ways += dp[first][rest - first];
        }
        dp[pre][rest] = ways;
      }
    }
    return dp[1][n];
  }

  static int dp2(int n)
  {
    if (n < 0)
    {
      return 0;
    }
    if (n == 1)
    {
      return 1;
    }
    TdArray dp(n + 1, vector<int>(n + 1));
    for (int pre = 1; pre <= n; pre++)
    {
      dp[pre][0] = 1;
      dp[pre][pre] = 1;
    }
    for (int pre = n - 1; pre >= 1; pre--)
    {
      for (int rest = pre + 1; rest <= n; rest++)
      {
        dp[pre][rest] = dp[pre + 1][rest];
        dp[pre][rest] += dp[pre][rest - pre];
      }
    }
    return dp[1][n];
  }

  static void test()
  {
    int test = 39;
    cout << ways(test) << endl;
    cout << dp1(test) << endl;
    cout << dp2(test) << endl;
  }
};

int main()
{
  SplitNumber::test();
  return 0;
}
