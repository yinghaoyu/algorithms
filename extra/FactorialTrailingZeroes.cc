#include <iostream>

using namespace std;

// 问题描述：
// 给定一个整数 n ，返回 n! 结果中尾随零的数量。
//
// 分析：
// 尾随0的数量等价于求能被多少个10除
// 10 = 2 * 5
// 对于1 * 2 * 3 * 4 * 5 ....
// 以为因子2的数量明显大于5，因此只需要求5的因子数量
// 每5个数有1个5的因子
// 1...5 有1个
// 6...10 有1个
// ...
// 21...25 这时25 = 5 * 5 有2个
// ...
// 121...125 这时125 = 5 * 5 * 5 有3个
// 综上，n/5 + n / 25 + n / 125 + ... + 0

// 测试链接：https://leetcode.cn/problems/factorial-trailing-zeroes/

class FactorialTrailingZeroes
{
 public:
  static int trailingZeroes(int n)
  {
    int ans = 0;
    while (n != 0)
    {
      n /= 5;
      ans += n;
    }
    return ans;
  }
};

int main()
{
  return 0;
}
