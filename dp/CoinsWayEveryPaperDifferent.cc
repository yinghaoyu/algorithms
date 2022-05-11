#include <iostream>
#include <memory.h>
#include <random>
#include <stdbool.h>

using namespace std;

// 问题描述：
// arr表示不同面值的硬币，从中任意取出硬币(每种arr[i]只能使用一次)，求达到目标金额aim的方法数。

class CoinsWayEveryPaperDifferent
{
  public:

    static int coinWays(int* arr, int len, int aim)
    {
      return process(arr, len, 0, aim);
    }

    // arr[index....] 组成正好rest这么多的钱，有几种方法
    static int process(int* arr, int len, int index, int rest)
    {
      if (rest < 0)
      {
        return 0;
      }
      if (index == len)
      {
        // 没钱了！
        return rest == 0 ? 1 : 0;
      }
      else
      {
        return process(arr, len, index + 1, rest)
                + process(arr, len, index + 1, rest - arr[index]);
      }
    }

    static int dp(int* arr, int len, int aim)
    {
      if (aim == 0)
      {
        return 1;
      }
      int N = len;
      int** dp = mallocArray(N + 1, aim + 1);
      dp[N][0] = 1;  // base case
      for (int index = N - 1; index >= 0; index--)
      {
        for (int rest = 0; rest <= aim; rest++)
        {
          dp[index][rest] = dp[index + 1][rest]
                    + (rest - arr[index] >= 0 ? dp[index + 1][rest - arr[index]] : 0);
        }
      }
      int ans = dp[0][aim];
      return ans;
    }

    // for test
    static int** mallocArray(int row, int column)
    {
      int** arr = (int**)malloc(sizeof(int*) * row);
      for(int i = 0; i < row; i++)
      {
        arr[i] = (int*)malloc(sizeof(int) * column);
        memset(arr[i], 0, sizeof(int) * column);
      }
      return arr;
    }

    static void freeArray(int** arr, int row)
    {
      for(int i = 0; i < row; i++)
      {
        free(arr[i]);
      }
      free(arr);
    }

    static int getRandom(int min, int max)
    {
      random_device seed;  // 硬件生成随机数种子
      ranlux48 engine(seed());  // 利用种子生成随机数引
      uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
      int res = distrib(engine);  // 随机数
      return res;
    }

    // 为了测试
    static int* randomArray(int maxLen, int maxValue, int* len)
    {
      *len = getRandom(0, maxLen);
      int* arr = (int*)malloc(sizeof(int) * (*len));
      for (int i = 0; i < *len; i++)
      {
        // arr[i] != 0，如果存在arr[i] = 0 且此时 aim = 0
        // 那么qrr[i]全部不取就被算进aim结果里了，递归写法结果会多1次
        arr[i] = getRandom(1, maxValue);
      }
      return arr;
    }

    // 为了测试
    static void printArray(int* arr, int len)
    {
      for (int i = 0; i < len; i++)
      {
        cout << arr[i] << " ";
      }
      cout << endl;
    }

    // 为了测试
    static void test()
    {
      int maxLen = 20;
      int maxValue = 30;
      int testTime = 10000;
      int len = 0;
      cout << "测试开始" << endl;
      for (int i = 0; i < testTime; i++)
      {
        int* arr = randomArray(maxLen, maxValue, &len);
        int aim = getRandom(0, maxValue);
        int ans1 = coinWays(arr, len, aim);
        int ans2 = dp(arr, len, aim);
        if (ans1 != ans2)
        {
          cout << "Oops!" << endl;
          printArray(arr, len);
          cout << aim << endl;
          cout << ans1 << endl;
          cout << ans2 << endl;
          break;
        }
      }
      cout << "测试结束" << endl;
    }
};

int main()
{
  CoinsWayEveryPaperDifferent::test();
  return 0;
}
