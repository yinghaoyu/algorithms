#include <iostream>
#include <vector>

using namespace std;

// 问题描述： 计算素数或者叫质数

// 测试链接：https://leetcode.cn/problems/count-primes/

class CountPrimes
{
 public:
  static int countPrimes(int n)
  {
    if (n < 3)
    {
      return 0;
    }
    // j已经不是素数了，f[j] = true;
    vector<bool> f(n);
    int count = n / 2;  // 所有偶数都不要，还剩几个数
    // 跳过了1、2  从3、5、7、...
    for (int i = 3; i * i < n; i += 2)
    {
      if (f[i])
      {
        continue;
      }
      // 3 * 2、3 * 1 都算过了
      // 7 * 6、7 * 5、7 * 4、... 都算过了
      // 所以从 i * i 开始递增去算
      // 3 -> 3 * 3 = 9   3 * 5 = 15   3 * 7 = 21
      // 7 -> 7 * 7 = 49  7 * 9 = 63
      // 13 -> 13 * 13  13 * 15
      for (int j = i * i; j < n; j += 2 * i)
      {
        if (!f[j])
        {
          --count;
          f[j] = true;
        }
      }
    }
    return count;
  }
};

int main()
{
  return 0;
}
