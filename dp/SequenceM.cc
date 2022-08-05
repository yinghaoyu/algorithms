#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

// 给定一个数组arr，arr[i] = j，表示第i号试题的难度为j。给定一个非负数M
// 想出一张卷子，对于任何相邻的两道题目，前一题的难度不能超过后一题的难度+M
// 返回所有可能的卷子种数
class SequenceM
{
  // 纯暴力方法，生成所有排列，一个一个验证
 public:
  static int ways1(vector<int> &arr, int m)
  {
    if (arr.size() == 0)
    {
      return 0;
    }
    return process(arr, 0, m);
  }

  static int process(vector<int> &arr, int index, int m)
  {
    if (index == arr.size())
    {
      for (int i = 1; i < index; i++)
      {
        if (arr[i - 1] > arr[i] + m)
        {
          return 0;
        }
      }
      return 1;
    }
    int ans = 0;
    for (int i = index; i < arr.size(); i++)
    {
      swap(arr, index, i);
      ans += process(arr, index + 1, m);
      swap(arr, index, i);
    }
    return ans;
  }

  static void swap(vector<int> &arr, int i, int j)
  {
    int tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
  }

  // 时间复杂度O(N * logN)
  // 从左往右的动态规划 + 范围上二分
  static int ways2(vector<int> &arr, int m)
  {
    if (arr.size() == 0)
    {
      return 0;
    }
    std::sort(arr.begin(), arr.end());
    int all = 1;
    for (int i = 1; i < arr.size(); i++)
    {
      all = all * (num(arr, i - 1, arr[i] - m) + 1);
    }
    return all;
  }

  // arr[0..r]上返回>=t的数有几个, 二分的方法
  // 找到 >=t 最左的位置a, 然后返回r - a + 1就是个数
  static int num(vector<int> &arr, int r, int t)
  {
    int i = 0;
    int j = r;
    int m = 0;
    int a = r + 1;
    while (i <= j)
    {
      m = (i + j) / 2;
      if (arr[m] >= t)
      {
        a = m;
        j = m - 1;
      }
      else
      {
        i = m + 1;
      }
    }
    return r - a + 1;
  }

  // 时间复杂度O(N * logV)
  // 从左往右的动态规划 + IndexTree
  static int ways3(vector<int> &arr, int m)
  {
    if (arr.size() == 0)
    {
      return 0;
    }
    int max = INT32_MIN;
    int min = INT32_MAX;
    for (int num : arr)
    {
      max = std::max(max, num);
      min = std::min(min, num);
    }
    IndexTree itree(max - min + 2);
    std::sort(arr.begin(), arr.end());
    int a = 0;
    int b = 0;
    int all = 1;
    itree.add(arr[0] - min + 1, 1);
    for (int i = 1; i < arr.size(); i++)
    {
      a = arr[i] - min + 1;
      b = i - (a - m - 1 >= 1 ? itree.sum(a - m - 1) : 0);
      all = all * (b + 1);
      itree.add(a, 1);
    }
    return all;
  }

  // 注意开始下标是1，不是0
  class IndexTree
  {
   private:
    vector<int> tree;

    int N;

   public:
    IndexTree(int size)
    {
      N = size;
      tree = vector<int>(N + 1);
    }

    int sum(int index)
    {
      int ret = 0;
      while (index > 0)
      {
        ret += tree[index];
        index -= index & -index;
      }
      return ret;
    }

    void add(int index, int d)
    {
      while (index <= N)
      {
        tree[index] += d;
        index += index & -index;
      }
    }
  };

  // 为了测试
  static vector<int> randomArray(int len, int value)
  {
    vector<int> arr(len);
    for (int i = 0; i < len; i++)
    {
      arr[i] = getRandom(1, value);
    }
    return arr;
  }

  static int getRandom(int min, int max)
  {
    random_device seed;                            // 硬件生成随机数种子
    ranlux48 engine(seed());                       // 利用种子生成随机数引
    uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
    int res = distrib(engine);                     // 随机数
    return res;
  }

  // 为了测试
  static void test()
  {
    int N = 10;
    int value = 20;
    int testTimes = 1000;
    cout << "测试开始" << endl;
    for (int i = 0; i < testTimes; i++)
    {
      int len = getRandom(1, N);
      vector<int> arr = randomArray(len, value);
      int m = getRandom(1, value);
      int ans1 = ways1(arr, m);
      int ans2 = ways2(arr, m);
      int ans3 = ways3(arr, m);
      if (ans1 != ans2 || ans1 != ans3)
      {
        cout << "出错了!" << endl;
        cout << ans1 << endl;
        cout << ans2 << endl;
        cout << ans3 << endl;
      }
    }
    cout << "测试结束" << endl;
  }
};

int main()
{
  return 0;
}
