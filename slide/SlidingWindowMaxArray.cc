#include <deque>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

//测试链接：https://leetcode-cn.com/problems/sliding-window-maximum

// 滑动窗口最大值
// 给你一个整数数组 nums，有一个大小为 k 的滑动窗口从数组的最左侧移动到数组的最右侧。你只可以看到在滑动窗口内的 k 个数字。滑动窗口每次只向右移动一位。
// 返回 滑动窗口中的最大值。

// 滑动窗口的位置                最大值
// ---------------               -----
// [1  3  -1] -3  5  3  6  7      3
// 1 [3  -1  -3] 5  3  6  7       3
// 1  3 [-1  -3  5] 3  6  7       5
// 1  3  -1 [-3  5  3] 6  7       5
// 1  3  -1  -3 [5  3  6] 7       6
// 1  3  -1  -3  5 [3  6  7]      7

class SlidingWindowMaxArray
{
 public:
  // 暴力的对数器方法
  static vector<int> right(vector<int> &arr, int w)
  {
    if (w < 1 || arr.size() < w)
    {
      return vector<int>();
    }
    int N = arr.size();
    // 最终选出N - w + 1个数
    vector<int> res(N - w + 1);
    int index = 0;
    int L = 0;
    int R = w - 1;
    while (R < N)
    {
      int max = arr[L];
      for (int i = L + 1; i <= R; i++)
      {
        max = std::max(max, arr[i]);
      }
      res[index++] = max;
      L++;
      R++;
    }
    return res;
  }

  static vector<int> getMaxWindow(vector<int> &arr, int w)
  {
    if (w < 1 || arr.size() < w)
    {
      return vector<int>();
    }
    int N = arr.size();
    // qmax 窗口最大值的更新结构
    // 放下标
    deque<int> qmax;
    vector<int> res(N - w + 1);
    int index = 0;
    for (int R = 0; R < N; R++)
    {
      // 窗口保证单调递减
      while (!qmax.empty() && arr[qmax.back()] <= arr[R])
      {
        qmax.pop_back();
      }
      qmax.push_back(R);
      if (qmax.front() == R - w)  // 窗口大于w
      {
        qmax.pop_front();
      }
      if (R >= w - 1)  // 从 w - 1 之后开始算
      {
        res[index++] = arr[qmax.front()];
      }
    }
    return res;
  }

  // for test
  static vector<int> generateRandomArray(int maxSize, int maxValue, int *len)
  {
    *len = getRandom(0, maxSize);
    vector<int> arr(*len);
    for (int i = 0; i < *len; i++)
    {
      arr[i] = getRandom(0, maxValue);
    }
    return arr;
  }

  // for test
  static bool isEqual(vector<int> &arr1, vector<int> &arr2)
  {
    if (arr1.size() != arr2.size())
    {
      return false;
    }
    for (int i = 0; i < arr1.size(); i++)
    {
      if (arr1[i] != arr2[i])
      {
        return false;
      }
    }
    return true;
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
    int testTime = 10000;
    int maxSize = 100;
    int maxValue = 100;
    int len = 0;
    cout << "test begin" << endl;
    for (int i = 0; i < testTime; i++)
    {
      vector<int> arr = generateRandomArray(maxSize, maxValue, &len);
      int w = getRandom(0, len);
      vector<int> ans1 = getMaxWindow(arr, w);
      vector<int> ans2 = right(arr, w);
      if (!isEqual(ans1, ans2))
      {
        cout << "Oops!" << endl;
      }
    }
    cout << "test finish" << endl;
  }
};

int main()
{
  SlidingWindowMaxArray::test();
  return 0;
}
