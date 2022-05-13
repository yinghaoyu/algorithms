#include <cstdint>
#include <iostream>
#include <stdbool.h>
#include <random>
#include <memory.h>

using namespace std;

class MinCoinsNoLimit
{
  public:
    static int minCoins(int* arr, int len, int aim)
    {
      return process(arr, len, 0, aim);
    }

    // arr[index...]面值，每种面值张数自由选择，
    // 搞出rest正好这么多钱，返回最小张数
    // 拿INT32_MAX标记怎么都搞定不了
    static int process(int* arr, int len, int index, int rest)
    {
      if (index == len)
      {
        // base case
        // rest == 0 说明前[0~index-1]刚好满足了aim，当前index只需要0张
        return rest == 0 ? 0 : INT32_MAX;
      }
      else
      {
        int ans = INT32_MAX;
        for (int zhang = 0; zhang * arr[index] <= rest; zhang++)
        {
          int next = process(arr, len, index + 1, rest - zhang * arr[index]);
          if (next != INT32_MAX)
          {
            // 当前的张数 + 后续的张数
            ans = std::min(ans, zhang + next);
          }
        }
        return ans;
      }
    }

    static int dp1(int* arr, int len, int aim)
    {
      if (aim == 0)
      {
        return 0;
      }
      int N = len;
      int** dp = mallocArray(N + 1, aim + 1);
      dp[N][0] = 0;
      for (int j = 1; j <= aim; j++)
      {
        // 任意面值硬币都不能拼出
        dp[N][j] = INT32_MAX;
      }
      for (int index = N - 1; index >= 0; index--)
      {
        for (int rest = 0; rest <= aim; rest++)
        {
          int ans = INT32_MAX;
          for (int zhang = 0; zhang * arr[index] <= rest; zhang++)
          {
            int next = dp[index + 1][rest - zhang * arr[index]];
            if (next != INT32_MAX)
            {
              ans = std::min(ans, zhang + next);
            }
          }
          dp[index][rest] = ans;
        }
      }
      int ans =dp[0][aim];
      freeArray(dp, N + 1);
      return ans;
    }

    static int dp2(int* arr, int len, int aim)
    {
      if (aim == 0)
      {
        return 0;
      }
      int N = len;
      int** dp = mallocArray(N + 1, aim + 1);
      dp[N][0] = 0;
      for (int j = 1; j <= aim; j++)
      {
        dp[N][j] = INT32_MAX;
      }
      for (int index = N - 1; index >= 0; index--)
      {
        for (int rest = 0; rest <= aim; rest++)
        {
          dp[index][rest] = dp[index + 1][rest];
          if (rest - arr[index] >= 0
              && dp[index][rest - arr[index]] != INT32_MAX)
          {
            // 斜率优化
            // 仔细想一想为什么这里 + 1 ？
            // 从N-1 ~ index累计增加了 rest / arr[index] 次
            dp[index][rest] = std::min(dp[index][rest], dp[index][rest - arr[index]] + 1);
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
      bool* has = (bool*)malloc(sizeof(bool) * (maxValue + 1));
      for (int i = 0; i < *len; i++)
      {
        do
        {
          arr[i] = getRandom(1, maxValue);
        } while (has[arr[i]]);
        has[arr[i]] = true;
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
      int testTime = 30000;
      int len = 0;
      cout << "功能测试开始" <<endl;
      for (int i = 0; i < testTime; i++)
      {
        int N = getRandom(0, maxLen);
        int* arr = randomArray(N, maxValue, &len);
        int aim = getRandom(0, maxValue);
        int ans1 = minCoins(arr, len, aim);
        int ans2 = dp1(arr, len, aim);
        int ans3 = dp2(arr, len, aim);
        if (ans1 != ans2 || ans1 != ans3)
        {
          cout << "Oops!" <<endl;
          printArray(arr, len);
          cout << aim <<endl;
          cout << ans1 <<endl;
          cout << ans2 <<endl;
          break;
        }
      }
      cout << "功能测试结束" <<endl;
    }
};

int main()
{
  MinCoinsNoLimit::test();
  return 0;
}
