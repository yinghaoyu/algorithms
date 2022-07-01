#include <iostream>
#include <list>

using namespace std;

// 问题描述：n个元素进栈，共有多少种出栈顺序？

// 卡特兰数组
// https://zhuanlan.zhihu.com/p/97619085

class Catalan
{
 public:
  // 根据性质计算
  static long ways1(int N)
  {
    int zero = N;  // 合法的序列1和0应该数量一样
    int one = N;
    list<int> path;       // 尝试扩1或者扩0的路径
    list<list<int>> ans;  // 所有路径集合
    process(zero, one, path, ans);
    long count = 0;
    for (list<int> cur : ans)
    {
      int status = 0;  // 用来统计前缀和1和0的数量差
      for (int num : cur)
      {
        if (num == 0)
        {
          status++;
        }
        else
        {
          status--;
        }
        if (status < 0)  // 前缀和只要有一次 < 0，这个路径就是非法路径
        {
          break;
        }
      }
      if (status == 0)  // 1和0数量一样，说明是合法路径
      {
        count++;
      }
    }
    return count;
  }

  static void process(int zero, int one, list<int> &path, list<list<int>> &ans)
  {
    if (zero == 0 && one == 0)
    {
      ans.push_back(path);
    }
    else
    {
      if (zero == 0)  // 只有1了
      {
        path.push_back(1);
        process(zero, one - 1, path, ans);
        path.pop_back();
      }
      else if (one == 0)  // 只有0了
      {
        path.push_back(0);
        process(zero - 1, one, path, ans);
        path.pop_back();
      }
      else
      {
        path.push_back(1);  // 扩1
        process(zero, one - 1, path, ans);
        path.pop_back();    // 扩1和扩0是互斥的，因此要恢复
        path.push_back(0);  // 扩0
        process(zero - 1, one, path, ans);
        path.pop_back();  // 恢复
      }
    }
  }

  // 直接利用公式
  static long ways2(int N)
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
    long limit = N << 1;
    for (long i = 1; i <= limit; i++)
    {
      if (i <= N)
      {
        a *= i;
      }
      else
      {
        b *= i;
      }
    }
    return (b / a) / (N + 1);
  }

  static void test()
  {
    cout << "test begin" << endl;
    for (int i = 0; i < 10; i++)
    {
      long ans1 = ways1(i);
      long ans2 = ways2(i);
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
  Catalan::test();
  return 0;
}
