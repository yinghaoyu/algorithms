#include <deque>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

// 给定一个数组arr，和一个正数M
// 返回在子数组长度不大于M的情况下，最大的子数组累加和
class MaxSumLengthNoMore
{
 public:
  // O(N^2)的解法，暴力解，用作对数器
  static int test(vector<int> &arr, int M)
  {
    if (arr.size() == 0 || M < 1)
    {
      return 0;
    }
    int N = arr.size();
    int max = INT32_MIN;
    for (int L = 0; L < N; L++)
    {
      int sum = 0;
      for (int R = L; R < N; R++)
      {
        if (R - L + 1 > M)  // 边界判断
        {
          break;
        }
        sum += arr[R];  // M个连续累加和
        max = std::max(max, sum);
      }
    }
    return max;
  }

  // O(N)的解法，最优解
  static int maxSum(vector<int> &arr, int M)
  {
    if (arr.size() == 0 || M < 1)
    {
      return 0;
    }
    int N = arr.size();
    vector<int> sum(N);
    sum[0] = arr[0];
    for (int i = 1; i < N; i++)
    {
      sum[i] = sum[i - 1] + arr[i];  // 前缀和
    }
    deque<int> qmax;
    int i = 0;
    int end = std::min(N, M);  // 取子数组、原数组两者长度的最小值，举例这里取M
    for (; i < end; i++)
    {
      // 先让前M个前缀和构建单调递增栈
      while (!qmax.empty() && sum[qmax.back()] <= sum[i])
      {
        qmax.pop_back();
      }
      qmax.push_back(i);
    }
    int max = sum[qmax.front()];
    int L = 0;
    for (; i < N; L++, i++)  // 左边界是L，右边界是i，i - L == M，这里保证了子数组长度不超过M
    {
      if (qmax.front() == L)  // 边界处理，L马上++，失效的下标需要删除
      {
        qmax.pop_front();
      }
      // 单调递增栈
      while (!qmax.empty() && sum[qmax.back()] <= sum[i])
      {
        qmax.pop_back();
      }
      qmax.push_back(i);
      max = std::max(max, sum[qmax.front()] - sum[L]);
    }
    for (; L < N - 1; L++)  // 左边界也需要走完
    {
      if (qmax.front() == L)
      {
        qmax.pop_front();
      }
      max = std::max(max, sum[qmax.front()] - sum[L]);
    }
    return max;
  }

  // 用作测试
  static vector<int> randomArray(int len, int max)
  {
    vector<int> arr(len);
    for (int i = 0; i < len; i++)
    {
      arr[i] = getRandom(0, max) - getRandom(0, max);
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

  // 用作测试
  static void test()
  {
    int maxN = 50;
    int maxValue = 100;
    int testTime = 1000000;
    cout << "测试开始" << endl;
    for (int i = 0; i < testTime; i++)
    {
      int N = getRandom(0, maxN);
      int M = getRandom(0, maxN);
      vector<int> arr = randomArray(N, maxValue);
      int ans1 = test(arr, M);
      int ans2 = maxSum(arr, M);
      if (ans1 != ans2)
      {
        cout << ans1 << endl;
        cout << ans2 << endl;
        cout << "Oops!" << endl;
      }
    }
    cout << "测试结束" << endl;
  }
};

int main()
{
  MaxSumLengthNoMore::test();
  return 0;
}
