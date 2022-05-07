#include <iostream>
#include <memory.h>

using namespace std;
// 计数排序
// 时间复杂度：O(N)
// 空间复杂度：O(N)
// 稳定性：可以达到稳定

void printArr(int arr[], int len)
{
  for(int i = 0; i < len; i++)
  {
    cout << arr[i] << " ";
  }
  cout << endl;
}

// only for 0~200 value
void countSort(int arr[], int len)
{
  if (arr == nullptr || len < 2)
  {
    return;
  }
  int max = INT32_MIN;
  for (int i = 0; i < len; i++)
  {
    max = std::max(max, arr[i]);  // 得到数组的最大值
  }
  int *bucket = (int*)malloc(sizeof(int) * (max + 1));  // 用数组最大值生成bucket数量
  memset(bucket, 0, sizeof(int) * (max + 1));
  for (int i = 0; i < len; i++)
  {
    bucket[arr[i]]++;  // 统计value频率
  }
  int i = 0;
  for (int j = 0; j < max + 1; j++)
  {
    while (bucket[j]-- > 0)
    {
      arr[i++] = j;  // 输出bucket[j]个j
    }
  }
}

int main ()
{
  int arr[] = {1, 3, 2, 1, 0, 2, 3, 4, 8, 7, 7, 6, 6, 5};
  int len = sizeof(arr) / sizeof(int);
  cout << "len = " << len << endl;
  countSort(arr, len);
  printArr(arr, len);
  return 0;
}
