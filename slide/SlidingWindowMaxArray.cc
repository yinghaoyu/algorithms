#include <iostream>
#include <deque>
#include <random>

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
    static int* right(int* arr, int len, int w)
    {
      if (arr == nullptr || w < 1 || len < w)
      {
        return nullptr;
      }
      int N = len;
      // 最终选出N - w + 1个数
      int* res = new int[N - w + 1](); // ()表示以默认形式构造数组，初值为0
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

    static int* getMaxWindow(int* arr, int len, int w)
    {
      if (arr == nullptr || w < 1 || len < w)
      {
        return nullptr;
      }
      // qmax 窗口最大值的更新结构
      // 放下标
      deque<int> qmax;
      int* res = new int[len - w + 1]();
      int index = 0;
      for (int R = 0; R < len; R++)
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
    static int* generateRandomArray(int maxSize, int maxValue, int* len)
    {
      *len = getRandom(0, maxSize);
      int* arr = new int[*len]();
      for (int i = 0; i < *len; i++)
      {
        arr[i] = getRandom(0, maxValue);
      }
      return arr;
    }

    // for test
    static bool isEqual(int* arr1, int* arr2, int len1, int len2)
    {
      if ((arr1 == nullptr && arr2 != nullptr)
          || (arr1 != nullptr && arr2 == nullptr))
      {
        return false;
      }
      if (arr1 == nullptr && arr2 == nullptr)
      {
        return true;
      }
      if (len1 != len2)
      {
        return false;
      }
      for (int i = 0; i < len1; i++)
      {
        if (arr1[i] != arr2[i])
        {
          return false;
        }
      }
      return true;
    }

    static void printArray(int* arr, int len)
    {
      for(int i = 0; i < len; i++)
      {
        cout << arr[i] << " ";
      }
      cout << endl;
    }

    static int getRandom(int min, int max)
    {
      random_device seed;  // 硬件生成随机数种子
      ranlux48 engine(seed());  // 利用种子生成随机数引
      uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
      int res = distrib(engine);  // 随机数
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
        int* arr = generateRandomArray(maxSize, maxValue, &len);
        int w = getRandom(0, len);
        int* ans1 = getMaxWindow(arr, len, w);
        int* ans2 = right(arr, len, w);
        if (!isEqual(ans1, ans2, len - w + 1, len - w + 1))
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
