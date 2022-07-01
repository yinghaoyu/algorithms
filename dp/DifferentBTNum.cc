#include <iostream>
#include <vector>

using namespace std;

// 卡特兰数组的应用
// 问题描述：
// n + 1 个叶子节点能够构成多少种形状不同的满二叉树？

// 使用深度优先搜索这个满二叉树，向左扩展时标记为 +1，向右扩展时标记为 -1。
// 由于每个非叶子节点都有两个左右子节点，所有它必然会先向左扩展，再向右扩展。
// 总体下来，左右扩展将会形成匹配，即变成进出栈的题型。
// n + 1个叶子结点会有 2n 次扩展

class DifferentBTNum
{
 public:
  //  k(0) = 1, k(1) = 1
  //  k(n) = k(0) * k(n - 1) + k(1) * k(n - 2) + ... + k(n - 2) * k(1) + k(n - 1) * k(0)
  //  或者
  //  k(n) = c(2n, n) / (n + 1)
  //  或者
  //  k(n) = c(2n, n) - c(2n, n-1)
  //
  static long num1(int N)
  {
    if (N < 0)
    {
      return 0;
    }
    if (N < 2)
    {
      return 1;
    }
    vector<long> dp(N + 1);
    dp[0] = 1;
    dp[1] = 1;
    for (int i = 2; i <= N; i++)
    {
      for (int leftSize = 0; leftSize < i; leftSize++)
      {
        dp[i] += dp[leftSize] * dp[i - 1 - leftSize];
      }
    }
    return dp[N];
  }

  static long num2(int N)
  {
    if (N < 0)
    {
      return 0;
    }
    if (N < 2)
    {
      return 1;
    }
    long a = 1;
    long b = 1;
    for (int i = 1, j = N + 1; i <= N; i++, j++)
    {
      a *= i;
      b *= j;
      long val = gcd(a, b);
      a /= val;
      b /= val;
    }
    return (b / a) / (N + 1);
  }

  // 求最大公约数
  static long gcd(long m, long n) { return n == 0 ? m : gcd(n, m % n); }

  static void test()
  {
    cout << "test begin" << endl;
    for (int i = 0; i < 15; i++)
    {
      long ans1 = num1(i);
      long ans2 = num2(i);
      if (ans1 != ans2)
      {
        cout << "Oops!" << endl;
      }
    }
    cout << "test finish" << endl;
  }
};

int main()
{
  DifferentBTNum::test();
  return 0;
}
