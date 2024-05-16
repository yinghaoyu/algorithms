#include <string>

using namespace std;
// 萌数
// 如果一个数字，存在长度至少为2的回文子串，那么这种数字被称为萌数
// 比如101、110、111、1234321、45568
// 求[l,r]范围上，有多少个萌数
// 由于答案可能很大，所以输出答案对1000000007求余
// 测试链接 : https://www.luogu.com.cn/problem/P3413
// @sa https://www.bilibili.com/video/BV1uC4y1Q72c/

class MengNumber
{
 public:
  static const int MOD = 1e9 + 7;

  static const int MAXN = 1001;

  int dp[MAXN][11][11][2];

  void build(int n)
  {
    for (int a = 0; a < n; a++)
    {
      for (int b = 0; b <= 10; b++)
      {
        for (int c = 0; c <= 10; c++)
        {
          for (int d = 0; d <= 1; d++)
          {
            dp[a][b][c][d] = -1;
          }
        }
      }
    }
  }

  int compute(string& l, string& r)
  {
    // [l...r]上的萌数 = [0...r]上的萌数 - [0...l]上的萌数
    int ans = (cnt(r) - cnt(l) + MOD) % MOD;
    if (check(l))
    {
      ans = (ans + 1) % MOD;
    }
    return ans;
  }

  // 返回0~num范围上萌数有多少个
  int cnt(string& num)
  {
    if (num[0] == '0')
    {
      // 0 肯定不是萌数
      return 0;
    }
    int n = num.length();
    long all = 0;  // 总数
    long base = 1;
    for (int i = n - 1; i >= 0; i--)
    {
      // 不理解的话看一下，讲解041-同余原理
      all = (all + base * (num[i] - '0')) % MOD;
      base = (base * 10) % MOD;
    }
    build(n);
    return (int) ((all - f(num, 0, 10, 10, 0) + MOD) % MOD);
  }

  // 从num的高位开始，当前来到第i位
  // 前一位数字是p、前前一位数字是pp，如果值是10，则表示那一位没有选择过数字
  // 如果之前的位已经确定比num小，那么free == 1，表示接下的数字可以自由选择
  // 如果之前的位和num一样，那么free == 0，表示接下的数字不能大于num当前位的数字
  // 返回<=num且不是萌数的数字有多少个
  int f(string& num, int i, int pp, int p, int free)
  {
    if (i == num.length())
    {
      return 1;
    }
    if (dp[i][pp][p][free] != -1)
    {
      return dp[i][pp][p][free];
    }
    int ans = 0;
    if (free == 0)
    {
      if (p == 10)
      {
        // 当前来到的就是num的最高位
        ans = (ans + f(num, i + 1, 10, 10, 1)) % MOD;  // 当前位不选数字
        // 当前位选择数字 < num[i]
        for (int cur = 1; cur < num[i] - '0'; cur++)
        {
          ans = (ans + f(num, i + 1, p, cur, 1)) % MOD;
        }
        // 当前位选择的数字 = num[i]
        ans = (ans + f(num, i + 1, p, num[i] - '0', 0)) % MOD;
      }
      else
      {
        // free == 0，之前和num一样，此时不能自由选择数字
        // 前一位p，选择过数字，p -> 0 ~ 9
        for (int cur = 0; cur < num[i] - '0'; cur++)
        {
          // 必须跟前前位、前一位都不一样
          if (pp != cur && p != cur)
          {
            ans = (ans + f(num, i + 1, p, cur, 1)) % MOD;
          }
        }
        if (pp != num[i] - '0' && p != num[i] - '0')
        {
          ans = (ans + f(num, i + 1, p, num[i] - '0', 0)) % MOD;
        }
      }
    }
    else
    {
      if (p == 10)
      {
        // free == 1，能自由选择数字
        // 从来没选过数字
        ans = (ans + f(num, i + 1, 10, 10, 1)) % MOD;  // 依然不选数字
        for (int cur = 1; cur <= 9; cur++)
        {
          ans = (ans + f(num, i + 1, p, cur, 1)) % MOD;
        }
      }
      else
      {
        // free == 1，能自由选择数字
        // 之前选择过数字
        for (int cur = 0; cur <= 9; cur++)
        {
          if (pp != cur && p != cur)
          {
            ans = (ans + f(num, i + 1, p, cur, 1)) % MOD;
          }
        }
      }
    }
    dp[i][pp][p][free] = ans;
    return ans;
  }

  // 一个数只要 num[i] == num[i-1] 或 num[i] != num[i-2]
  // 那么就是萌数
  bool check(string& num)
  {
    for (int pp = -2, p = -1, i = 0; i < num.length(); pp++, p++, i++)
    {
      if (pp >= 0 && num[pp] == num[i])
      {
        return true;
      }
      if (p >= 0 && num[p] == num[i])
      {
        return true;
      }
    }
    return false;
  }
};
