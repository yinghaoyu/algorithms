#include <iostream>
#include <random>
#include <vector>

using namespace std;

// 测试链接：https://leetcode.com/problems/sum-of-subarray-minimums/

// 题目描述：给定一个整数数组 arr，找到 min(b) 的总和，其中 b 的范围为 arr 的每个（连续）子数组
// 由于答案可能很大，因此 返回答案模 10^9 + 7
// 示例：
// 输入：arr = [3,1,2,4]
// 输出：17
// 解释：
// 子数组为 [3]，[1]，[2]，[4]，[3,1]，[1,2]，[2,4]，[3,1,2]，[1,2,4]，[3,1,2,4]。
// 最小值为 3，1，2，4，1，1，2，1，1，1，和为 17。

class SumOfSubarrayMinimums
{
 public:
  // 暴力解，找到所有子数组的最小值，相加
  // 时间复杂度：O(N^3)
  static int subArrayMinSum1(int *arr, int len)
  {
    int ans = 0;
    for (int i = 0; i < len; i++)
    {
      for (int j = i; j < len; j++)
      {
        int min = arr[i];
        for (int k = i + 1; k <= j; k++)
        {
          min = std::min(min, arr[k]);
        }
        ans += min;
      }
    }
    return ans;
  }

  // 没有用单调栈
  // 时间复杂度O(N^2)
  static int subArrayMinSum2(int *arr, int len)
  {
    int *left = leftNearLessEqual2(arr, len);
    int *right = rightNearLess2(arr, len);
    int ans = 0;
    for (int i = 0; i < len; i++)
    {
      int start = i - left[i];
      int end = right[i] - i;
      ans += start * end * arr[i];
    }
    return ans;
  }

  static int *leftNearLessEqual2(int *arr, int len)
  {
    int N = len;
    int *left = new int[N]();
    for (int i = 0; i < N; i++)
    {
      int ans = -1;
      // left[i] = x : arr[i]左边，离arr[i]最近，<=arr[i]，位置在x
      for (int j = i - 1; j >= 0; j--)
      {
        if (arr[j] <= arr[i])
        {
          ans = j;
          break;
        }
      }
      left[i] = ans;
    }
    return left;
  }

  static int *rightNearLess2(int *arr, int len)
  {
    int N = len;
    int *right = new int[N]();
    for (int i = 0; i < N; i++)
    {
      int ans = N;
      // right[i] = y : arr[i]右边，离arr[i]最近，< arr[i]，位置在y
      for (int j = i + 1; j < N; j++)
      {
        if (arr[i] > arr[j])
        {
          ans = j;
          break;
        }
      }
      right[i] = ans;
    }
    return right;
  }

  // 单调栈优化
  // 时间复杂度log(N)
  static int sumSubarrayMins(int *arr, int len)
  {
    int *stack = new int[len]();
    int *left = nearLessEqualLeft(arr, stack, len);
    int *right = nearLessRight(arr, stack, len);
    long ans = 0;
    for (int i = 0; i < len; i++)
    {
      long start = i - left[i];
      long end = right[i] - i;
      ans += start * end * (long) arr[i];
      ans %= 1000000007;
    }
    return (int) ans;
  }

  static int *nearLessEqualLeft(int *arr, int *stack, int len)
  {
    int N = len;
    int *left = new int[N]();
    int size = 0;
    // 从右往左
    for (int i = N - 1; i >= 0; i--)
    {
      // 单调递增栈
      while (size != 0 && arr[i] <= arr[stack[size - 1]])
      {
        left[stack[--size]] = i;
      }
      stack[size++] = i;
    }
    while (size != 0)
    {
      left[stack[--size]] = -1;
    }
    return left;
  }

  static int *nearLessRight(int *arr, int *stack, int len)
  {
    int N = len;
    int *right = new int[N]();
    int size = 0;
    // 从左往右
    for (int i = 0; i < N; i++)
    {
      // 单调递增栈
      while (size != 0 && arr[stack[size - 1]] > arr[i])
      {
        right[stack[--size]] = i;
      }
      stack[size++] = i;
    }
    while (size != 0)
    {
      right[stack[--size]] = N;
    }
    return right;
  }

  static int *randomArray(int len, int maxValue)
  {
    int *ans = new int[len]();
    for (int i = 0; i < len; i++)
    {
      ans[i] = getRandom(1, maxValue);
    }
    return ans;
  }

  static void printArray(int *arr, int len)
  {
    for (int i = 0; i < len; i++)
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
    int maxLen = 100;
    int maxValue = 50;
    int testTime = 100000;
    cout << "测试开始" << endl;
    for (int i = 0; i < testTime; i++)
    {
      int len = getRandom(0, maxLen);
      int *arr = randomArray(len, maxValue);
      int ans1 = subArrayMinSum1(arr, len);
      int ans2 = subArrayMinSum2(arr, len);
      int ans3 = sumSubarrayMins(arr, len);
      if (ans1 != ans2 || ans1 != ans3)
      {
        printArray(arr, len);
        cout << ans1 << endl;
        cout << ans2 << endl;
        cout << ans3 << endl;
        cout << "出错了！" << endl;
        break;
      }
    }
    cout << "测试结束" << endl;
  }
};

int main()
{
  SumOfSubarrayMinimums::test();
  return 0;
}
