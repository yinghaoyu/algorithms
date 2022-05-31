#include <memory.h>
#include <iostream>
#include <vector>

using namespace std;

using TdArray = vector<vector<int>>;

// Fibonacci的log(N)解法
// 思路来源1：
// 快速幂：
// a^n = (a^(n/2))^2 * a^(n%2)，这里的n/2是取整数
//
// 思路来源2：
//                   | 0 1 |
// |a(i-2) a(i-1)| * | 1 1 | = |a(i-1) a(i)| (i >= 2)
// 得到：
//               | 0 1 |^(n-1)
// |a(0) a(1)| * | 1 1 |       = |a(n-1) a(n)|
//
// 拓展：只要有递推性质的，都可以使用这种思想
//
// 证明：https://zhuanlan.zhihu.com/p/352341370

class FibonacciProblem
{
 public:
  // 递归
  static int f1(int n)
  {
    if (n < 1)
    {
      return 0;
    }
    if (n == 1 || n == 2)
    {
      return 1;
    }
    return f1(n - 1) + f1(n - 2);
  }

  // 非递归
  static int f2(int n)
  {
    if (n < 1)
    {
      return 0;
    }
    if (n == 1 || n == 2)
    {
      return 1;
    }
    int res = 1;
    int pre = 1;
    int tmp = 0;
    for (int i = 3; i <= n; i++)
    {
      tmp = res;
      res = res + pre;
      pre = tmp;
    }
    return res;
  }

  // O(logN)
  static int f3(int n)
  {
    if (n < 1)
    {
      return 0;
    }
    if (n == 1 || n == 2)
    {
      return 1;
    }
    // [ 1 ,1 ]
    // [ 1, 0 ]
    TdArray base(2, vector<int>(2));
    base[0][0] = 1;
    base[0][1] = 1;
    base[1][0] = 1;
    base[1][1] = 0;
    TdArray res = matrixPower(base, n - 2);
    return res[0][0] + res[1][0];
  }

  static TdArray matrixPower(const TdArray &m, int p)
  {
    TdArray res(m.size(), vector<int>(m[0].size()));
    for (int i = 0; i < m.size(); i++)
    {
      res[i][i] = 1;
    }
    // res = 矩阵中的1
    TdArray t(m);  // 矩阵1次方
    for (; p != 0; p >>= 1)
    {
      if ((p & 1) != 0)  // p为奇数
      {
        res = muliMatrix(res, t);
      }
      t = muliMatrix(t, t);  // 快速幂
    }
    return res;
  }

  // 两个矩阵乘完之后的结果返回
  static TdArray muliMatrix(const TdArray &m1, const TdArray &m2)
  {
    TdArray res(m1.size(), vector<int>(m1[0].size()));
    for (int i = 0; i < m1.size(); i++)
    {
      for (int j = 0; j < m2[0].size(); j++)
      {
        for (int k = 0; k < m2.size(); k++)
        {
          res[i][j] += m1[i][k] * m2[k][j];
        }
      }
    }
    return res;
  }

  static int s1(int n)
  {
    if (n < 1)
    {
      return 0;
    }
    if (n == 1 || n == 2)
    {
      return n;
    }
    return s1(n - 1) + s1(n - 2);
  }

  static int s2(int n)
  {
    if (n < 1)
    {
      return 0;
    }
    if (n == 1 || n == 2)
    {
      return n;
    }
    int res = 2;
    int pre = 1;
    int tmp = 0;
    for (int i = 3; i <= n; i++)
    {
      tmp = res;
      res = res + pre;
      pre = tmp;
    }
    return res;
  }

  static int s3(int n)
  {
    if (n < 1)
    {
      return 0;
    }
    if (n == 1 || n == 2)
    {
      return n;
    }
    TdArray base(2, vector<int>(2));
    base[0][0] = 1;
    base[0][1] = 1;
    base[1][0] = 1;
    base[1][1] = 0;
    TdArray res = matrixPower(base, n - 2);
    return 2 * res[0][0] + res[1][0];
  }

  static int c1(int n)
  {
    if (n < 1)
    {
      return 0;
    }
    if (n == 1 || n == 2 || n == 3)
    {
      return n;
    }
    return c1(n - 1) + c1(n - 3);
  }

  static int c2(int n)
  {
    if (n < 1)
    {
      return 0;
    }
    if (n == 1 || n == 2 || n == 3)
    {
      return n;
    }
    int res = 3;
    int pre = 2;
    int prepre = 1;
    int tmp1 = 0;
    int tmp2 = 0;
    for (int i = 4; i <= n; i++)
    {
      tmp1 = res;
      tmp2 = pre;
      res = res + prepre;
      pre = tmp1;
      prepre = tmp2;
    }
    return res;
  }

  static int c3(int n)
  {
    if (n < 1)
    {
      return 0;
    }
    if (n == 1 || n == 2 || n == 3)
    {
      return n;
    }
    TdArray base(3, vector<int>(3));
    // [ 1, 1, 0 ]
    // [ 0, 0, 1 ]
    // [ 1, 0, 0 ]
    base[0][0] = 1;
    base[0][1] = 1;
    base[0][2] = 0;
    base[1][0] = 0;
    base[1][1] = 0;
    base[1][2] = 1;
    base[2][0] = 1;
    base[2][1] = 0;
    base[2][2] = 0;
    TdArray res = matrixPower(base, n - 3);
    return 3 * res[0][0] + 2 * res[1][0] + res[2][0];
  }

  static void test()
  {
    int n = 19;
    cout << f1(n) << endl;
    cout << f2(n) << endl;
    cout << f3(n) << endl;
    cout << "===" << endl;

    cout << s1(n) << endl;
    cout << s2(n) << endl;
    cout << s3(n) << endl;
    cout << "===" << endl;

    cout << c1(n) << endl;
    cout << c2(n) << endl;
    cout << c3(n) << endl;
    cout << "===" << endl;
  }
};

int main()
{
  FibonacciProblem::test();
  return 0;
}
