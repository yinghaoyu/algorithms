#include <memory.h>
#include <stdbool.h>
#include <iostream>
#include <random>
#include <unordered_map>

using namespace std;
using TdArray = vector<vector<int>>;

class CoinsWaySameValueSamePapper
{
 public:
  class Info
  {
   public:
    vector<int> coins;   // coins[i]不同的面值
    vector<int> zhangs;  // zhangs[i]面值的张数

    Info(vector<int> &c, vector<int> &z)
    {
      coins = c;
      zhangs = z;
    }
  };

  // 去重
  static Info getInfo(vector<int> &arr)
  {
    unordered_map<int, int> counts;
    for (int i = 0; i < arr.size(); i++)
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
    vector<int> coins(N);
    vector<int> zhangs(N);
    int index = 0;
    for (auto cur : counts)
    {
      coins[index] = cur.first;
      zhangs[index++] = cur.second;
    }
    return Info(coins, zhangs);
  }

  static int coinsWay(vector<int> &arr, int aim)
  {
    if (arr.size() == 0 || aim < 0)
    {
      return 0;
    }
    Info info = getInfo(arr);
    return process(info.coins, info.zhangs, 0, aim);
  }

  // coins 面值数组，正数且去重
  // zhangs 每种面值对应的张数
  static int process(vector<int> &coins, vector<int> &zhangs, int index, int rest)
  {
    if (index == coins.size())
    {
      return rest == 0 ? 1 : 0;
    }
    int ways = 0;
    for (int zhang = 0; zhang * coins[index] <= rest && zhang <= zhangs[index]; zhang++)
    {
      ways += process(coins, zhangs, index + 1, rest - (zhang * coins[index]));
    }
    return ways;
  }

  static int dp1(vector<int> &arr, int aim)
  {
    if (arr.size() == 0 || aim < 0)
    {
      return 0;
    }
    Info info = getInfo(arr);
    vector<int> coins = info.coins;
    vector<int> zhangs = info.zhangs;
    int N = coins.size();
    TdArray dp(N + 1, vector<int>(aim + 1));
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
    return dp[0][aim];
  }

  static int dp2(vector<int> &arr, int aim)
  {
    if (arr.size() == 0 || aim < 0)
    {
      return 0;
    }

    Info info = getInfo(arr);
    vector<int> coins = info.coins;
    vector<int> zhangs = info.zhangs;
    int N = info.coins.size();
    TdArray dp(N + 1, vector<int>(aim + 1));
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

  // 为了测试
  static vector<int> randomArray(int maxLen, int maxValue, int *len)
  {
    *len = getRandom(0, maxLen);
    vector<int> arr(*len);
    for (int i = 0; i < *len; i++)
    {
      arr[i] = getRandom(1, maxValue);
    }
    return arr;
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
  static void printArray(vector<int> &arr)
  {
    for (int i = 0; i < arr.size(); i++)
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
    cout << "测试开始" << endl;
    for (int i = 0; i < testTime; i++)
    {
      vector<int> arr = randomArray(maxLen, maxValue, &len);
      int aim = getRandom(0, maxValue);
      int ans1 = coinsWay(arr, aim);
      int ans2 = dp1(arr, aim);
      int ans3 = dp2(arr, aim);
      if (ans1 != ans2 || ans1 != ans3)
      {
        cout << "Oops!" << endl;
        printArray(arr);
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
  CoinsWaySameValueSamePapper::test();
  return 0;
}
