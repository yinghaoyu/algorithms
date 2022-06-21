#include <iostream>
#include <vector>

using namespace std;

void merge(vector<int> &arr, int L, int M, int R)
{
  int len = R - L + 1;
  vector<int> help(len);
  int i = 0;
  int p1 = L;
  int p2 = M + 1;
  while (p1 <= M && p2 <= R)
  {
    help[i++] = arr[p1] <= arr[p2] ? arr[p1++] : arr[p2++];
  }
  // 要么p1越界了，要么p2越界了
  while (p1 <= M)
  {
    help[i++] = arr[p1++];
  }
  while (p2 <= R)
  {
    help[i++] = arr[p2++];
  }
  for (i = 0; i < len; i++)
  {
    arr[L + i] = help[i];
  }
}

// 请把arr[L..R]排有序
// l...r N
// T(N) = 2 * T(N / 2) + O(N)
// O(N * logN)
void process(vector<int> &arr, int L, int R)
{
  if (L == R)
  {  // base case
    return;
  }
  int mid = L + ((R - L) >> 1);
  process(arr, L, mid);
  process(arr, mid + 1, R);
  merge(arr, L, mid, R);
}

// 递归方法实现
void mergeSort1(vector<int> &arr)
{
  if (arr.size() < 2)
  {
    return;
  }
  process(arr, 0, arr.size() - 1);
}

// 非递归方法实现
void mergeSort2(vector<int> &arr)
{
  if (arr.size() < 2)
  {
    return;
  }
  int N = arr.size();
  // 每个分组的长度
  int mergeSize = 1;
  while (mergeSize < N)  // O(logN)
  {
    // 当前左组的，第一个位置
    int L = 0;
    while (L < N)  // 从左到右，分组两两归并，O(N)
    {
      // 只剩左分组，且该分组长度 <= mergeSize
      // 一个分组不需要归并操作
      if (mergeSize >= N - L)
      {
        break;
      }
      int M = L + mergeSize - 1;
      int R = M + std::min(mergeSize, N - M - 1);  // 右组的右边界取较小值
      merge(arr, L, M, R);
      L = R + 1;
    }
    // 防止溢出
    if (mergeSize > N / 2)
    {
      break;
    }
    mergeSize <<= 1;
  }
}
