#include <stdbool.h>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

// 返回[min, max]
int getRandom(int min, int max)
{
  random_device seed;                            // 硬件生成随机数种子
  ranlux48 engine(seed());                       // 利用种子生成随机数引
  uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
  int res = distrib(engine);                     // 随机数
  return res;
}

// for test
void printArray(vector<int> &arr)
{
  if (arr.size() == 0)
  {
    return;
  }
  for (int i = 0; i < arr.size(); i++)
  {
    cout << arr[i] << " ";
  }
  cout << endl;
}

void swap(vector<int> &arr, int i, int j)
{
  int tmp = arr[i];
  arr[i] = arr[j];
  arr[j] = tmp;
}

// for test
vector<int> generateRandomArray(int maxSize, int maxValue, int *len)
{
  *len = getRandom(1, maxSize);
  vector<int> arr(*len);
  for (int i = 0; i < *len; i++)
  {
    arr[i] = getRandom(1, maxValue + 1) - getRandom(0, maxValue);
  }
  return arr;
}

// for test
bool isEqual(vector<int> &arr1, vector<int> &arr2)
{
  if (arr1.size() != arr2.size())
  {
    return false;
  }
  for (int i = 0; i < arr1.size(); i++)
  {
    if (arr1[i] != arr2[i])
    {
      return false;
    }
  }
  return true;
}

// arr[L..R]上，以arr[R]位置的数做划分值
// <= X > X
// <= X X
int partition(vector<int> &arr, int L, int R)
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

void process1(vector<int> &arr, int L, int R)
{
  if (L >= R)
  {
    return;
  }
  // L..R partition arr[R] [ <=arr[R] arr[R] >arr[R] ]
  int M = partition(arr, L, R);
  process1(arr, L, M - 1);
  process1(arr, M + 1, R);
}

// 快排递归版本
void quickSort1(vector<int> &arr)
{
  if (arr.size() < 2)
  {
    return;
  }
  process1(arr, 0, arr.size() - 1);
}

// arr[L...R] 玩荷兰国旗问题的划分，以arr[R]做划分值
// <arr[R] ==arr[R] > arr[R]
vector<int> netherlandsFlag(vector<int> &arr, int L, int R)
{
  if (L > R)
  {
    // L...R L>R
    return vector<int>{-1, -1};
  }
  if (L == R)
  {
    return vector<int>{L, R};
  }
  int less = L - 1;  // < 区 右边界
  int more = R;      // > 区 左边界
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
      swap(arr, index++, ++less);
    }
    else
    {
      // >
      swap(arr, index, --more);
    }
  }
  swap(arr, more, R);  // <[R]   =[R]   >[R]
  return vector<int>{less + 1, more};
}
void process2(vector<int> &arr, int L, int R)
{
  if (L >= R)
  {
    return;
  }
  int num = getRandom(0, R - L);  // 随机抽取哨兵
  swap(arr, L + num, R);
  vector<int> equalArea = netherlandsFlag(arr, L, R);
  process2(arr, L, equalArea[0] - 1);
  process2(arr, equalArea[1] + 1, R);
}

void quickSort2(vector<int> &arr)
{
  if (arr.size() < 2)
  {
    return;
  }
  process2(arr, 0, arr.size() - 1);
}

// 快排非递归版本需要的辅助类
// 要处理的是什么范围上的排序
class Op
{
 public:
  int l;
  int r;

  Op(int left, int right)
  {
    l = left;
    r = right;
  }
};

// 快排3.0 非递归版本
void quickSort3(vector<int> &arr)
{
  if (arr.size() < 2)
  {
    return;
  }
  int N = arr.size();
  swap(arr, getRandom(0, N - 1), N - 1);  // 随机抽取一个哨兵
  vector<int> equalArea = netherlandsFlag(arr, 0, N - 1);
  int el = equalArea[0];
  int er = equalArea[1];
  vector<Op> stack;
  stack.emplace_back(Op(0, el - 1));      // 把左区间入栈
  stack.emplace_back(Op(er + 1, N - 1));  // 把右区间入栈
  while (!stack.empty())
  {
    Op op = stack.back();  // 取出一个区间op.l  ... op.r
    stack.pop_back();
    if (op.l < op.r)
    {
      swap(arr, op.l + getRandom(0, op.r - op.l), op.r);
      equalArea = netherlandsFlag(arr, op.l, op.r);
      el = equalArea[0];
      er = equalArea[1];
      stack.emplace_back(Op(op.l, el - 1));
      stack.emplace_back(Op(er + 1, op.r));
    }
  }
}

// 跑大样本随机测试（对数器）
int main()
{
  int testTime = 5000;
  int maxSize = 100;
  int maxValue = 100;
  bool succeed = true;
  int len = 0;
  cout << "test begin" << endl;
  for (int i = 0; i < testTime; i++)
  {
    vector<int> arr1 = generateRandomArray(maxSize, maxValue, &len);
    vector<int> arr2 = arr1;
    vector<int> arr3 = arr1;
    quickSort1(arr1);
    quickSort2(arr2);
    quickSort3(arr3);
    if (!isEqual(arr1, arr2) || !isEqual(arr2, arr3))
    {
      succeed = false;
      break;
    }
  }
  cout << "test end" << endl;
  cout << "测试" << testTime << "组是否全部通过：" << (succeed ? "是" : "否") << endl;
  return 0;
}
