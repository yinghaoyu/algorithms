#include <memory.h>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

// 题目描述：
// 正数的数组arr中任何一个子数组sub，设value = (sub累加和) * (sub中的最小值)
// 那么所有子数组中，这个value最大是多少?

class AllTimesMinToMax
{
 public:
  // 暴力解
  static int max1(int *arr, int len)
  {
    int max = INT32_MIN;
    for (int i = 0; i < len; i++)
    {
      for (int j = i; j < len; j++)
      {
        int minNum = INT32_MAX;
        int sum = 0;
        for (int k = i; k <= j; k++)
        {
          sum += arr[k];
          minNum = std::min(minNum, arr[k]);
        }
        max = std::max(max, minNum * sum);
      }
    }
    return max;
  }

  static int max2(int *arr, int len)
  {
    int size = len;
    int *sums = new int[size]();
    sums[0] = arr[0];
    for (int i = 1; i < size; i++)
    {
      // 前缀和
      sums[i] = sums[i - 1] + arr[i];
    }
    int max = INT32_MIN;
    vector<int> stack;
    for (int i = 0; i < size; i++)
    {
      // 单调递增栈
      while (!stack.empty() && arr[stack.back()] >= arr[i])
      {
        // arr[j]左边最近小值的下标是栈顶第2个元素 leftLessIndex
        // arr[j]右边最近小值是i
        // 在[leftLessIndex, i-1]之间的数都是大于arr[j]的数
        int j = stack.back();
        stack.pop_back();
        max = std::max(max, (stack.empty() ? sums[i - 1] : (sums[i - 1] - sums[stack.back()])) * arr[j]);
      }
      stack.push_back(i);
    }
    while (!stack.empty())
    {
      int j = stack.back();
      stack.pop_back();
      max = std::max(max, (stack.empty() ? sums[size - 1] : (sums[size - 1] - sums[stack.back()])) * arr[j]);
    }
    return max;
  }

  static int getRandom(int min, int max)
  {
    random_device seed;                            // 硬件生成随机数种子
    ranlux48 engine(seed());                       // 利用种子生成随机数引
    uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
    int res = distrib(engine);                     // 随机数
    return res;
  }

  static int *gerenareRondomArray(int *len)
  {
    *len = getRandom(10, 30);
    int *arr = new int[*len];
    for (int i = 0; i < *len; i++)
    {
      arr[i] = getRandom(0, 100);
    }
    return arr;
  }

  static void test()
  {
    int testTimes = 2000000;
    int len = 0;
    cout << "test begin" << endl;
    for (int i = 0; i < testTimes; i++)
    {
      int *arr = gerenareRondomArray(&len);
      if (max1(arr, len) != max2(arr, len))
      {
        cout << "Oops!" << endl;
        break;
      }
    }
    cout << "test end" << endl;
  }
};

int main()
{
  AllTimesMinToMax::test();
  return 0;
}
