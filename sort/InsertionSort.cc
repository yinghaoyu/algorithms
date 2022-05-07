#include <iostream>

// 插入排序
// 时间复杂度O(n^2)
// 空间复杂度O(1)
// 稳定性：可以达到稳定

#define N 1000

int arr[N];

// 交换arr的i和j位置上的值
void swap(int arr[], int i, int j)
{
  if (i == j)
  {
    return;
  }
  arr[i] = arr[i] ^ arr[j];
  arr[j] = arr[i] ^ arr[j];
  arr[i] = arr[i] ^ arr[j];
}

void insertionSort(int arr[])
{
  if (arr == nullptr || N < 2)
  {
    return;
  }
  // 不只1个数
  for (int i = 1; i < N; i++)
  { // 0 ~ i 做到有序
    for (int j = i - 1; j >= 0 && arr[j] > arr[j + 1]; j--)
    {
      swap(arr, j, j + 1);
    }
  }
}
