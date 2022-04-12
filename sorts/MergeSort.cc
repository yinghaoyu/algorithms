#include <iostream>

#define SIZE 1000

int arr[SIZE];

void merge(int arr[], int L, int M, int R)
{
  int len = R - L + 1;
  int *help= new int[len];
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
void process(int arr[], int L, int R)
{
  if (L == R)
  { // base case
    return;
  }
  int mid = L + ((R - L) >> 1);
  process(arr, L, mid);
  process(arr, mid + 1, R);
  merge(arr, L, mid, R);
}

// 递归方法实现
void mergeSort1(int arr[])
{
  if (arr == NULL || SIZE < 2)
  {
    return;
  }
  process(arr, 0, SIZE - 1);
}

// 非递归方法实现
void mergeSort2(int arr[]) {
  if (arr == NULL || SIZE < 2)
  {
    return;
  }
  int N = SIZE;
  // 步长
  int mergeSize = 1;
  while (mergeSize < N)
  { // log N
    // 当前左组的，第一个位置
    int L = 0;
    while (L < N)
    {
      if (mergeSize >= N - L)
      {
        break;
      }
      int M = L + mergeSize - 1;
      int R = M + std::min(mergeSize, N - M - 1);
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
