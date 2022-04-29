#include <iostream>
#include <stdbool.h>
#include <random>

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
      random_device seed;  // 硬件生成随机数种子
      ranlux48 engine(seed());  // 利用种子生成随机数引
      uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
      int res = distrib(engine);  // 随机数
      return res;
    }

    static int biggerTwice(int arr[], int len)
    {
      if (arr == NULL || len < 2)
      {
        return 0;
      }
      return process(arr, 0, len - 1);
    }

    static int process(int arr[], int l, int r)
    {
      if (l == r)
      {
        return 0;
      }
      // l < r
      int mid = l + ((r - l) >> 1);
      return process(arr, l, mid)
        + process(arr, mid + 1, r)
        + merge(arr, l, mid, r);
    }

    static int merge(int arr[], int L, int m, int r)
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
        ans += windowR - m - 1;
      }

      int len = r - L + 1;
      int* help = new int[len];
      int i = 0;
      int p1 = L;
      int p2 = m + 1;
      while (p1 <= m && p2 <= r)
      {
        help[i++] = arr[p1] <= arr[p2] ? arr[p1++] : arr[p2++]; // 这里<=不影响结果，为了保证稳定，先拷贝左数组
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
    static int comparator(int arr[], int len)
    {
      int ans = 0;
      for (int i = 0; i < len; i++)
      {
        for (int j = i + 1; j < len; j++)
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
    static int* generateRandomArray(int maxSize, int maxValue, int* len)
    {
      *len = getRandom(0, maxSize);
      int* arr = new int[*len];
      for (int i = 0; i < *len; i++)
      {
        arr[i] = getRandom(0, maxValue) - getRandom(0, maxValue);
      }
      return arr;
    }

    // for test
    static int* copyArray(int* arr, int len)
    {
      if (arr == NULL)
      {
        return NULL;
      }
      int* res = new int[len];
      for (int i = 0; i < len; i++)
      {
        res[i] = arr[i];
      }
      return res;
    }

    // for test
    static bool isEqual(int arr1[], int arr2[], int len1, int len2)
    {
      if ((arr1 == NULL && arr2 != NULL) || (arr1 != NULL && arr2 == NULL))
      {
        return false;
      }
      if (arr1 == NULL && arr2 == NULL)
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

    // for test
    static void printArray(int arr[], int len)
    {
      if (arr == NULL)
      {
        return;
      }
      for (int i = 0; i < len; i++)
      {
        cout<< arr[i] << " ";
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
    int* arr1 = BiggerThanRightTwice::generateRandomArray(maxSize, maxValue, &len);
    int* arr2 = BiggerThanRightTwice::copyArray(arr1, len);
    if (BiggerThanRightTwice::biggerTwice(arr1, len) != BiggerThanRightTwice::comparator(arr2, len))
    {
      succeed = false;
      BiggerThanRightTwice::printArray(arr1, len);
      BiggerThanRightTwice::printArray(arr2, len);
      break;
    }
  }
  cout << (succeed ? "Nice!" : "Fucking fucked!") << endl;
}
