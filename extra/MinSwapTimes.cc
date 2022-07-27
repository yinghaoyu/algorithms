#include <iostream>
#include <random>
#include <vector>

using namespace std;

// 一个无序数组长度为n，所有数字都不一样，并且值都在[0...n-1]范围上
// 返回让这个无序数组变成有序数组的最小交换次数
class MinSwapTimes
{
 public:
  // 纯暴力，arr长度大一点都会超时
  // 但是绝对正确
  static int minSwap1(vector<int> &arr) { return process1(arr, 0); }

  // 让arr变有序，最少的交换次数是多少！返回
  // times, 之前已经做了多少次交换

  static int process1(vector<int> &arr, int times)
  {
    bool sorted = true;
    for (int i = 1; i < arr.size(); i++)
    {
      if (arr[i - 1] > arr[i])
      {
        sorted = false;
        break;
      }
    }
    if (sorted)
    {
      return times;
    }
    // 数组现在是无序的状态！
    if (times >= arr.size() - 1)
    {
      return INT32_MAX;
    }
    int ans = INT32_MAX;
    for (int i = 0; i < arr.size(); i++)
    {
      for (int j = i + 1; j < arr.size(); j++)
      {
        swap(arr, i, j);
        ans = std::min(ans, process1(arr, times + 1));
        swap(arr, i, j);
      }
    }
    return ans;
  }

  static void swap(vector<int> &arr, int i, int j)
  {
    int tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
  }

  // 已知arr中，只有0~n-1这些值，并且都出现1次
  static int minSwap2(vector<int> &arr)
  {
    int ans = 0;
    for (int i = 0; i < arr.size(); i++)
    {
      // 下标循环怼
      while (i != arr[i])
      {
        swap(arr, i, arr[i]);
        ans++;
      }
    }
    return ans;
  }

  // 为了测试
  static vector<int> randomArray(int len)
  {
    vector<int> arr(len);
    for (int i = 0; i < len; i++)
    {
      arr[i] = i;
    }
    for (int i = 0; i < len; i++)
    {
      swap(arr, i, getRandom(0, len - 1));
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
  static void test()
  {
    int n = 6;
    int testTime = 2000;
    cout << "测试开始" << endl;
    for (int i = 0; i < testTime; i++)
    {
      int len = getRandom(1, n);
      vector<int> arr = randomArray(len);
      int ans1 = minSwap1(arr);
      int ans2 = minSwap2(arr);
      if (ans1 != ans2)
      {
        cout << "出错了!" << endl;
      }
    }
    cout << "测试结束" << endl;
  }
};

int main()
{
  MinSwapTimes::test();
  return 0;
}
