#include <iostream>
#include <algorithm>
#include <stdbool.h>
#include <random>

using namespace std;

// 返回[min, max]
int getRandom(int min, int max)
{
  random_device seed;  // 硬件生成随机数种子
  ranlux48 engine(seed());  // 利用种子生成随机数引
  uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
  int res = distrib(engine);  // 随机数
  return res;
}

// for test
void printArray(int arr[], int len)
{
  if (arr == nullptr)
  {
    return;
  }
  for (int i = 0; i < len; i++)
  {
    cout << arr[i] << " ";
  }
  cout << endl;
}

void swap(int arr[], int i, int j)
{
  int tmp = arr[i];
  arr[i] = arr[j];
  arr[j] = tmp;
}
// arr[L..R]上，以arr[R]位置的数做划分值
// <= X > X
// <= X X
int partition(int arr[], int L, int R)
{
  if (L > R)
  {
    return -1;
  }
  if (L == R)
  {
    return L;
  }
  int lessEqual = L - 1;
  int index = L;
  while (index < R)
  {
    if (arr[index] <= arr[R])  // 取arr[R]为哨兵
    {
      swap(arr, index, ++lessEqual);
    }
    index++;
  }
  swap(arr, ++lessEqual, R);  // lessEqual左边为<= arr[R]的数
  return lessEqual;
}
// arr[L...R] 玩荷兰国旗问题的划分，以arr[R]做划分值
// <arr[R] ==arr[R] > arr[R]
int* netherlandsFlag(int arr[], int L, int R)
{
  if (L > R)
  {
    // L...R L>R
    return new int[2] { -1, -1 };
  }
  if (L == R)
  {
    return new int[2] { L, R };
  }
  int less = L - 1; // < 区 右边界
  int more = R; // > 区 左边界
  int index = L;
  while (index < more)
  {
    // 当前位置，不能和 >区的左边界撞上
    if (arr[index] == arr[R])
    {
      index++;
    }
    else if (arr[index] < arr[R])
    {
      //        swap(arr, less + 1, index);
      //        less++;
      //        index++;
      swap(arr, index++, ++less);
    }
    else
    {
      // >
      swap(arr, index, --more);
    }
  }
  swap(arr, more, R); // <[R]   =[R]   >[R]
  return new int[2] { less + 1, more };
}

void process1(int arr[], int L, int R)
{
  if (L >= R)
  {
    return;
  }
  // L..R partition arr[R] [ <=arr[R] arr[R] >arr[R] ]
  int M = partition(arr, L, R); // 经过这趟partition后arr[M]的位置已经确定
  process1(arr, L, M - 1);
  process1(arr, M + 1, R);
}

void quickSort1(int arr[], int len)
{
  if (arr == nullptr || len < 2)
  {
    return;
  }
  process1(arr, 0, len - 1);
}

// arr[L...R] 排有序，快排2.0方式
void process2(int arr[], int L, int R)
{
  if (L >= R)
  {
    return;
  }
  // [ equalArea[0]  ,  equalArea[1]] 这个是值相等的区间
  int* equalArea = netherlandsFlag(arr, L, R);
  process2(arr, L, equalArea[0] - 1);
  process2(arr, equalArea[1] + 1, R);
}

void quickSort2(int arr[], int len)
{
  if (arr == nullptr || len < 2)
  {
    return;
  }
  process2(arr, 0, len - 1);
}

void process3(int arr[], int L, int R)
{
  if (L >= R)
  {
    return;
  }
  int num = getRandom(0, R - L);  // 随机抽取哨兵
  swap(arr, L + num, R);
  int* equalArea = netherlandsFlag(arr, L, R);
  process3(arr, L, equalArea[0] - 1);
  process3(arr, equalArea[1] + 1, R);
}

void quickSort3(int arr[], int len)
{
  if (arr == nullptr || len < 2)
  {
    return;
  }
  process3(arr, 0, len - 1);
}

// for test
int* generateRandomArray(int maxSize, int maxValue, int* len)
{
  *len = getRandom(1, maxSize);
  int* arr = new int[*len];
  for (int i = 0; i < *len; i++)
  {
    arr[i] = getRandom(1, maxValue + 1) - getRandom(0, maxValue);
  }
  return arr;
}

// for test
int* copyArray(int arr[], int len)
{
  if (arr == nullptr)
  {
    return nullptr;
  }
  int* res = new int[len];
  for (int i = 0; i < len; i++)
  {
    res[i] = arr[i];
  }
  return res;
}

// for test
bool isEqual(int arr1[], int arr2[], int len1, int len2)
{
  if ((arr1 == nullptr && arr2 != nullptr) || (arr1 != nullptr && arr2 == nullptr))
  {
    return false;
  }
  if (arr1 == nullptr && arr2 == nullptr) {
    return true;
  }
  if (len1 != len2)
  {
    return false;
  }
  for (int i = 0; i < len1; i++)
  {
    if (arr1[i] != arr2[i])
    {
      return false;
    }
  }
  return true;
}

// for test
int main()
{
  int testTime = 500000;
  int maxSize = 20;
  int maxValue = 100;
  bool succeed = true;
  int len = 0;
  for (int i = 0; i < testTime; i++)
  {
    int* arr1 = generateRandomArray(maxSize, maxValue, &len);
    int* arr2 = copyArray(arr1, len);
    int* arr3 = copyArray(arr1, len);
    quickSort1(arr1, len);
    quickSort2(arr2, len);
    quickSort3(arr3, len);
    if (!isEqual(arr1, arr2, len, len) || !isEqual(arr2, arr3, len, len))
    {
      succeed = false;
      break;
    }
  }
  cout << (succeed ? "Nice" : "Oops!") << endl;
  return 0;
}
