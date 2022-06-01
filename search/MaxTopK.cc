#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

class MaxTopK
{
 public:
  // 时间复杂度O(N*logN)
  // 排序+收集
  static int *maxTopK1(int *arr, int len, int k)
  {
    if (arr == nullptr || len == 0)
    {
      return new int[0];
    }
    int N = len;
    k = std::min(N, k);
    std::sort(arr, arr + len);  // 直接使用sort的归并排序
    int *ans = new int[k]();
    for (int i = N - 1, j = 0; j < k; i--, j++)
    {
      // 将后k个数给ans
      ans[j] = arr[i];
    }
    return ans;
  }

  // 方法二，时间复杂度O(N + K*logN)
  // 解释：堆
  static int *maxTopK2(int *arr, int len, int k)
  {
    if (arr == nullptr || len == 0)
    {
      return new int[0];
    }
    int N = len;
    k = std::min(N, k);
    // 从底向上建堆，时间复杂度O(N)
    for (int i = N - 1; i >= 0; i--)
    {
      heapify(arr, i, N);
    }
    // 只把前K个数放在arr末尾，然后收集，O(K*logN)
    int heapSize = N;
    swap(arr, 0, --heapSize);
    int count = 1;
    while (heapSize > 0 && count < k)
    {
      heapify(arr, 0, heapSize);
      swap(arr, 0, --heapSize);
      count++;
    }
    int *ans = new int[k]();
    for (int i = N - 1, j = 0; j < k; i--, j++)
    {
      ans[j] = arr[i];
    }
    return ans;
  }

  static void heapInsert(int *arr, int index)
  {
    while (arr[index] > arr[(index - 1) / 2])
    {
      swap(arr, index, (index - 1) / 2);
      index = (index - 1) / 2;
    }
  }

  static void heapify(int *arr, int index, int heapSize)
  {
    int left = index * 2 + 1;
    while (left < heapSize)
    {
      int largest = left + 1 < heapSize && arr[left + 1] > arr[left] ? left + 1 : left;
      largest = arr[largest] > arr[index] ? largest : index;
      if (largest == index)
      {
        break;
      }
      swap(arr, largest, index);
      index = largest;
      left = index * 2 + 1;
    }
  }

  static void swap(int *arr, int i, int j)
  {
    int tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
  }

  // 方法三，时间复杂度O(n + k * logk)
  static int *maxTopK3(int *arr, int len, int k)
  {
    if (arr == nullptr || len == 0)
    {
      return new int[0];
    }
    int N = len;
    k = std::min(N, k);
    // O(N)
    int num = minKth(arr, len, N - k);  // num是第 N-k 小的数
    int *ans = new int[k]();
    int index = 0;
    for (int i = 0; i < N; i++)
    {
      // 把arr中所有大于num的数加入ans
      if (arr[i] > num)
      {
        ans[index++] = arr[i];
      }
    }
    for (; index < k; index++)
    {
      // 说明有多个等于num的数
      ans[index] = num;
    }
    // O(k*logk)
    sort(ans, ans + k);
    for (int L = 0, R = k - 1; L < R; L++, R--)
    {
      swap(ans, L, R);
    }
    return ans;
  }

  // 找到第index小的数
  // 时间复杂度O(N)
  static int minKth(int *arr, int len, int index)
  {
    int L = 0;
    int R = len - 1;
    int pivot = 0;
    int *range = nullptr;
    // 这里的时间复杂度为什么是O(N)？
    // 仔细观察可以发现while循环里的L或者R一直是往单向走的
    while (L < R)
    {
      pivot = arr[L + getRandom(0, R - L)];
      range = partition(arr, L, R, pivot);
      // 经过一次partition后，所有值为 pivot 的数位置都确定
      // 且都在区间 [range[0], range[1]] 上
      if (index < range[0])
      {
        // 第 index 小的数在区间[range[0], range[1]]的左边
        R = range[0] - 1;
      }
      else if (index > range[1])
      {
        // 第 index 小的数在区间[range[0], ranger[1]]的右边
        L = range[1] + 1;
      }
      else
      {
        // 第 index 小的数在区间[range[0], range[1]]上
        return pivot;
      }
    }
    return arr[L];
  }

  // 平均时间复杂度O(logN) 最坏O(N)
  static int *partition(int *arr, int L, int R, int pivot)
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
    return new int[2]{less + 1, more - 1};  // 这个区间的数等于pivot
  }

  // for test
  static int *generateRandomArray(int maxSize, int maxValue, int *len)
  {
    *len = 7;  // getRandom(0, maxSize);
    int *arr = new int[*len];
    for (int i = 0; i < *len; i++)
    {
      // [-? , +?]
      arr[i] = getRandom(0, maxValue) - getRandom(0, maxValue);
    }
    return arr;
  }

  // for test
  static int *copyArray(int *arr, int len)
  {
    if (arr == nullptr)
    {
      return nullptr;
    }
    int *res = new int[len]();
    for (int i = 0; i < len; i++)
    {
      res[i] = arr[i];
    }
    return res;
  }

  // for test
  static bool isEqual(int *arr1, int *arr2, int len1, int len2)
  {
    if ((arr1 == nullptr && arr2 != nullptr) || (arr1 != nullptr && arr2 == nullptr))
    {
      return false;
    }
    if (arr1 == nullptr && arr2 == nullptr)
    {
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
  static void printArray(int *arr, int len)
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

  static int getRandom(int min, int max)
  {
    random_device seed;                            // 硬件生成随机数种子
    ranlux48 engine(seed());                       // 利用种子生成随机数引
    uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
    int res = distrib(engine);                     // 随机数
    return res;
  }

  // 生成随机数组测试
  static void test()
  {
    int testTime = 500000;
    int maxSize = 100;
    int maxValue = 100;
    int len = 0;
    bool pass = true;
    cout << "测试开始，没有打印出错信息说明测试通过" << endl;
    for (int i = 0; i < testTime; i++)
    {
      int k = getRandom(1, maxSize);
      int *arr = generateRandomArray(maxSize, maxValue, &len);

      int *arr1 = copyArray(arr, len);
      int *arr2 = copyArray(arr, len);
      int *arr3 = copyArray(arr, len);
      int *ans1 = maxTopK1(arr1, len, k);
      int *ans2 = maxTopK2(arr2, len, k);
      int *ans3 = maxTopK3(arr3, len, k);
      k = std::min(len, k);
      if (!isEqual(ans1, ans2, k, k) || !isEqual(ans1, ans3, k, k))
      {
        pass = false;
        cout << "出错了！" << endl;
        printArray(ans1, k);
        printArray(ans2, k);
        printArray(ans3, k);
        break;
      }
    }
    cout << "测试结束了，测试了" << testTime << "组，是否所有测试用例都通过？" << (pass ? "是" : "否") << endl;
  }
};

int main()
{
  MaxTopK::test();
  return 0;
}
