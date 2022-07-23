#include <iostream>

// 问题描述：快速幂

class PowXN
{
 public:
  static int pow(int a, int n)
  {
    int ans = 1;
    int t = a;
    while (n != 0)
    {
      if ((n & 1) != 0)
      {
        ans *= t;
      }
      t *= t;
      n >>= 1;
    }
    return ans;
  }

  // x的n次方，n可能是负数
  static double myPow(double x, int n)
  {
    if (n == 0)
    {
      return 1.0;
    }
    // 系统最小值的绝对值转正数是溢出的，因此要特殊处理
    // int a = INT32_MIN;
    // cout << a << endl; // 输出-2147483648
    // cout << std::abs(a) << endl; // 输出-2147483648
    // cout << -a << endl; // 输出-2147483648
    int pow = std::abs(n == INT32_MIN ? n + 1 : n);
    double t = x;
    double ans = 1.0;
    while (pow != 0)
    {
      if ((pow & 1) != 0)
      {
        ans *= t;
      }
      pow >>= 1;
      t = t * t;
    }
    if (n == INT32_MIN)
    {
      ans *= x;
    }
    return n < 0 ? (1.0 / ans) : ans;
  }
};
