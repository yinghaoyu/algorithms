#include <iostream>
#include <unordered_map>
#include <stdbool.h>
#include <random>
#include <memory.h>

using namespace std;

class CoinsWaySameValueSamePapper
{
  public:
    class Info
    {
      public:
        int* coins;  // coins[i]不同的面值
        int* zhangs;  // zhangs[i]面值的张数
        int length; // 去重后的数组长度

        Info(int* c, int* z, int len)
        {
          coins = c;
          zhangs = z;
          length = len;
        }
    };

    // 去重
    static Info getInfo(int* arr, int len)
    {
      unordered_map<int, int> counts;
      for (int i = 0; i < len; i++)
      {
        if (counts.find(arr[i]) == counts.end())
        {
          counts[arr[i]] = 1;
        }
        else
        {
          counts[arr[i]]++;
        }
      }
      int N = counts.size();
      int* coins = (int*)malloc(sizeof(int) * N);
      memset(coins, 0, sizeof(int) * N);
      int* zhangs = (int*)malloc(sizeof(int) * N);
      memset(zhangs, 0, sizeof(int) * N);
      int index = 0;
      for (auto cur : counts)
      {
        coins[index] = cur.first;
        zhangs[index++] = cur.second;
      }
      return Info(coins, zhangs, N);
    }

    static int coinsWay(int* arr, int len, int aim)
    {
      if (arr == nullptr || len == 0 || aim < 0)
      {
        return 0;
      }
      Info info = getInfo(arr, len);
      return process(info.coins, info.zhangs, info.length, 0, aim);
    }

    // coins 面值数组，正数且去重
    // zhangs 每种面值对应的张数
    static int process(int* coins, int* zhangs, int len, int index, int rest)
    {
      if (index == len)
      {
        return rest == 0 ? 1 : 0;
      }
      int ways = 0;
      for (int zhang = 0; zhang * coins[index] <= rest && zhang <= zhangs[index]; zhang++)
      {
        ways += process(coins, zhangs, len, index + 1, rest - (zhang * coins[index]));
      }
      return ways;
    }

    static int dp1(int* arr, int len, int aim)
    {
      if (arr == nullptr || len == 0 || aim < 0)
      {
        return 0;
      }
      Info info = getInfo(arr, len);
      int* coins = info.coins;
      int* zhangs = info.zhangs;
      int N = info.length;
      int** dp = mallocArray(N + 1, aim + 1);
      dp[N][0] = 1;
      for (int index = N - 1; index >= 0; index--)
      {
        for (int rest = 0; rest <= aim; rest++)
        {
          int ways = 0;
          for (int zhang = 0; zhang * coins[index] <= rest && zhang <= zhangs[index]; zhang++)
          {
            ways += dp[index + 1][rest - (zhang * coins[index])];
          }
          dp[index][rest] = ways;
        }
      }
      int ans = dp[0][aim];
      freeArray(dp, N + 1);
      return ans;
    }

    static int dp2(int* arr, int len, int aim)
    {
      if (arr == nullptr || len == 0 || aim < 0)
      {
        return 0;
      }

      Info info = getInfo(arr, len);
      int* coins = info.coins;
      int* zhangs = info.zhangs;
      int N = info.length;
      int** dp = mallocArray(N + 1, aim + 1);
      dp[N][0] = 1;
      for (int index = N - 1; index >= 0; index--)
      {
        for (int rest = 0; rest <= aim; rest++)
        {
          dp[index][rest] = dp[index + 1][rest];  // 当前index不使用
          // 当前index不计数量的使用，斜率优化
          if (rest - coins[index] >= 0)
          {
            dp[index][rest] += dp[index][rest - coins[index]];
          }
          // 减去数量超出zhangs[index]的方案
          if (rest - coins[index] * (zhangs[index] + 1) >= 0)
          {
            // 仔细想一想这里为什么减去 index + 1 而不是 index ？
            // 从←看，不计数量使用：
            // index+1行 |    d     |   c   |  b  | a
            // index行   | a+b+c+d  | a+b+c | a+b | a
            // 从←看，记数量使用，减去index项：
            // index+1行 |             d          |      c      |  b  | a
            // index行   | a+b+c+d - (a) - (a + b)| a+b+c - (a) | a+b | a
            // 会发现减了n次a，(n-1)次b，(n-2)次c，...
            // 从←看，记数量使用，减去index+1项：
            // index+1行 |         d          |      c      |  b  | a
            // index行   | a+b+c+d - (a) - (b)| a+b+c - (a) | a+b | a
            // 会发现减了1次a，1次b，1次c，...
            // 并且可以发现减法是从左边一步步累减过来的
            dp[index][rest] -= dp[index + 1][rest - coins[index] * (zhangs[index] + 1)];
          }
        }
      }
      return dp[0][aim];
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

    // 为了测试
    static int* randomArray(int maxLen, int maxValue, int* len)
    {
      *len = getRandom(0, maxLen);
      int* arr = (int*)malloc(sizeof(int) * (*len));
      for (int i = 0; i < *len; i++)
      {
        arr[i] = getRandom(1, maxValue);
      }
      return arr;
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
      int maxLen = 10;
      int maxValue = 20;
      int testTime = 10000;
      int len = 0;
      cout << "测试开始" <<endl;
      for (int i = 0; i < testTime; i++)
      {
        int* arr = randomArray(maxLen, maxValue, &len);
        int aim = getRandom(0, maxValue);
        int ans1 = coinsWay(arr, len, aim);
        int ans2 = dp1(arr, len, aim);
        int ans3 = dp2(arr, len, aim);
        if (ans1 != ans2 || ans1 != ans3)
        {
          cout << "Oops!" <<endl;
          printArray(arr, len);
          cout << aim <<endl;
          cout << ans1 <<endl;
          cout << ans2 <<endl;
          cout << ans3 <<endl;
          break;
        }
      }
      cout << "测试结束" <<endl;
    }
};

int main()
{
  CoinsWaySameValueSamePapper::test();
  return 0;
}
