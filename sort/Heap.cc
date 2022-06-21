#include <stdbool.h>
#include <iostream>

// 堆排序时间复杂度N*log(N)
// 稳定性：不稳定，比如3 27 36 27如果堆顶3先输出，则最后一个27先被交换到堆顶
//
// 向上建堆和向下建堆的差异
// https://blog.csdn.net/weixin_61061381/article/details/125030743

#define limit 1000

int heap[limit];
int heapSize;

bool isEmpty()
{
  return heapSize == 0;
}

bool isFull()
{
  return heapSize == limit;
}

void swap(int arr[], int i, int j)
{
  int tmp = arr[i];
  arr[i] = arr[j];
  arr[j] = tmp;
}

// 新加进来的数，现在停在了index位置，请依次往上移动，
// 移动到0位置，或者干不掉自己的父亲了，停！
// 时间复杂度：log(N)
void heapInsert(int arr[], int index)
{
  // [index]    [index-1]/2
  // index == 0
  while (arr[index] > arr[(index - 1) / 2])
  {
    swap(arr, index, (index - 1) / 2);
    index = (index - 1) / 2;
  }
}

// 从index位置，往下看，不断的下沉
// 停：较大的孩子都不再比index位置的数大；已经没孩子了
// 时间复杂度：log(N)
void heapify(int arr[], int index, int heapSize)
{
  int left = index * 2 + 1;  // 左孩子的下标
  while (left < heapSize)    // 下方还有孩子的时候
  {
    // 如果有左孩子，有没有右孩子，可能有可能没有！
    // 两个孩子中，谁的值大，把下标给largest
    // 1）只有左孩子，left -> largest
    // 2) 同时有左孩子和右孩子，右孩子的值<= 左孩子的值，left -> largest
    // 3) 同时有左孩子和右孩子并且右孩子的值> 左孩子的值， right -> largest
    int largest = left + 1 < heapSize && arr[left + 1] > arr[left] ? left + 1 : left;
    largest = arr[largest] > arr[index] ? largest : index;
    if (largest == index)
    {
      break;
    }
    // index和较大孩子，要互换
    swap(arr, largest, index);
    index = largest;
    left = index * 2 + 1;
  }
}

void push(int value)
{
  if (heapSize == limit)
  {
    return;
  }
  heap[heapSize] = value;
  // value  heapSize
  heapInsert(heap, heapSize++);
}

// 用户此时，让你返回最大值，并且在大根堆中，把最大值删掉
// 剩下的数，依然保持大根堆组织
int pop()
{
  int ans = heap[0];
  swap(heap, 0, --heapSize);
  heapify(heap, 0, heapSize);
  return ans;
}

// 堆排序额外空间复杂度O(1)
void heapSort(int arr[], int len)
{
  if (arr == nullptr || len < 2)
  {
    return;
  }
  // 自顶向下的建堆方式O(N*logN)
  //    for (int i = 0; i < arr.length; i++) { // O(N)
  //      heapInsert(arr, i); // O(logN)
  //    }
  // 自底向上的建堆方式，即Floyd算法，时间复杂度O(N)
  // 前提条件：根结点的左右子树均为小堆或大堆才可
  // 而这里，数组为乱序的，无法直接从根结点开始向下调整
  // 所以一般从倒数第一个非叶结点开始向下调整，从下往上调
  for (int i = len - 1; i >= 0; i--)
  {
    heapify(arr, i, len);
  }
  int heapSize = len;
  swap(arr, 0, --heapSize);
  // O(N*logN)
  while (heapSize > 0)  // O(N)
  {
    heapify(arr, 0, heapSize);  // O(logN)
    swap(arr, 0, --heapSize);   // O(1)
  }
}
