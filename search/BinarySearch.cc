#include <stdbool.h>
#include <iostream>
#include <vector>

using namespace std;

// 二分查找
// 时间复杂度O(log(n))
class BinarySearch
{
 public:
  static bool exist(vector<int> &sortedArr, int num)
  {
    if (sortedArr.size() == 0)
    {
      return false;
    }
    int L = 0;
    int R = sortedArr.size() - 1;
    int mid = 0;
    // L..R
    while (L < R)
    {
      // L..R 至少两个数的时候
      mid = L + ((R - L) >> 1);
      if (sortedArr[mid] == num)
      {
        return true;
      }
      else if (sortedArr[mid] > num)
      {
        R = mid - 1;
      }
      else
      {
        L = mid + 1;
      }
    }
    return sortedArr[L] == num;
  }

  // 找到>=value的最左位置
  static int nearestLeftIndex(vector<int> &sortedArr, int value)
  {
    int L = 0;
    int R = sortedArr.size() - 1;
    int index = -1;  // 记录最左的对号
    while (L <= R)
    {
      // 至少一个数的时候
      int mid = L + ((R - L) >> 1);
      if (sortedArr[mid] >= value)  // 当有连续一串值相等时，右边界一直左移
      {
        index = mid;
        R = mid - 1;
      }
      else
      {
        L = mid + 1;
      }
    }
    return index;
  }

  // 在arr上，找满足<=value的最右位置
  static int nearestRightIndex(vector<int> &sortedArr, int value)
  {
    int L = 0;
    int R = sortedArr.size() - 1;
    int index = -1;  // 记录最右的对号
    while (L <= R)
    {
      int mid = L + ((R - L) >> 1);
      if (sortedArr[mid] <= value)  // 当有连续一串值相等时，左边界一直右移
      {
        index = mid;
        L = mid + 1;
      }
      else
      {
        R = mid - 1;
      }
    }
    return index;
  }

  static void test()
  {
    vector<int> arr = {1, 2, 2, 3, 3, 3, 5, 6, 6, 6, 7, 8, 9};
    cout << "5 is exist ?" << (exist(arr, 5) ? "true" : "false") << endl;  // true
    cout << "2 left index ?" << nearestLeftIndex(arr, 2) << endl;          // 1
    cout << "6 right index ?" << nearestRightIndex(arr, 6) << endl;        // 9
  }
};

int main()
{
  BinarySearch::test();
  return 0;
}
