#include <stdlib.h>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

// 设有一个数组 [a1, a2, a3,... an]，对于数组中任意两个元素ai，aj，若i < j，ai > aj，则说明ai和aj是一对逆序对。求一个给定数组的逆序对个数。

// 思路：合并的时候，从右往左合并，(此时右组位置 - mid位置) 的累加和 即是逆序对个数。

// 这又咋和归并排序联系上的呢？在左组和右组merge的时候，会比较数的大小，但是我要找到的是右边更小的，所以可以采用从右往左合并的方式；同时在处理相等的时候，需要先拷贝右组的，这样才能准确找出右组小的个数。

class ReversePair
{
 public:
  static int getRandom(int min, int max)
  {
    random_device seed;
    ranlux48 engine(seed());
    uniform_real_distribution<> distrib(min, max);
    int res = distrib(engine);
    return res;
  }

  static int reverPairNumber(vector<int> &arr)
  {
    if (arr.size() < 2)
    {
      return 0;
    }
    return process(arr, 0, arr.size() - 1);
  }

  // arr[L..R]既要排好序，也要求逆序对数量返回
  // 所有merge时，产生的逆序对数量，累加，返回
  // 左 排序 merge并产生逆序对数量
  // 右 排序 merge并产生逆序对数量
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
    int len = r - L + 1;
    int *help = new int[len];
    int i = len - 1;  // 由于从右往左合并，所以下标为数组最大值
    int p1 = m;
    int p2 = r;
    int res = 0;
    while (p1 >= L && p2 > m)
    {
      // 找到右组第一个比左组小的数，则当前满足要求的逆序对个数为 (p2 - (m + 1) + 1) 即是 (p2 - m)
      // 因为左组，右组在组内都是有序的，arr[m+1 ... p2]之间的数都小于arr[p1]
      res += arr[p1] > arr[p2] ? (p2 - m) : 0;
      // 从右往左拷贝，相等的先拷贝右组的，否则会少算
      help[i--] = arr[p1] > arr[p2] ? arr[p1--] : arr[p2--];
    }
    while (p1 >= L)
    {
      help[i--] = arr[p1--];
    }
    while (p2 > m)
    {
      help[i--] = arr[p2--];
    }
    for (i = 0; i < len; i++)
    {
      arr[L + i] = help[i];
    }
    return res;
  }

  // for test
  static int comparator(vector<int> &arr)
  {
    int ans = 0;
    for (int i = 0; i < arr.size(); i++)
    {
      for (int j = i + 1; j < arr.size(); j++)  // 暴力解O(N^2)
      {
        if (arr[i] > arr[j])
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
      arr[i] = getRandom(0, maxValue + 1) - getRandom(0, maxValue);
    }
    return arr;
  }

  // for test
  static bool isEqual(vector<int> &arr1, vector<int> &arr2)
  {
    if (arr1.size() != arr1.size())
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
  int testTime = 5000;
  int maxSize = 100;
  int maxValue = 100;
  int len = 0;
  cout << "测试开始" << endl;
  for (int i = 0; i < testTime; i++)
  {
    vector<int> arr1 = ReversePair::generateRandomArray(maxSize, maxValue, &len);
    vector<int> arr2 = arr1;
    if (ReversePair::reverPairNumber(arr1) != ReversePair::comparator(arr2))
    {
      cout << "Oops!" << endl;
      ReversePair::printArray(arr1);
      ReversePair::printArray(arr2);
      break;
    }
  }
  cout << "测试结束" << endl;
  return 0;
}
