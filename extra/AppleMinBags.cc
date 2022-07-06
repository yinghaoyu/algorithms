#include <iostream>

using namespace std;

// 问题描述：
// 小虎去买苹果，商店只提供两种类型的塑料袋，每种类型都有任意数量。
// A）能装下6个苹果的袋子
// B）能装下8个苹果的袋子
// 要求使用的袋子数量必须最少，且使用的每个袋子必须装满。
// 给定一个正整数N，返回至少使用多少袋子。
// 如果N无法让使用的每个袋子必须装满，返回-1

class AppleMinBags
{
 public:
  static int minBags(int apple)
  {
    if (apple < 0)
    {
      return -1;
    }
    int bag8 = (apple >> 3);         // 用B号袋子的数量
    int rest = apple - (bag8 << 3);  // B号袋子装不满的余数
    while (bag8 >= 0)
    {
      // rest 个
      if (rest % 6 == 0)
      {
        return bag8 + (rest / 6);
      }
      else
      {
        // 没有剩余的话，就把B号袋子的苹果倒出来
        bag8--;
        rest += 8;
      }
    }
    return -1;
  }

  static int minBagAwesome(int apple)
  {
    if ((apple & 1) != 0)
    {
      // 如果是奇数，返回-1
      return -1;
    }
    if (apple < 18)  // 最小公倍数
    {
      return apple == 0 ? 0 : (apple == 6 || apple == 8) ? 1 : (apple == 12 || apple == 14 || apple == 16) ? 2 : -1;
    }
    return (apple - 18) / 8 + 3;
  }

  static void test()
  {
    for (int apple = 1; apple < 200; apple++)
    {
      cout << apple << " : " << minBags(apple) << " " << minBagAwesome(apple) << endl;
    }
  }
};

int main()
{
  AppleMinBags::test();
  return 0;
}
