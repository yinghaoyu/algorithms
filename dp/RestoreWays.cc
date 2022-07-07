#include <iostream>
#include <random>
#include <vector>

#define MAX_SEED 1024
#define HALF_SEED (MAX_SEED >> 1)

using namespace std;
using TdArray = vector<vector<int>>;
using TtArray = vector<vector<vector<int>>>;

// 整型数组arr长度为n(3 <= n <= 10^4)，最初每个数字是<=200的正数且满足如下条件：
// 1. 0位置的要求：arr[0]<=arr[1]▫
// 2. n-1位置的要求：arr[n-1]<=arr[n-2]
// 3. 中间i位置的要求：arr[i]<=max(arr[i-1],arr[i+1])▫
// 但是在arr有些数字丢失了，比如k位置的数字之前是正数，丢失之后k位置的数字为0
// 请你根据上述条件，计算可能有多少种不同的arr可以满足以上条件
// 比如 [6,0,9] 只有还原成 [6,9,9]满足全部三个条件，所以返回1种，即[6,9,9]达标

class RestoreWays
{
 public:
  static int ways0(vector<int> &arr) { return process0(arr, 0); }

  static int process0(vector<int> &arr, int index)
  {
    if (index == arr.size())
    {
      // 所有位置合法，就返回1种方法
      return isValid(arr) ? 1 : 0;
    }
    else
    {
      if (arr[index] != 0)  // 没有丢失不需要处理
      {
        return process0(arr, index + 1);
      }
      else
      {
        // 进这里说明index位置的数丢失了
        int ways = 0;
        for (int v = 1; v < 201; v++)  // 暴力枚举验证
        {
          arr[index] = v;
          ways += process0(arr, index + 1);
        }
        arr[index] = 0;  // 恢复
        return ways;
      }
    }
  }

  // 诊断条件
  static bool isValid(vector<int> &arr)
  {
    if (arr[0] > arr[1])
    {
      return false;
    }
    if (arr[arr.size() - 1] > arr[arr.size() - 2])
    {
      return false;
    }
    for (int i = 1; i < arr.size() - 1; i++)
    {
      if (arr[i] > std::max(arr[i - 1], arr[i + 1]))
      {
        return false;
      }
    }
    return true;
  }

  // 从右往左尝试
  static int ways1(vector<int> &arr)
  {
    int N = arr.size();
    if (arr[N - 1] != 0)
    {
      // 2 的含义为arr[N-2] > arr[N-1]
      return process1(arr, N - 1, arr[N - 1], 2);
    }
    else
    {
      // 最后1个数丢失了
      int ways = 0;
      for (int v = 1; v < 201; v++)
      {
        // 2 的含义为arr[N-2] > arr[N-1]
        ways += process1(arr, N - 1, v, 2);
      }
      return ways;
    }
  }

  // 如果i位置的数字变成了v,
  // 并且arr[i]和arr[i+1]的关系为s，
  // s==0，代表arr[i] < arr[i+1] 右大
  // s==1，代表arr[i] == arr[i+1] 右=当前
  // s==2，代表arr[i] > arr[i+1] 右小
  // 返回0...i范围上有多少种有效的转化方式？
  static int process1(vector<int> &arr, int i, int v, int s)
  {
    if (i == 0)
    {
      // 0...i 只剩一个数了，0...0
      return ((s == 0 || s == 1) && (arr[0] == 0 || v == arr[0])) ? 1 : 0;
    }
    // i > 0
    if (arr[i] != 0 && v != arr[i])
    {
      return 0;
    }
    // i>0 ，并且， i位置的数真的可以变成V，
    int ways = 0;
    if (s == 0 || s == 1)
    {
      // [i] -> V <= [i+1]
      for (int pre = 1; pre < 201; pre++)
      {
        ways += process1(arr, i - 1, pre, pre < v ? 0 : (pre == v ? 1 : 2));
      }
    }
    else
    {
      // ? 当前 > 右 当前 <= max{左，右}
      for (int pre = v; pre < 201; pre++)
      {
        ways += process1(arr, i - 1, pre, pre == v ? 1 : 2);
      }
    }
    return ways;
  }

  static int zuo(vector<int> &arr, int i, int v, int s)
  {
    if (i == 0)
    {
      // 0...i 只剩一个数了，0...0
      return ((s == 0 || s == 1) && (arr[0] == 0 || v == arr[0])) ? 1 : 0;
    }
    // i > 0
    if (arr[i] != 0 && v != arr[i])
    {
      return 0;
    }
    // i>0 ，并且， i位置的数真的可以变成V，
    int ways = 0;
    if (s == 0 || s == 1)
    {
      // [i] -> V <= [i+1]
      for (int pre = 1; pre < v; pre++)
      {
        ways += zuo(arr, i - 1, pre, 0);
      }
    }
    ways += zuo(arr, i - 1, v, 1);
    for (int pre = v + 1; pre < 201; pre++)
    {
      ways += zuo(arr, i - 1, pre, 2);
    }
    return ways;
  }

  static int ways2(vector<int> &arr)
  {
    int N = arr.size();
    TtArray dp(N, vector<vector<int>>(201, vector<int>(3)));
    if (arr[0] != 0)
    {
      dp[0][arr[0]][0] = 1;
      dp[0][arr[0]][1] = 1;
    }
    else
    {
      for (int v = 1; v < 201; v++)
      {
        dp[0][v][0] = 1;
        dp[0][v][1] = 1;
      }
    }
    for (int i = 1; i < N; i++)
    {
      for (int v = 1; v < 201; v++)
      {
        for (int s = 0; s < 3; s++)
        {
          if (arr[i] == 0 || v == arr[i])
          {
            if (s == 0 || s == 1)
            {
              for (int pre = 1; pre < v; pre++)
              {
                dp[i][v][s] += dp[i - 1][pre][0];
              }
            }
            dp[i][v][s] += dp[i - 1][v][1];
            for (int pre = v + 1; pre < 201; pre++)
            {
              dp[i][v][s] += dp[i - 1][pre][2];
            }
          }
        }
      }
    }
    if (arr[N - 1] != 0)
    {
      return dp[N - 1][arr[N - 1]][2];
    }
    else
    {
      int ways = 0;
      for (int v = 1; v < 201; v++)
      {
        ways += dp[N - 1][v][2];
      }
      return ways;
    }
  }

  static int ways3(vector<int> &arr)
  {
    int N = arr.size();
    TtArray dp(N, vector<vector<int>>(201, vector<int>(3)));
    if (arr[0] != 0)
    {
      dp[0][arr[0]][0] = 1;
      dp[0][arr[0]][1] = 1;
    }
    else
    {
      for (int v = 1; v < 201; v++)
      {
        dp[0][v][0] = 1;
        dp[0][v][1] = 1;
      }
    }
    TdArray presum(201, vector<int>(3));
    for (int v = 1; v < 201; v++)
    {
      for (int s = 0; s < 3; s++)
      {
        presum[v][s] = presum[v - 1][s] + dp[0][v][s];
      }
    }
    for (int i = 1; i < N; i++)
    {
      for (int v = 1; v < 201; v++)
      {
        for (int s = 0; s < 3; s++)
        {
          if (arr[i] == 0 || v == arr[i])
          {
            if (s == 0 || s == 1)
            {
              dp[i][v][s] += sum(1, v - 1, 0, presum);
            }
            dp[i][v][s] += dp[i - 1][v][1];
            dp[i][v][s] += sum(v + 1, 200, 2, presum);
          }
        }
      }
      for (int v = 1; v < 201; v++)
      {
        for (int s = 0; s < 3; s++)
        {
          presum[v][s] = presum[v - 1][s] + dp[i][v][s];
        }
      }
    }
    if (arr[N - 1] != 0)
    {
      return dp[N - 1][arr[N - 1]][2];
    }
    else
    {
      return sum(1, 200, 2, presum);
    }
  }

  static int sum(int begin, int end, int relation, TdArray &presum) { return presum[end][relation] - presum[begin - 1][relation]; }

  // for test
  static vector<int> generateRandomArray(int len)
  {
    vector<int> ans(len);
    for (int i = 0; i < ans.size(); i++)
    {
      if (getRandom(0, MAX_SEED) < HALF_SEED)
      {
        ans[i] = 0;
      }
      else
      {
        ans[i] = getRandom(0, 200);
      }
    }
    return ans;
  }
  // for test

  static void printArray(vector<int> &arr)
  {
    cout << "arr size : " << arr.size();
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
    int len = 4;
    int testTime = 15;
    cout << "功能测试开始" << endl;
    for (int i = 0; i < testTime; i++)
    {
      int N = getRandom(2, len + 1);
      vector<int> arr = generateRandomArray(N);
      int ans0 = ways0(arr);
      int ans1 = ways1(arr);
      int ans2 = ways2(arr);
      int ans3 = ways3(arr);
      // cout << ans0 << " " << ans1 << " " << ans2 << " " << ans3 << endl;
      if (ans0 != ans1 || ans2 != ans3 || ans0 != ans2)
      {
        cout << "Oops!" << endl;
      }
    }
    cout << "功能测试结束" << endl;
    cout << "===========" << endl;
    int N = 100000;
    vector<int> arr = generateRandomArray(N);
    clock_t begin = clock();
    ways3(arr);
    clock_t end = clock();
    cout << "run time : " << static_cast<double>(end - begin) / CLOCKS_PER_SEC << " ms" << endl;
  }
};

int main()
{
  RestoreWays::test();
  return 0;
}
