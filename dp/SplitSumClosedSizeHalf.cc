#include <iostream>
#include <random>
#include <vector>

using namespace std;
using TdArray = vector<vector<int>>;
using TtArray = vector<vector<vector<int>>>;

class SplitSumClosedSizeHalf
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
    if ((arr.size() & 1) == 0)
    {
      // 偶数长度
      return process(arr, 0, arr.size() / 2, sum / 2);
    }
    else
    {
      // 奇数长度
      return std::max(process(arr, 0, arr.size() / 2, sum / 2), process(arr, 0, arr.size() / 2 + 1, sum / 2));
    }
  }

  // arr[i....]自由选择，挑选的个数一定要是picks个，累加和<=rest, 离rest最近的返回
  static int process(vector<int> &arr, int i, int picks, int rest)
  {
    if (i == arr.size())
    {
      return picks == 0 ? 0 : -1;
    }
    else
    {
      int p1 = process(arr, i + 1, picks, rest);
      // 就是要使用arr[i]这个数
      int p2 = -1;
      int next = -1;
      if (arr[i] <= rest)
      {
        next = process(arr, i + 1, picks - 1, rest - arr[i]);
      }
      if (next != -1)
      {
        p2 = arr[i] + next;
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
    int M = (N + 1) / 2;
    TtArray dp(N + 1, vector<vector<int>>(M + 1, vector<int>(sum + 1, -1)));
    for (int rest = 0; rest <= sum; rest++)
    {
      dp[N][0][rest] = 0;
    }
    for (int i = N - 1; i >= 0; i--)
    {
      for (int picks = 0; picks <= M; picks++)
      {
        for (int rest = 0; rest <= sum; rest++)
        {
          int p1 = dp[i + 1][picks][rest];
          // 就是要使用arr[i]这个数
          int p2 = -1;
          int next = -1;
          if (picks - 1 >= 0 && arr[i] <= rest)
          {
            next = dp[i + 1][picks - 1][rest - arr[i]];
          }
          if (next != -1)
          {
            p2 = arr[i] + next;
          }
          dp[i][picks][rest] = std::max(p1, p2);
        }
      }
    }
    int ans = -1;
    if ((arr.size() & 1) == 0)
    {
      ans = dp[0][arr.size() / 2][sum];
    }
    else
    {
      ans = std::max(dp[0][arr.size() / 2][sum], dp[0][(arr.size() / 2) + 1][sum]);
    }
    return ans;
  }

  static int dp2(vector<int> &arr)
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
    sum >>= 1;
    int N = arr.size();
    int M = (arr.size() + 1) >> 1;
    TtArray dp(N + 1, vector<vector<int>>(M + 1, vector<int>(sum + 1, INT32_MIN)));
    for (int i = 0; i < N; i++)
    {
      for (int k = 0; k <= sum; k++)
      {
        dp[i][0][k] = 0;
      }
    }
    for (int k = 0; k <= sum; k++)
    {
      dp[0][1][k] = arr[0] <= k ? arr[0] : INT32_MIN;
    }
    for (int i = 1; i < N; i++)
    {
      for (int j = 1; j <= std::min(i + 1, M); j++)
      {
        for (int k = 0; k <= sum; k++)
        {
          dp[i][j][k] = dp[i - 1][j][k];
          if (k - arr[i] >= 0)
          {
            dp[i][j][k] = std::max(dp[i][j][k], dp[i - 1][j - 1][k - arr[i]] + arr[i]);
          }
        }
      }
    }
    int ans = std::max(dp[N - 1][M][sum], dp[N - 1][N - M][sum]);
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

  // for test
  static void test()
  {
    int maxLen = 10;
    int maxValue = 50;
    int testTime = 10000;
    cout << "测试开始" << endl;
    for (int i = 0; i < testTime; i++)
    {
      int len = getRandom(0, maxLen);
      vector<int> arr = randomArray(len, maxValue);
      int ans1 = right(arr);
      int ans2 = dp(arr);
      int ans3 = dp2(arr);
      if (ans1 != ans2 || ans1 != ans3)
      {
        printArray(arr);
        cout << ans1 << endl;
        cout << ans2 << endl;
        cout << ans3 << endl;
        cout << "Oops!" << endl;
        break;
      }
    }
    cout << "测试结束" << endl;
  }
};
int main()
{
  SplitSumClosedSizeHalf::test();
  return 0;
}
