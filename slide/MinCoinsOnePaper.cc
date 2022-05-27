#include <memory.h>
#include <cstdint>
#include <deque>
#include <iostream>
#include <random>
#include <unordered_map>

using namespace std;

// 题目描述：
// arr是货币数组，其中的值都是正数。再给定一个正数aim。
// 每个值都认为是一张货币
// 返回组成aim的最少货币数
// 注意：因为是求最少货币数，所以每一张货币认为相同或者不同就不重要了

class MinCoinsOnePaper
{
 public:
  static int minCoins(int *arr, int len, int aim) { return process(arr, len, 0, aim); }

  // 计算达到rest最少需要多少张货币
  static int process(int *arr, int len, int index, int rest)
  {
    if (rest < 0)
    {
      return INT32_MAX;
    }
    if (index == len)
    {
      return rest == 0 ? 0 : INT32_MAX;
    }
    else
    {
      int p1 = process(arr, len, index + 1, rest);               // 不使用当前
      int p2 = process(arr, len, index + 1, rest - arr[index]);  // 使用当前
      if (p2 != INT32_MAX)
      {
        p2++;  // 加上当前货币张数1
      }
      return std::min(p1, p2);
    }
  }

  // for test
  static int **mallocArray(int row, int column)
  {
    int **arr = (int **) malloc(sizeof(int *) * row);
    for (int i = 0; i < row; i++)
    {
      arr[i] = (int *) malloc(sizeof(int) * column);
      memset(arr[i], 0, sizeof(int) * column);
    }
    return arr;
  }

  static void freeArray(int **arr, int row)
  {
    for (int i = 0; i < row; i++)
    {
      free(arr[i]);
    }
    free(arr);
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
  static void printArray(int *arr, int len)
  {
    for (int i = 0; i < len; i++)
    {
      cout << arr[i] << " ";
    }
    cout << endl;
  }

  // dp1时间复杂度为：O(arr长度 * aim)
  static int dp1(int *arr, int len, int aim)
  {
    if (aim == 0)
    {
      return 0;
    }
    int N = len;
    int **dp = mallocArray(N + 1, aim + 1);
    dp[N][0] = 0;  // base case，表示没有可用货币index=N且rest=0时，方案是0张
    for (int j = 1; j <= aim; j++)
    {
      // 没有可用货币，但rest != 0，方案不达标，由于求的是最小值，所以用INT32_MAX标记
      dp[N][j] = INT32_MAX;
    }
    for (int index = N - 1; index >= 0; index--)
    {
      for (int rest = 0; rest <= aim; rest++)
      {
        // 不用当前货币
        int p1 = dp[index + 1][rest];
        // 使用当前货币
        int p2 = rest - arr[index] >= 0 ? dp[index + 1][rest - arr[index]] : INT32_MAX;
        if (p2 != INT32_MAX)
        {
          p2++;  // 使用当前货币，张数+1
        }
        dp[index][rest] = std::min(p1, p2);
      }
    }
    int ans = dp[0][aim];
    freeArray(dp, N + 1);
    return ans;
  }

  class Info
  {
   public:
    int *coins;
    int *zhangs;
    int len;

    Info(int *c, int *z, int l)
    {
      coins = c;
      zhangs = z;
      len = l;
    }
  };

  // 统计
  static Info getInfo(int *arr, int len)
  {
    unordered_map<int, int> counts;
    for (int i = 0; i < len; i++)
    {
      if (counts.find(arr[i]) == counts.end())
      {
        counts.emplace(arr[i], 1);
      }
      else
      {
        counts[arr[i]]++;
      }
    }
    int N = counts.size();
    int *coins = new int[N]();
    int *zhangs = new int[N]();
    int index = 0;
    for (auto entry : counts)
    {
      coins[index] = entry.first;
      zhangs[index++] = entry.second;
    }
    return Info(coins, zhangs, index);
  }

  // dp2时间复杂度为：O(arr长度) + O(货币种数 * aim * 每种货币的平均张数)
  static int dp2(int *arr, int len, int aim)
  {
    if (aim == 0)
    {
      return 0;
    }
    // 得到info时间复杂度O(arr长度)
    Info info = getInfo(arr, len);
    int *coins = info.coins;
    int *zhangs = info.zhangs;
    int N = info.len;
    int **dp = mallocArray(N + 1, aim + 1);
    dp[N][0] = 0;
    for (int j = 1; j <= aim; j++)
    {
      dp[N][j] = INT32_MAX;
    }
    // 这三层for循环，时间复杂度为O(货币种数 * aim * 每种货币的平均张数)
    for (int index = N - 1; index >= 0; index--)
    {
      for (int rest = 0; rest <= aim; rest++)
      {
        dp[index][rest] = dp[index + 1][rest];
        for (int zhang = 1; zhang * coins[index] <= aim && zhang <= zhangs[index]; zhang++)
        {
          if (rest - zhang * coins[index] >= 0 && dp[index + 1][rest - zhang * coins[index]] != INT32_MAX)
          {
            dp[index][rest] = std::min(dp[index][rest], zhang + dp[index + 1][rest - zhang * coins[index]]);
          }
        }
      }
    }
    int ans = dp[0][aim];
    freeArray(dp, N + 1);
    return ans;
  }

  // dp3时间复杂度为：O(arr长度) + O(货币种数 * aim)
  // 优化需要用到窗口内最小值的更新结构
  static int dp3(int *arr, int len, int aim)
  {
    if (aim == 0)
    {
      return 0;
    }
    // 得到info时间复杂度O(arr长度)
    Info info = getInfo(arr, len);
    int *c = info.coins;
    int *z = info.zhangs;
    int N = info.len;
    int **dp = mallocArray(N + 1, aim + 1);
    dp[N][0] = 0;
    for (int j = 1; j <= aim; j++)
    {
      dp[N][j] = INT32_MAX;
    }
    // 虽然是嵌套了很多循环，但是时间复杂度为O(货币种数 * aim)
    // 因为用了窗口内最小值的更新结构
    for (int i = N - 1; i >= 0; i--)
    {
      // 这里为何取 std::min(aim + 1, c[i]) ？
      for (int mod = 0; mod < std::min(aim + 1, c[i]); mod++)
      {
        // 当前面值 X
        // mod  mod + x   mod + 2*x   mod + 3 * x
        deque<int> w;
        w.push_back(mod);
        dp[i][mod] = dp[i + 1][mod];
        // 往右扩，窗口长度为z[i]
        for (int r = mod + c[i]; r <= aim; r += c[i])
        {
          // 构建单调递增栈
          while (!w.empty() && (dp[i + 1][w.back()] == INT32_MAX || dp[i + 1][w.back()] + compensate(w.back(), r, c[i]) >= dp[i + 1][r]))
          {
            w.pop_back();
          }
          w.push_back(r);
          int overdue = r - c[i] * (z[i] + 1);
          if (w.front() == overdue)
          {
            // 过期的最小值就剔除
            w.pop_front();
          }
          dp[i][r] = dp[i + 1][w.front()] + compensate(w.front(), r, c[i]);
        }
      }
    }
    int ans = dp[0][aim];
    freeArray(dp, N + 1);
    return ans;
  }

  // 返回补偿的货币数，计算cur到pre之间所需的货币张数
  static int compensate(int pre, int cur, int coin) { return (cur - pre) / coin; }

  // 为了测试
  static int *randomArray(int N, int maxValue)
  {
    int *arr = new int[N];
    for (int i = 0; i < N; i++)
    {
      arr[i] = getRandom(1, maxValue);  // 不能为0
    }
    return arr;
  }

  // 为了测试
  static void test()
  {
    int maxLen = 20;
    int maxValue = 30;
    int testTime = 100000;
    cout << "功能测试开始" << endl;
    for (int i = 0; i < testTime; i++)
    {
      int N = getRandom(0, maxLen);
      int *arr = randomArray(N, maxValue);
      int aim = getRandom(0, maxValue);
      int ans1 = minCoins(arr, N, aim);
      int ans2 = dp1(arr, N, aim);
      int ans3 = dp2(arr, N, aim);
      int ans4 = dp3(arr, N, aim);
      if (ans1 != ans2 || ans3 != ans4 || ans1 != ans3)
      {
        cout << "Oops!" << endl;
        printArray(arr, N);
        cout << aim << endl;
        cout << ans1 << endl;
        cout << ans2 << endl;
        cout << ans3 << endl;
        cout << ans4 << endl;
        break;
      }
    }
    cout << "测试功能结束" << endl;
    cout << "==========" << endl;

    int aim = 0;
    int *arr = nullptr;
    clock_t start;
    clock_t end;
    double elapsedTime;
    int ans2;
    int ans3;

    cout << "性能测试开始" << endl;
    maxLen = 30000;
    maxValue = 20;
    aim = 60000;
    arr = randomArray(maxLen, maxValue);
    start = clock();
    ans2 = dp2(arr, maxLen, aim);
    end = clock();
    elapsedTime = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    cout << "dp2答案 : " << ans2 << ", dp2运行时间 : " << elapsedTime << " ms" << endl;
    start = clock();
    ans3 = dp3(arr, maxLen, aim);
    end = clock();
    elapsedTime = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    cout << "dp3答案 : " << ans3 << ", dp3运行时间 : " << elapsedTime << " ms" << endl;
    cout << "性能测试结束" << endl;

    cout << "===========" << endl;

    cout << "货币大量重复出现情况下，" << endl;
    cout << "大数据量测试dp3开始" << endl;
    maxLen = 20000000;
    aim = 10000;
    maxValue = 10000;
    arr = randomArray(maxLen, maxValue);
    start = clock();
    ans3 = dp3(arr, maxLen, aim);
    end = clock();
    elapsedTime = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    cout << "dp3运行时间 : " << elapsedTime << " ms" << endl;
    cout << "大数据量测试dp3结束" << endl;

    cout << "===========" << endl;

    cout << "当货币很少出现重复，dp2比dp3有常数时间优势" << endl;
    cout << "当货币大量出现重复，dp3时间复杂度明显优于dp2" << endl;
    cout << "dp3的优化用到了窗口内最小值的更新结构" << endl;
  }
};

int main()
{
  MinCoinsOnePaper::test();
  return 0;
}
