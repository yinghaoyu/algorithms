#include <cstdint>
#include <iostream>
#include <math.h>

// 基数排序
// 时间复杂度：O(n)
// 空间复杂度：O(n)
// 稳定性：可以达到稳定

// 获取最大值需要多少位10进制数表示，例如：99，返回2
int maxbits(int arr[], int len)
{
  int max = INT32_MIN;
  for (int i = 0; i < len; i++)
  {
    max = std::max(max, arr[i]);
  }
  int res = 0;
  while (max != 0)
  {
    res++;
    max /= 10;
  }
  return res;
}

// 求x的第d位的数
int getDigit(int x, int d)
{
  return ((x / ((int) pow(10, d - 1))) % 10);
}

// arr[L..R]排序  ,  最大值的十进制位数digit
void radixSort(int arr[], int L, int R, int digit)
{
  const int radix = 10;
  int i = 0, j = 0;
  // 有多少个数准备多少个辅助空间
  int *help = new int[R - L + 1];
  for (int d = 1; d <= digit; d++)
  {
    // 有多少位就进出几次
    // 10个空间
    // count[0] 当前位(d位)是0的数字有多少个
    // count[1] 当前位(d位)是(0和1)的数字有多少个
    // count[2] 当前位(d位)是(0、1和2)的数字有多少个
    // count[i] 当前位(d位)是(0~i)的数字有多少个
    int *count = new int[radix]; // count[0..9]
    for (i = L; i <= R; i++)
    {
      // 103  1   3
      // 209  1   9
      j = getDigit(arr[i], d);
      count[j]++;  // 先统计d位是j的数有多少个
    }
    for (i = 1; i < radix; i++)
    {
      count[i] = count[i] + count[i - 1];
    }
    for (i = R; i >= L; i--)
    {
      j = getDigit(arr[i], d);
      help[count[j] - 1] = arr[i];
      count[j]--;
    }
    for (i = L, j = 0; i <= R; i++, j++)
    {
      arr[i] = help[j];
    }
  }
}

// only for no-negative value
void radixSort(int arr[], int len)
{
  if (arr == NULL || len < 2)
  {
    return;
  }
  radixSort(arr, 0, len - 1, maxbits(arr, len));
}
