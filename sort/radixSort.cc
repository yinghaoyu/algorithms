#include <math.h>
#include <memory.h>
#include <cstdint>
#include <iostream>

using namespace std;
// 基数排序
// 时间复杂度：O(n)
// 空间复杂度：O(n)
// 稳定性：可以达到稳定

//（1）遍历整个数组，得到最大值的十进制位数，同时将不满足最大位数的数高位补0
//（2）准备十个桶，从0 - 9编号，每个桶大小为原数组大小
//（3）遍历数组，以个位数为标准，个位数的值是 i，则将其放到第 i 号桶里；遍历完后，从0号桶开始，挨个将里面的数倒出来（先进先出）。此时的数是根据个位数排序
//（4）遍历数组，以十位数为标准，十位数的值是 i，则将其放到第 i 号桶里；遍历完后，从0号桶开始，挨个将里面的数倒出来（先进先出）。此时的数是根据十位数排序
//
//  先进先出，保证了稳定性。
//
//……直到遍历到最高位，最后倒出来的数即已排好序。
//
//实际代码编写的时候，并没有准备十个桶，而是准备的一个长度为10的数组，这样可以大大节省空间。

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
  int *help = (int *) malloc((R - L + 1) * sizeof(int));
  for (int d = 1; d <= digit; d++)
  {
    // 有多少位就进出几次
    // 10个空间
    // count[0] 当前位(d位)是0的数字有多少个
    // count[1] 当前位(d位)是(0和1)的数字有多少个
    // count[2] 当前位(d位)是(0、1和2)的数字有多少个
    // count[i] 当前位(d位)是(0~i)的数字有多少个
    int *count = (int *) malloc(radix * sizeof(int));  // count[0..9]
    memset(count, 0, radix * sizeof(int));
    for (i = L; i <= R; i++)
    {
      // 103  1   3
      // 209  1   9
      j = getDigit(arr[i], d);
      count[j]++;  // 先统计d位是j的数有多少个
    }
    for (i = 1; i < radix; i++)
    {
      count[i] = count[i] + count[i - 1];  // 前缀和
    }
    for (i = R; i >= L; i--)  // 从右到左出桶
    {
      j = getDigit(arr[i], d);
      help[count[j] - 1] = arr[i];
      count[j]--;
    }
    for (i = L, j = 0; i <= R; i++, j++)
    {
      arr[i] = help[j];
    }
    free(count);
  }
  free(help);
}

// only for no-negative value
void radixSort(int arr[], int len)
{
  if (arr == nullptr || len < 2)
  {
    return;
  }
  radixSort(arr, 0, len - 1, maxbits(arr, len));
}

void printArr(int arr[], int len)
{
  for (int i = 0; i < len; i++)
  {
    cout << arr[i] << " ";
  }
  cout << endl;
}

int main()
{
  int arr[] = {1, 2, 0, 1, 3, 2, 1, 4, 5, 6, 9, 7, 8, 5, 6, 1};
  int len = sizeof(arr) / sizeof(int);
  radixSort(arr, len);
  printArr(arr, len);
  return 0;
}
