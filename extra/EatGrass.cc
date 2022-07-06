#include <iostream>
#include <string>

using namespace std;

// 问题描述：
// 给定一个正整数N，表示有N份青草统一堆放在仓库里
// 有一只牛和一只羊，牛先吃，羊后吃，它俩轮流吃草
// 不管是牛还是羊，每一轮能吃的草量必须是：
// 1，4，16，64…(4的某次方)
// 谁最先把草吃完，谁获胜
// 假设牛和羊都绝顶聪明，都想赢，都会做出理性的决定
// 根据唯一的参数N，返回谁会赢

class EatGrass
{
 public:
  // 如果n份草，最终先手赢，返回"先手"
  // 如果n份草，最终后手赢，返回"后手"
  static string whoWin(int n)
  {
    if (n < 5)
    {
      return n == 0 || n == 2 ? "后手" : "先手";
    }
    // 进到这个过程里来，当前的先手，先选
    int want = 1;
    while (want <= n)
    {
      // 先手吃掉want，接下来的操作相当于后手
      // 如果whoWin(n-want)返回的是后手赢，那么说明本次先手赢！！！
      if (whoWin(n - want) == "后手")
      {
        return "先手";
      }
      if (want <= (n / 4))  // 防止溢出
      {
        want *= 4;
      }
      else
      {
        break;
      }
    }
    return "后手";
  }

  static string winner1(int n)
  {
    if (n < 5)
    {
      return (n == 0 || n == 2) ? "后手" : "先手";
    }
    int base = 1;
    while (base <= n)
    {
      if (winner1(n - base) == "后手")
      {
        return "先手";
      }
      if (base > n / 4)
      {
        // 防止base*4之后溢出
        break;
      }
      base *= 4;
    }
    return "后手";
  }

  static string winner2(int n)
  {
    if (n % 5 == 0 || n % 5 == 2)
    {
      return "后手";
    }
    else
    {
      return "先手";
    }
  }

  static void test()
  {
    for (int i = 0; i <= 50; i++)
    {
      cout << i << " : " << whoWin(i) << " " << winner1(i) << " " << winner2(i) << endl;
    }
  }
};

int main()
{
  EatGrass::test();
  return 0;
}
