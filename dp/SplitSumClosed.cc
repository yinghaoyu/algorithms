#include <iostream>
#include <random>
#include <vector>

using namespace std;
using TdArray = vector<vector<int>>;

class SplitSumClosed
{
 public:
  static int right(vector<int> &arr)
  {
    if (arr.size() < 2)
    {
      return 0;
    }
    int sum = 0;
    for (int i = 0; i < arr.size(); i++)
    {
      sum += arr[i];
    }
    return process(arr, 0, sum / 2);
  }

  // arr[i...]可以自由选择，请返回累加和尽量接近rest，但不能超过rest的情况下，最接近的累加和是多少？
  static int process(vector<int> &arr, int i, int rest)
  {
    if (i == arr.size())
    {
      return 0;
    }
    else
    {
      // 还有数，arr[i]这个数
      // 可能性1，不使用arr[i]
      int p1 = process(arr, i + 1, rest);
      // 可能性2，要使用arr[i]
      int p2 = 0;
      if (arr[i] <= rest)
      {
        p2 = arr[i] + process(arr, i + 1, rest - arr[i]);
      }
      return std::max(p1, p2);
    }
  }

  static int dp(vector<int> &arr)
  {
    if (arr.size() < 2)
    {
      return 0;
    }
    int sum = 0;
    for (int i = 0; i < arr.size(); i++)
    {
      sum += arr[i];
    }
    sum /= 2;
    int N = arr.size();
    TdArray dp(N + 1, vector<int>(sum + 1));
    for (int i = N - 1; i >= 0; i--)
    {
      for (int rest = 0; rest <= sum; rest++)
      {
        // 可能性1，不使用arr[i]
        int p1 = dp[i + 1][rest];
        // 可能性2，要使用arr[i]
        int p2 = 0;
        if (arr[i] <= rest)
        {
          p2 = arr[i] + dp[i + 1][rest - arr[i]];
        }
        dp[i][rest] = std::max(p1, p2);
      }
    }
    return dp[0][sum];
  }

  static vector<int> randomArray(int len, int value)
  {
    vector<int> arr(len);
    for (int i = 0; i < len; i++)
    {
      arr[i] = getRandom(0, value);
    }
    return arr;
  }

  static void printArray(vector<int> &arr)
  {
    for (int i = 0; i < arr.size(); i++)
    {
      cout << arr[i] << " ";
    }
    cout << endl;
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
    int maxLen = 20;
    int maxValue = 50;
    int testTime = 10000;
    cout << "测试开始" << endl;
    for (int i = 0; i < testTime; i++)
    {
      int len = getRandom(0, maxLen);
      vector<int> arr = randomArray(len, maxValue);
      int ans1 = right(arr);
      int ans2 = dp(arr);
      if (ans1 != ans2)
      {
        printArray(arr);
        cout << ans1 << endl;
        cout << ans2 << endl;
        cout << "Oops!" << endl;
        break;
      }
    }
    cout << "测试结束" << endl;
  }
};

int main()
{
  SplitSumClosed::test();
  return 0;
}
