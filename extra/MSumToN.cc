#include <iostream>

using namespace std;

// 问题描述：
// 定义一种数：可以表示成若干（数量>1）连续正数和的数
// 比如:
// 5 = 2+3，5就是这样的数
// 12 = 3+4+5，12就是这样的数
// 1不是这样的数，因为要求数量大于1个、连续正数和
// 2 = 1 + 1，2也不是，因为等号右边不是连续正数
// 给定一个参数N，返回是不是可以表示成若干连续正数和的数

class MSumToN
{
 public:
  static bool isMSum1(int num)
  {
    for (int start = 1; start <= num; start++)
    {
      int sum = start;
      for (int j = start + 1; j <= num; j++)
      {
        if (sum + j > num)
        {
          break;
        }
        if (sum + j == num)
        {
          return true;
        }
        sum += j;
      }
    }
    return false;
  }

  static bool isMSum2(int num)
  {
    //
    //  return num == (num & (~num + 1));
    //
    //  return num == (num & (-num));
    //
    // num &(num - 1) == 0 表示num是2的指数,
    // 例如10000，10000 - 1 = 01111，10000 & 01111 = 00000
    return (num & (num - 1)) != 0;
  }

  static void test()
  {
    for (int num = 1; num < 200; num++)
    {
      cout << num << " : " << isMSum1(num) << endl;
    }
    cout << "test begin" << endl;
    for (int num = 1; num < 5000; num++)
    {
      if (isMSum1(num) != isMSum2(num))
      {
        cout << "Oops!" << endl;
      }
    }
    cout << "test end" << endl;
  }
};

int main()
{
  MSumToN::test();
  return 0;
}
