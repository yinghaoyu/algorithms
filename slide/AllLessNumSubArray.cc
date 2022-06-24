#include <deque>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

// 给定数组arr和整数num，共返回多少子数组满足：
// max(arr[i..j]) - min(arr[i...j]) <= num

class AllLessNumSubArray
{
 public:
  // 暴力的对数器方法
  static int right(vector<int> &arr, int sum)
  {
    if (arr.size() == 0 || sum < 0)
    {
      return 0;
    }
    int N = arr.size();
    int count = 0;
    for (int L = 0; L < N; L++)
    {
      for (int R = L; R < N; R++)
      {
        int max = arr[L];
        int min = arr[L];
        for (int i = L + 1; i <= R; i++)
        {
          max = std::max(max, arr[i]);
          min = std::min(min, arr[i]);
        }
        if (max - min <= sum)
        {
          count++;
        }
      }
    }
    return count;
  }

  static int num(vector<int> &arr, int sum)
  {
    if (arr.size() == 0 || sum < 0)
    {
      return 0;
    }
    int N = arr.size();
    int count = 0;
    deque<int> maxWindow;
    deque<int> minWindow;
    int R = 0;
    for (int L = 0; L < N; L++)
    {
      while (R < N)
      {
        // 窗口[L...R]，R向右扩直到违规
        // 单调递减
        while (!maxWindow.empty() && arr[maxWindow.back()] <= arr[R])
        {
          maxWindow.pop_back();
        }
        maxWindow.push_back(R);
        // 单调递增
        while (!minWindow.empty() && arr[minWindow.back()] >= arr[R])
        {
          minWindow.pop_back();
        }
        minWindow.push_back(R);
        // arr[L...R]区间的最大值和最小值之差大于sum
        // 即第一个违规的数进了窗口
        if (arr[maxWindow.front()] - arr[minWindow.front()] > sum)
        {
          break;
        }
        else
        {
          R++;
        }
      }
      // R是第一个违规的位置，只考虑以L开头的子数组
      // 此时满足要求的子数组有arr[L]、arr[L...L+1]、arr[L...L+2]、...、arr[L...R-1]
      // 总共有R-L个
      count += R - L;
      // 检查最大值和最小值有没有过期，因为马上就要L++
      if (maxWindow.front() == L)
      {
        maxWindow.pop_front();
      }
      if (minWindow.front() == L)
      {
        minWindow.pop_front();
      }
    }
    return count;
  }

  // for test
  static vector<int> generateRandomArray(int maxLen, int maxValue, int *len)
  {
    *len = getRandom(0, maxLen);
    vector<int> arr(*len);
    for (int i = 0; i < *len; i++)
    {
      arr[i] = getRandom(0, maxValue);
    }
    return arr;
  }

  // for test
  static void printArray(vector<int> &arr)
  {
    if (arr.size() != 0)
    {
      for (int i = 0; i < arr.size(); i++)
      {
        cout << arr[i] << " ";
      }
      cout << endl;
    }
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
    int maxValue = 200;
    int testTime = 100000;
    int len = 0;
    cout << "测试开始" << endl;
    for (int i = 0; i < testTime; i++)
    {
      vector<int> arr = generateRandomArray(maxLen, maxValue, &len);
      int sum = getRandom(0, maxValue);
      int ans1 = right(arr, sum);
      int ans2 = num(arr, sum);
      if (ans1 != ans2)
      {
        cout << "Oops!" << endl;
        printArray(arr);
        cout << sum << endl;
        cout << ans1 << endl;
        cout << ans2 << endl;
        break;
      }
    }
    cout << "测试结束" << endl;
  }
};

int main()
{
  AllLessNumSubArray::test();
  return 0;
}
