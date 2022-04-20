#include <functional> // less greater
#include <random>
#include <queue>
#include <iostream>

using namespace std;

// 从一个无序数组中，找到第k小的数

/**************** Way 1 ************************/
// 利用大根堆，时间复杂度O(N*logK)
int minKth1(int arr[], int len, int k)
{
  priority_queue<int, vector<int>, less<int> > maxHeap;  // 大根堆
  for (int i = 0; i < k; i++)  // 只让堆里有k个元素
  {
    maxHeap.push(arr[i]);  // 建堆时间复杂度O(k)
  }
  for (int i = k; i < len; i++) // 时间复杂度O((N-k)*logK)
  {
    if (arr[i] < maxHeap.top()) // 比堆顶小的元素才入堆，保证堆顶经过一次遍历后还是
    {
      maxHeap.pop();  // priority_queue的pop不会排序
      maxHeap.push(arr[i]);  // 时间复杂度logK
    }
  }
  return maxHeap.top();  // 经过一次遍历建堆，堆顶一定是第k小的数
}

/**************** Way 2 ************************/
int getRandom(int min, int max)
{
  random_device seed;  // 硬件生成随机数种子
  ranlux48 engine(seed());  // 利用种子生成随机数引
  uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
  int res = distrib(engine);  // 随机数
  return res;
}

int* copyArray(int arr[], int len)
{
  int* ans = new int[len];
  for (int i = 0; i != len; i++)
  {
    ans[i] = arr[i];
  }
  return ans;
}

void swap(int arr[], int i1, int i2)
{
  int tmp = arr[i1];
  arr[i1] = arr[i2];
  arr[i2] = tmp;
}

// 这一步做完，值等于pivot的区间确定
int* partition(int arr[], int L, int R, int pivot)
{
  int less = L - 1;
  int more = R + 1;
  int cur = L;
  while (cur < more)
  {
    if (arr[cur] < pivot)
    {
      swap(arr, ++less, cur++);
    }
    else if (arr[cur] > pivot)
    {
      swap(arr, cur, --more);
    }
    else
    {
      cur++;
    }
  }
  return new int[] { less + 1, more - 1 };  // 这个范围的数是等于pivot的数
}
// arr 第k小的数
// process2(arr, 0, N-1, k-1)
// arr[L..R]  范围上，如果排序的话(不是真的去排序)，找位于index的数
// index [L..R]
int process2(int arr[], int L, int R, int index)
{
  if (L == R)
  {
    // L = =R ==INDEX
    return arr[L];
  }
  // 不止一个数  L +  [0, R -L]
  int pivot = arr[L + getRandom(0, R - L + 1)]; // 这里用random是为了保证哨兵的随机性
  int* range = partition(arr, L, R, pivot);  // range 返回的是等于pivot的区间
  if (index >= range[0] && index <= range[1])  // 长度在区间内
  {
    return arr[index];
  }
  else if (index < range[0]) // 小于这个区间，就从左半区找
  {
    return process2(arr, L, range[0] - 1, index);
  }
  else  // 大于这个区间就从右半区找
  {
    return process2(arr, range[1] + 1, R, index);
  }
}

// 改写快排，时间复杂度O(N)
// k >= 1
int minKth2(int array[], int len, int k)
{
  int* arr = copyArray(array, len);  // c++应该free(arr)
  return process2(arr, 0, len - 1, k - 1);
}

/**************** Way 3 ************************/
int medianOfMedians(int arr[], int L, int R);

// arr[L..R]  如果排序的话，位于index位置的数，是什么，返回
int bfprt(int arr[], int L, int R, int index)
{
  if (L == R)
  {
    return arr[L];
  }
  // L...R  每五个数一组
  // 每一个小组内部排好序
  // 小组的中位数组成新数组
  // 这个新数组的中位数返回
  int pivot = medianOfMedians(arr, L, R);
  int* range = partition(arr, L, R, pivot);
  if (index >= range[0] && index <= range[1])
  {
    return arr[index];
  }
  else if (index < range[0])
  {
    return bfprt(arr, L, range[0] - 1, index);
  }
  else
  {
    return bfprt(arr, range[1] + 1, R, index);
  }
}

void insertionSort(int arr[], int L, int R)
{
  for (int i = L + 1; i <= R; i++)
  {
    for (int j = i - 1; j >= L && arr[j] > arr[j + 1]; j--)
    {
      swap(arr, j, j + 1);
    }
  }
}

int getMedian(int arr[], int L, int R)
{
  insertionSort(arr, L, R);
  return arr[(L + R) / 2];
}

// arr[L...R]  五个数一组
// 每个小组内部排序
// 每个小组中位数领出来，组成marr
// marr中的中位数，返回
int medianOfMedians(int arr[], int L, int R)
{
  int size = R - L + 1;
  int offset = size % 5 == 0 ? 0 : 1;
  int len = size / 5 + offset;
  int* mArr = new int[len];
  for (int team = 0; team < len; team++)
  {
    int teamFirst = L + team * 5;
    // L ... L + 4
    // L +5 ... L +9
    // L +10....L+14
    mArr[team] = getMedian(arr, teamFirst, std::min(R, teamFirst + 4));
  }
  // marr中，找到中位数
  return bfprt(mArr, 0, len - 1, len / 2);
}

// 利用bfprt算法，时间复杂度O(N)
int minKth3(int array[], int len, int k)
{
  int* arr = copyArray(array, len);
  return bfprt(arr, 0, len - 1, k - 1);
}
