#include <math.h>
#include <iostream>

using namespace std;

class PerfectSquares
{
 public:
  // 暴力解
  static int numSquares1(int n)
  {
    int res = n, num = 2;
    while (num * num <= n)
    {
      int a = n / (num * num), b = n % (num * num);
      res = std::min(res, a + numSquares1(b));
      num++;
    }
    return res;
  }

  // 1 : 1, 4, 9, 16, 25, 36, ...
  // 4 : 7, 15, 23, 28, 31, 39, 47, 55, 60, 63, 71, ...
  // 规律解
  // 规律一：个数不超过4
  // 规律二：出现1个的时候，显而易见
  // 规律三：任何数 % 8 == 7，一定是4个
  // 规律四：任何数消去4的因子之后，剩下rest，rest % 8 == 7，一定是4个
  static int numSquares2(int n)
  {
    int rest = n;
    while (rest % 4 == 0)
    {
      rest /= 4;
    }
    if (rest % 8 == 7)
    {
      return 4;
    }
    int f = std::sqrt(n);
    if (f * f == n)
    {
      return 1;
    }
    for (int first = 1; first * first <= n; first++)
    {
      int second = std::sqrt(n - first * first);
      if (first * first + second * second == n)
      {
        return 2;
      }
    }
    return 3;
  }

  // 数学解
  // 1）四平方和定理
  // 2）任何数消掉4的因子，结论不变
  static int numSquares3(int n)
  {
    while (n % 4 == 0)
    {
      n /= 4;
    }
    if (n % 8 == 7)
    {
      return 4;
    }
    for (int a = 0; a * a <= n; ++a)
    {
      // a * a +  b * b = n
      int b = (int) std::sqrt(n - a * a);
      if (a * a + b * b == n)
      {
        return (a > 0 && b > 0) ? 2 : 1;
      }
    }
    return 3;
  }

  static void test()
  {
    for (int i = 1; i < 1000; i++)
    {
      cout << i << " , " << numSquares1(i);
    }
  }
};

int main()
{
  return 0;
}
