#include <iostream>
#include <stdbool.h>

// 二分查找
// 时间复杂度O(log(n))

#define N 1000

int sortedArr[N];

bool exist(int sortedArr[], int num)
{
  if (sortedArr == NULL || N == 0)
  {
    return false;
  }
  int L = 0;
  int R = N - 1;
  int mid = 0;
  // L..R
  while (L < R)
  { // L..R 至少两个数的时候
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
int nearestLeftIndex(int sortedArr[], int value)
{
  int L = 0;
  int R = N - 1;
  int index = -1; // 记录最左的对号
  while (L <= R)
  { // 至少一个数的时候
    int mid = L + ((R - L) >> 1);
    if (sortedArr[mid] >= value)
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
int nearestRightIndex(int sortedArr[], int value)
{
  int L = 0;
  int R = N - 1;
  int index = -1; // 记录最右的对号
  while (L <= R)
  {
    int mid = L + ((R - L) >> 1);
    if (sortedArr[mid] <= value)
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
