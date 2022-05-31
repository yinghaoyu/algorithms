#include <iostream>
#include <vector>

using namespace std;

using TdArray = vector<vector<int>>;

// 题目描述：
// 给定一个数N，想象只由0和1两种字符，组成的所有长度为N的字符串
// 如果某个字符串,任何0字符的左边都有1紧挨着,认为这个字符串达标
// 返回有多少达标的字符串
//
// 思路：定义一个函数f(i)，表示在i的前面一定是1返回此时i个数达标的情况，
// 情况一：如果i位置是0那么i+1位置只可能是1,所以让i+2位置返回多少种方法f(i+2)
// 情况二：如果i位置是1那么让i+1位置去决策方法数f(i+1)
// 所以地推式：f(i) = f(i + 1) + f(i + 2), 又是一个斐波拉切数列问题。

class ZeroLeftOneStringNumber
{
 public:
  static int getNum1(int n)
  {
    if (n < 1)
    {
      return 0;
    }
    return process(1, n);
  }

  static int process(int i, int n)
  {
    if (i == n - 1)
    {
      return 2;
    }
    if (i == n)
    {
      return 1;
    }
    return process(i + 1, n) + process(i + 2, n);
  }

  static int getNum2(int n)
  {
    if (n < 1)
    {
      return 0;
    }
    if (n == 1)
    {
      return 1;
    }
    int pre = 1;
    int cur = 1;
    int tmp = 0;
    for (int i = 2; i < n + 1; i++)
    {
      tmp = cur;
      cur += pre;
      pre = tmp;
    }
    return cur;
  }

  // 由f(i) = f(i + 1) + f(i + 2)构建
  //                   |0 1|
  // [f(i+2) f(i+1)] * |1 1| = [f(i+1) f(i)]
  //
  //                     |0 1|^(n-2)
  // 可得[f(n) f(n-1)] * |1 1|       = [f(i+1) f(i)]
  // 此时f(n) = 1, f(n-1) = 2
  static int getNum3(int n)
  {
    if (n < 1)
    {
      return 0;
    }
    if (n == 1 || n == 2)
    {
      return n;
    }
    // {1, 1}
    // {1, 0}
    TdArray base(2, vector<int>(2));
    base[0][0] = 0;
    base[0][1] = 1;
    base[1][0] = 1;
    base[1][1] = 1;
    // 矩阵计算base^(n-2)
    TdArray res = matrixPower(base, n - 2);
    // 根据f(i) = f(n) * res[1][0] + f(n-1) * res[1][1]
    return res[1][0] + 2 * res[1][1];
  }

  static TdArray matrixPower(const TdArray &m, int p)
  {
    TdArray res(m.size(), vector<int>(m[0].size()));
    for (int i = 0; i < m.size(); i++)
    {
      res[i][i] = 1;  // 单位矩阵
    }
    TdArray tmp(m);
    for (; p != 0; p >>= 1)
    {
      if ((p & 1) != 0)  // 奇数
      {
        res = muliMatrix(res, tmp);
      }
      tmp = muliMatrix(tmp, tmp);
    }
    return res;
  }

  static TdArray muliMatrix(const TdArray &m1, const TdArray &m2)
  {
    TdArray res(m1.size(), vector<int>(m2[0].size()));
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

  static void test()
  {
    for (int i = 0; i != 20; i++)
    {
      cout << getNum1(i) << endl;
      cout << getNum2(i) << endl;
      cout << getNum3(i) << endl;
      cout << "===================" << endl;
    }
  }
};

int main()
{
  ZeroLeftOneStringNumber::test();
  return 0;
}
