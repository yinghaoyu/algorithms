#include <stdbool.h>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

// 求给定数组中，当前数 大于 右侧数的两倍 的个数
// 例子：
// 数组：[6, 7, 3, 2, 1]
// 当前数大于右侧数的两倍的数有(6,2)，(6,1)，(7,3)，(7,2)，(7,1)，(3,1)
// 所有总共有6个

class BiggerThanRightTwice
{
 public:
  static int getRandom(int min, int max)
  {
    random_device seed;                            // 硬件生成随机数种子
    ranlux48 engine(seed());                       // 利用种子生成随机数引
    uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
    int res = distrib(engine);                     // 随机数
    return res;
  }

  static int biggerTwice(vector<int> &arr)
  {
    if (arr.size() < 2)
    {
      return 0;
    }
    return process(arr, 0, arr.size() - 1);
  }

  static int process(vector<int> &arr, int l, int r)
  {
    if (l == r)
    {
      return 0;
    }
    // l < r
    int mid = l + ((r - l) >> 1);
    return process(arr, l, mid) + process(arr, mid + 1, r) + merge(arr, l, mid, r);
  }

  static int merge(vector<int> &arr, int L, int m, int r)
  {
    // [L....M]   [M+1....R]
    int ans = 0;
    // 目前囊括进来的数，是从[M+1, windowR)
    int windowR = m + 1;
    for (int i = L; i <= m; i++)
    {
      while (windowR <= r && arr[i] > (arr[windowR] << 1))
      {
        windowR++;
      }
      ans += windowR - m - 1;  // arr[windowR]是第一个不满足条件的
    }

    int len = r - L + 1;
    vector<int> help(len);
    int i = 0;
    int p1 = L;
    int p2 = m + 1;
    while (p1 <= m && p2 <= r)
    {
      help[i++] = arr[p1] <= arr[p2] ? arr[p1++] : arr[p2++];  // 这里<=不影响结果，为了保证稳定，先拷贝左数组
    }
    while (p1 <= m)
    {
      help[i++] = arr[p1++];
    }
    while (p2 <= r)
    {
      help[i++] = arr[p2++];
    }
    for (i = 0; i < len; i++)
    {
      arr[L + i] = help[i];
    }
    return ans;
  }

  // for test
  // 暴力解时间复杂度O(N^2)
  static int comparator(vector<int> &arr)
  {
    int ans = 0;
    for (int i = 0; i < arr.size(); i++)
    {
      for (int j = i + 1; j < arr.size(); j++)
      {
        if (arr[i] > (arr[j] << 1))
        {
          ans++;
        }
      }
    }
    return ans;
  }

  // for test
  static vector<int> generateRandomArray(int maxSize, int maxValue, int *len)
  {
    *len = getRandom(0, maxSize);
    vector<int> arr(*len);
    for (int i = 0; i < *len; i++)
    {
      arr[i] = getRandom(0, maxValue) - getRandom(0, maxValue);
    }
    return arr;
  }

  // for test
  static void printArray(vector<int> &arr)
  {
    if (arr.size() == 0)
    {
      return;
    }
    for (int i = 0; i < arr.size(); i++)
    {
      cout << arr[i] << " ";
    }
    cout << endl;
  }
};

int main()
{
  int testTime = 100;
  int maxSize = 100;
  int maxValue = 100;
  bool succeed = true;
  int len = 0;
  for (int i = 0; i < testTime; i++)
  {
    vector<int> arr1 = BiggerThanRightTwice::generateRandomArray(maxSize, maxValue, &len);
    vector<int> arr2 = arr1;
    if (BiggerThanRightTwice::biggerTwice(arr1) != BiggerThanRightTwice::comparator(arr2))
    {
      succeed = false;
      BiggerThanRightTwice::printArray(arr1);
      BiggerThanRightTwice::printArray(arr2);
      break;
    }
  }
  cout << (succeed ? "Nice!" : "Fucking fucked!") << endl;
}
