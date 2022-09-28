#include <algorithm>
#include <iostream>
#include <random>
#include <set>
#include <vector>

using namespace std;

using TdArray = vector<vector<int>>;

#define MAX_SEED 1024

#define HALF_SEED (MAX_SEED >> 1)

// 来自百度
// 二狗买了一些小兵玩具，和大胖一起玩
// 一共有n个小兵，这n个小兵拍成一列
// 第i个小兵战斗力为hi，然后他们两个开始对小兵进行排列
// 一共进行m次操作，二狗每次操作选择一个数k，将前k个小兵战斗力从小到大排列
// 大胖每次操作选择一个数k，将前k个小兵战斗力从大到小排列
// 问所有操作结束后，排列顺序什么样
// 给定一个长度为n的数组arr，表示每个小兵的战斗力
// 给定一个长度为m的数组op,
// op[i] = { k , 0 }, 表示对前k个士兵执行从小到大的操作
// op[i] = { k , 1 }, 表示对前k个士兵执行从大到小的操作
// 返回数组ans，表示最终的排列
// 1 <= n, m <= 2 * 10^5
// - 10^9 <= arr[i] <= + 10^9
class SortGame
{
 public:
  // 暴力方法
  // 为了验证
  static vector<int> game1(vector<int> &arr, TdArray &op)
  {
    int n = arr.size();
    vector<int> help(n);
    for (int i = 0; i < n; i++)
    {
      help[i] = arr[i];
    }
    for (auto o : op)
    {
      if (o[1] == 0)
      {
        std::sort(help.begin(), help.begin() + o[0], [](int a, int b) -> bool { return a < b; });
      }
      else
      {
        std::sort(help.begin(), help.begin() + o[0], [](int a, int b) -> bool { return b < a; });
      }
    }
    vector<int> ans(n);
    for (int i = 0; i < n; i++)
    {
      ans[i] = help[i];
    }
    return ans;
  }

  // 正式方法
  // 时间复杂度O(M) + O(N*logN)
  static vector<int> game2(vector<int> &arr, TdArray &op)
  {
    int n = arr.size();
    int m = op.size();
    vector<int> stack(m);
    int r = 0;
    for (int i = 0; i < m; i++)
    {
      // 假设按序有如下3条操作
      // {50, 0}
      // {100, 0}
      // {200, 1} 可以发现前面两条操作可以直接忽略
      // 单调递减栈，过滤无效操作
      while (r != 0 && op[stack[r - 1]][0] <= op[i][0])
      {
        r--;
      }
      stack[r++] = i;
    }
    vector<int> ans(n);
    int ansi = n - 1;
    int l = 0;
    // 假设数组长150，单调递减栈过滤后剩余有效操作如下：
    // {100, 1}
    // {80, 0}
    // {30, 1}
    // {20, 0}
    for (; ansi >= op[stack[l]][0]; ansi--)
    {
      // 因为操作长度是递减的，后面的数最终位置已经确定
      // 先把后150 - 100 = 50个数拷贝
      ans[ansi] = arr[ansi];
    }
    set<Number> sorted;
    // 对前100个数排序
    for (int i = 0; i < op[stack[l]][0]; i++)
    {
      sorted.emplace(Number(arr[i], i));
    }
    while (l != r)
    {
      // 当前处理的指令
      vector<int> cur = op[stack[l++]];
      if (l != r)
      {
        vector<int> next = op[stack[l]];
        int num = cur[0] - next[0];  // 后面的数
        if (cur[1] == 0)
        {
          // 从小到大排序
          for (int i = 0; i < num; i++)
          {
            // 把后面的数先拷贝
            ans[ansi--] = sorted.rbegin()->value;
            sorted.erase(*sorted.rbegin());
          }
        }
        else
        {
          // 从大到小排序
          for (int i = 0; i < num; i++)
          {
            ans[ansi--] = sorted.begin()->value;
            sorted.erase(*sorted.begin());
          }
        }
      }
      else
      {
        if (cur[1] == 0)
        {
          while (!sorted.empty())
          {
            ans[ansi--] = sorted.rbegin()->value;
            sorted.erase(*sorted.rbegin());
          }
        }
        else
        {
          while (!sorted.empty())
          {
            ans[ansi--] = sorted.begin()->value;
            sorted.erase(*sorted.begin());
          }
        }
      }
    }
    return ans;
  }

  class Number
  {
   public:
    int value;
    int index;

    Number(int v, int i)
    {
      value = v;
      index = i;
    }

    bool operator<(const Number &other) const  // 对set排序一定要加const
    {
      // 值按从小到大排序，值相等按索引从小到大排序
      if (this->value != other.value)
      {
        return this->value < other.value;
      }
      else
      {
        return this->index < other.index;
      }
    }
  };

  // 为了测试
  static vector<int> randomArray(int n, int v)
  {
    vector<int> ans(n);
    for (int i = 0; i < n; i++)
    {
      ans[i] = getRandom(1, v);
    }
    return ans;
  }

  // 为了测试
  static TdArray randomOp(int m, int n)
  {
    TdArray ans(m, vector<int>(2));
    for (int i = 0; i < m; i++)
    {
      ans[i][0] = getRandom(1, n);
      ans[i][1] = getRandom(0, MAX_SEED) < HALF_SEED ? 0 : 1;
    }
    return ans;
  }

  // 为了测试
  static bool isEqual(vector<int> &arr1, vector<int> &arr2)
  {
    if (arr1.size() != arr2.size())
    {
      return false;
    }
    for (int i = 0; i < arr1.size(); i++)
    {
      if (arr1[i] != arr2[i])
      {
        return false;
      }
    }
    return true;
  }

  static int getRandom(int min, int max)
  {
    random_device seed;                            // 硬件生成随机数种子
    ranlux48 engine(seed());                       // 利用种子生成随机数引
    uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
    int res = distrib(engine);                     // 随机数
    return res;
  }

  // 为了测试
  static void test()
  {
    int N = 100;
    int M = 100;
    int V = 200;
    int testTimes = 5000;
    cout << "测试开始" << endl;
    for (int i = 0; i < testTimes; i++)
    {
      int n = getRandom(1, N);
      int m = getRandom(1, M);
      vector<int> arr = randomArray(n, V);
      TdArray op = randomOp(m, n);
      vector<int> ans1 = game1(arr, op);
      vector<int> ans2 = game2(arr, op);
      if (!isEqual(ans1, ans2))
      {
        cout << "出错了!" << endl;
      }
    }
    cout << "测试结束" << endl;
  }
};

int main()
{
  SortGame::test();
  return 0;
}
