#include <iostream>

// 计数排序
// 时间复杂度：O(N)
// 空间复杂度：O(N)
// 稳定性：可以达到稳定

// only for 0~200 value
void countSort(int arr[], int len)
{
  if (arr == NULL || len < 2)
  {
    return;
  }
  int max = INT32_MIN;
  for (int i = 0; i < len; i++)
  {
    max = std::max(max, arr[i]);
  }
  int *bucket = new int[max + 1];
  for (int i = 0; i < len; i++)
  {
    bucket[arr[i]]++;
  }
  int i = 0;
  for (int j = 0; j < len; j++)
  {
    while (bucket[j]-- > 0)
    {
      arr[i++] = j;
    }
  }
}
