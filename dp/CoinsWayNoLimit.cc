#include <iostream>
#include <memory.h>
#include <random>
#include <stdbool.h>

using namespace std;

// 问题描述：
// arr表示不同面值的硬币，从中任意取出硬币(每种arr[i]可以重复使用)，求达到目标金额aim的方法数。
// 你可以认为每种硬币的数量是无限的

class CoinsWayNoLimit
{
  public:

    static int coinWays(int* arr, int len, int aim)
    {
      if(arr == nullptr || len == 0 || aim < 0)
      {
        return 0;
      }
      return process(arr, len, 0, aim);
    }

    // arr[index....] 所有的面值，每一个面值都可以任意选择张数，组成正好rest这么多钱，方法数多少？
    static int process(int* arr, int len, int index, int rest)
    {
      if (index == len)
      {
        // 没钱了！
        return rest == 0 ? 1 : 0;
      }
      int ways = 0;
      for(int zhang = 0; zhang * arr[index] <= rest; zhang++)
      {
        ways += process(arr, len, index + 1, rest - (zhang * arr[index]));
      }
      return ways;
    }

    static int dp1(int* arr, int len, int aim)
    {
      if(arr == nullptr || len == 0 || aim < 0)
      {
        return 0;
      }
      int N = len;
      int** dp = mallocArray(N + 1, aim + 1);
      dp[N][0] = 1;  // base case
      for (int index = N - 1; index >= 0; index--)
      {
        for (int rest = 0; rest <= aim; rest++)
        {
          int ways = 0;
          for(int zhang = 0; zhang * arr[index] <= rest; zhang++)
          {
            ways += dp[index + 1][rest - (zhang * arr[index])];
          }
          dp[index][rest] = ways;
        }
      }
      int ans = dp[0][aim];
      return ans;
    }

    static int dp2(int* arr, int len, int aim)
    {
      if(arr == nullptr || len == 0 || aim < 0)
      {
        return 0;
      }
      int N = len;
      int** dp = mallocArray(N + 1, aim + 1);
      dp[N][0] = 1;  // base case
      for (int index = N - 1; index >= 0; index--)
      {
        for (int rest = 0; rest <= aim; rest++)
        {
          dp[index][rest] = dp[index + 1][rest];
          // 斜率优化，直接观察得出！
          // dp[index + 1][rest] + dp[index][rest - arr[index]]就是dp1的ways的值
          if (rest - arr[index] >= 0)
          {
            dp[index][rest] += dp[index][rest - arr[index]];
          }
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
      bool* has = (bool*)malloc(sizeof(bool) * maxValue + 1);
      memset(has, false, sizeof(bool) * (maxValue + 1));
      for (int i = 0; i < *len; i++)
      {
        do
        {
          // arr[i] != 0，如果存在arr[i] = 0 且此时 aim = 0
          // 那么qrr[i]全部不取就被算进aim结果里了，递归写法结果会多1次
          arr[i] = getRandom(1, maxValue);
        }while(has[arr[i]]);
        has[arr[i]] = true;
      };
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
        int aim = 11;//getRandom(0, maxValue);
        int ans1 = coinWays(arr, len, aim);
        int ans2 = dp1(arr, len, aim);
        int ans3 = dp2(arr, len, aim);
        if (ans1 != ans2 || ans1 != ans3)
        {
          cout << "Oops!" << endl;
          printArray(arr, len);
          cout << aim << endl;
          cout << ans1 << endl;
          cout << ans2 << endl;
          cout << ans3 << endl;
          break;
        }
      }
      cout << "测试结束" << endl;
    }
};

int main()
{
  CoinsWayNoLimit::test();
  return 0;
}
