#include <iostream>
#include <vector>

using namespace std;

// 问题描述：
// n 个孩子站成一排。给你一个整数数组 ratings 表示每个孩子的评分。
// 你需要按照以下要求，给这些孩子分发糖果：
// 1、每个孩子至少分配到 1 个糖果。
// 2、相邻两个孩子评分更高的孩子会获得更多的糖果。
// 请你给每个孩子分发糖果，计算并返回需要准备的 最少糖果数目 。

// 测试链接 : https://leetcode.com/problems/candy/
class CandyProblem
{
 public:
  // 这是原问题的优良解
  // 时间复杂度O(N)，额外空间复杂度O(N)
  static int candy1(vector<int> &arr)
  {
    if (arr.size() == 0)
    {
      return 0;
    }
    int N = arr.size();
    vector<int> left(N);
    for (int i = 1; i < N; i++)  // 从左往右，递增糖果数量+1
    {
      if (arr[i - 1] < arr[i])
      {
        left[i] = left[i - 1] + 1;
      }
    }
    vector<int> right(N);
    for (int i = N - 2; i >= 0; i--)  // 从右往左，递增糖果数量+1
    {
      if (arr[i] > arr[i + 1])
      {
        right[i] = right[i + 1] + 1;
      }
    }
    int ans = 0;
    for (int i = 0; i < N; i++)  // 求最大值
    {
      ans += std::max(left[i], right[i]);
    }
    //本来left和right数组元素应该初始化为1，因为每个孩子最少分1颗糖果
    // 由于left和right数组初始化为0了，因此这里需要+N
    return ans + N;
  }

  // 这是原问题空间优化后的解
  // 时间复杂度O(N)，额外空间复杂度O(1)
  static int candy2(vector<int> &arr)
  {
    if (arr.size() == 0)
    {
      return 0;
    }
    int index = nextMinIndex2(arr, 0);
    int res = rightCands(arr, 0, index++);
    int lbase = 1;
    int next = 0;
    int rcands = 0;
    int rbase = 0;
    while (index != arr.size())
    {
      if (arr[index] > arr[index - 1])
      {
        res += ++lbase;
        index++;
      }
      else if (arr[index] < arr[index - 1])
      {
        next = nextMinIndex2(arr, index - 1);
        rcands = rightCands(arr, index - 1, next++);
        rbase = next - index + 1;
        res += rcands + (rbase > lbase ? -lbase : -rbase);
        lbase = 1;
        index = next;
      }
      else
      {
        res += 1;
        lbase = 1;
        index++;
      }
    }
    return res;
  }

  static int nextMinIndex2(vector<int> &arr, int start)
  {
    for (int i = start; i != arr.size() - 1; i++)
    {
      if (arr[i] <= arr[i + 1])
      {
        return i;
      }
    }
    return arr.size() - 1;
  }

  static int rightCands(vector<int> arr, int left, int right)
  {
    int n = right - left + 1;
    return n + n * (n - 1) / 2;
  }

  // 进阶问题：加上条件 3、相邻的孩子分的糖一样
  // 这是进阶问题的最优解，不要提交这个
  // 时间复杂度O(N), 额外空间复杂度O(1)
  static int candy3(vector<int> arr)
  {
    if (arr.size() == 0)
    {
      return 0;
    }
    int index = nextMinIndex3(arr, 0);
    vector<int> data = rightCandsAndBase(arr, 0, index++);
    int res = data[0];
    int lbase = 1;
    int same = 1;
    int next = 0;
    while (index != arr.size())
    {
      if (arr[index] > arr[index - 1])
      {
        res += ++lbase;
        same = 1;
        index++;
      }
      else if (arr[index] < arr[index - 1])
      {
        next = nextMinIndex3(arr, index - 1);
        data = rightCandsAndBase(arr, index - 1, next++);
        if (data[1] <= lbase)
        {
          res += data[0] - data[1];
        }
        else
        {
          res += -lbase * same + data[0] - data[1] + data[1] * same;
        }
        index = next;
        lbase = 1;
        same = 1;
      }
      else
      {
        res += lbase;
        same++;
        index++;
      }
    }
    return res;
  }

  static int nextMinIndex3(vector<int> &arr, int start)
  {
    for (int i = start; i != arr.size() - 1; i++)
    {
      if (arr[i] < arr[i + 1])
      {
        return i;
      }
    }
    return arr.size() - 1;
  }

  static vector<int> rightCandsAndBase(vector<int> &arr, int left, int right)
  {
    int base = 1;
    int cands = 1;
    for (int i = right - 1; i >= left; i--)
    {
      if (arr[i] == arr[i + 1])
      {
        cands += base;
      }
      else
      {
        cands += ++base;
      }
    }
    return vector<int>{cands, base};
  }

  static void test()
  {
    vector<int> test1 = {3, 0, 5, 5, 4, 4, 0};
    cout << candy2(test1) << endl;

    vector<int> test2 = {3, 0, 5, 5, 4, 4, 0};
    cout << candy3(test2) << endl;
  }
};

int main()
{
  return 0;
}
