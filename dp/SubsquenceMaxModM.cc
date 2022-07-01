#include <iostream>
#include <random>
#include <set>
#include <unordered_set>
#include <vector>

using namespace std;

using TdArray = vector<vector<bool>>;

// 子数组的定义：一个或连续多个数组中的元素组成一个子数组(子数组最少包含一个元素)
// 子序列的定义：去除某些元素但不破坏余下元素的相对位置（在前或在后）而形成的新序列

// 给定一个非负数组arr，和一个正数m。 返回arr的所有子序列中累加和%m之后的最大值。
class SubsquenceMaxModM
{
 public:
  // 暴力递归
  static int max1(vector<int> &arr, int m)
  {
    unordered_set<int> set;
    process(arr, 0, 0, set);
    int max = 0;
    for (int sum : set)
    {
      max = std::max(max, sum % m);
    }
    return max;
  }

  static void process(vector<int> &arr, int index, int sum, unordered_set<int> &set)
  {
    if (index == arr.size())
    {
      set.emplace(sum);
    }
    else
    {
      process(arr, index + 1, sum, set);
      process(arr, index + 1, sum + arr[index], set);
    }
  }

  // 枚举所有可能性
  static int max2(vector<int> arr, int m)
  {
    int sum = 0;
    int N = arr.size();
    for (int i = 0; i < N; i++)
    {
      // 累加和
      sum += arr[i];
    }
    TdArray dp(N, vector<bool>(sum + 1));
    for (int i = 0; i < N; i++)
    {
      // 累加和为0，可能任何情况都能取到
      dp[i][0] = true;
    }
    dp[0][arr[0]] = true;  // index为0，累加和为arr[0]能取到
    for (int i = 1; i < N; i++)
    {
      for (int j = 1; j <= sum; j++)
      {
        dp[i][j] = dp[i - 1][j];  //不取当前arr[i]
        if (j - arr[i] >= 0)
        {
          dp[i][j] = dp[i][j] | dp[i - 1][j - arr[i]];  // 取当前arr[i]
        }
      }
    }
    int ans = 0;
    for (int j = 0; j <= sum; j++)
    {
      if (dp[N - 1][j])  // 能取到的情况下，找出最大值
      {
        ans = std::max(ans, j % m);
      }
    }
    return ans;
  }

  // 缩减数组
  static int max3(vector<int> &arr, int m)
  {
    int N = arr.size();
    // 0...m-1
    TdArray dp(N, vector<bool>(m));
    for (int i = 0; i < N; i++)
    {
      dp[i][0] = true;
    }
    dp[0][arr[0] % m] = true;
    for (int i = 1; i < N; i++)
    {
      for (int j = 1; j < m; j++)
      {
        // dp[i][j] T or F
        dp[i][j] = dp[i - 1][j];
        int cur = arr[i] % m;
        if (cur <= j)
        {
          dp[i][j] = dp[i][j] | dp[i - 1][j - cur];
        }
        else
        {
          dp[i][j] = dp[i][j] | dp[i - 1][m + j - cur];  // j - cur < 0补上 m
        }
      }
    }
    int ans = 0;
    for (int i = 0; i < m; i++)
    {
      if (dp[N - 1][i])
      {
        ans = i;
      }
    }
    return ans;
  }

  // 如果arr的累加和很大，m也很大
  // 但是arr的长度相对不大
  // 分治法
  static int max4(vector<int> &arr, int m)
  {
    if (arr.size() == 1)
    {
      return arr[0] % m;
    }
    int mid = (arr.size() - 1) / 2;
    set<int> sortSet1;
    process4(arr, 0, 0, mid, m, sortSet1);
    set<int> sortSet2;
    process4(arr, mid + 1, 0, arr.size() - 1, m, sortSet2);
    int ans = 0;
    for (int leftMod : sortSet1)
    {
      // 找到第一个 > 的位置
      auto floor = sortSet2.upper_bound(m - 1 - leftMod);
      // floor--后，floor在最近的 <= 的位置，相当于floorKey
      floor--;
      ans = std::max(ans, leftMod + *floor);
    }
    return ans;
  }

  // 从index出发，最后有边界是end+1，arr[index...end]
  static void process4(vector<int> &arr, int index, int sum, int end, int m, set<int> &sortSet)
  {
    if (index == end + 1)
    {
      sortSet.emplace(sum % m);
    }
    else
    {
      process4(arr, index + 1, sum, end, m, sortSet);
      process4(arr, index + 1, sum + arr[index], end, m, sortSet);
    }
  }

  static vector<int> generateRandomArray(int len, int value)
  {
    vector<int> ans(getRandom(1, len));
    for (int i = 0; i < ans.size(); i++)
    {
      ans[i] = getRandom(0, value);
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
    int len = 10;
    int value = 100;
    int m = 76;
    int testTime = 500000;
    cout << "test begin" << endl;
    for (int i = 0; i < testTime; i++)
    {
      vector<int> arr = generateRandomArray(len, value);
      int ans1 = max1(arr, m);
      int ans2 = max2(arr, m);
      int ans3 = max3(arr, m);
      int ans4 = max4(arr, m);
      if (ans1 != ans2 || ans2 != ans3 || ans3 != ans4)
      {
        cout << "Oops!" << endl;
      }
    }
    cout << "test finish!" << endl;
  }
};

int main()
{
  SubsquenceMaxModM::test();
  return 0;
}
