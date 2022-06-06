#include <iostream>
#include <random>

using namespace std;

// 问题描述:
// 给定一个非负数组arr，长度为N，那么有N-1种方案可以把arr切成左右两部分。
// 每一种方案都有，min{左部分累加和，右部分累加和}，
// 求这么多方案中，min{左部分累加和，右部分累加和}的最大值是多少？
// 整个过程要求时间复杂度O(N)

class BestSplitForAll
{
 public:
  // 暴力解时间复杂度O(log(N))
  static int bestSplit1(int *arr, int len)
  {
    if (arr == nullptr || len < 2)
    {
      return 0;
    }
    int N = len;
    int ans = 0;
    for (int s = 0; s < N - 1; s++)
    {
      int sumL = 0;
      // 以s为分割点分别计算左右
      for (int L = 0; L <= s; L++)
      {
        sumL += arr[L];
      }
      int sumR = 0;
      for (int R = s + 1; R < N; R++)
      {
        sumR += arr[R];
      }
      ans = std::max(ans, std::min(sumL, sumR));
    }
    return ans;
  }

  // 利用前缀和时间复杂度降为O(log(N))
  static int bestSplit2(int *arr, int len)
  {
    if (arr == nullptr || len < 2)
    {
      return 0;
    }
    int N = len;
    int sumAll = 0;
    for (int i = 0; i < len; i++)
    {
      sumAll += arr[i];
    }
    int ans = 0;
    int sumL = 0;
    // [0...s]  [s+1...N-1]
    for (int s = 0; s < N - 1; s++)
    {
      sumL += arr[s];
      int sumR = sumAll - sumL;
      ans = std::max(ans, std::min(sumL, sumR));
    }
    return ans;
  }

  static int *randomArray(int len, int max)
  {
    int *ans = new int[len]();
    for (int i = 0; i < len; i++)
    {
      ans[i] = getRandom(0, max);
    }
    return ans;
  }

  static int getRandom(int min, int max)
  {
    random_device seed;                            // 硬件生成随机数种子
    ranlux48 engine(seed());                       // 利用种子生成随机数引
    uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
    int res = distrib(engine);                     // 随机数
    return res;
  }

  static void test()
  {
    int N = 20;
    int max = 30;
    int testTime = 1000000;
    cout << "测试开始" << endl;
    for (int i = 0; i < testTime; i++)
    {
      int len = getRandom(0, N);
      int *arr = randomArray(len, max);
      int ans1 = bestSplit1(arr, len);
      int ans2 = bestSplit2(arr, len);
      if (ans1 != ans2)
      {
        cout << ans1 << endl;
        cout << ans2 << endl;
        cout << "Oops!" << endl;
      }
    }
    cout << "测试结束" << endl;
  }
};

int main()
{
  BestSplitForAll::test();
  return 0;
}
