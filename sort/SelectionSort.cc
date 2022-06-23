#include <iostream>

// 选择排序，每次选取最小值进行交换。
// 时间复杂度O(n^2)
// 空间复杂度O(1)
// 稳定性：不稳定，比如5 8 5 2 9第一次选择最小的数2和第一个5交换
// 交换次数最差n-1

#define N 1000

int arr[N];

void swap(int arr[], int i, int j)
{
  int tmp = arr[i];
  arr[i] = arr[j];
  arr[j] = tmp;
}

void selectionSort()
{
  // 0 ~ N-1  找到最小值，在哪，放到0位置上
  // 1 ~ n-1  找到最小值，在哪，放到1 位置上
  // 2 ~ n-1  找到最小值，在哪，放到2 位置上
  for (int i = 0; i < N - 1; i++)
  {
    int minIndex = i;
    for (int j = i + 1; j < N; j++)
    {
      // i ~ N-1 上找最小值的下标
      minIndex = arr[j] < arr[minIndex] ? j : minIndex;
    }
    swap(arr, i, minIndex);
  }
}
