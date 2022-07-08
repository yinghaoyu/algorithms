#include <stdbool.h>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

// 这道题直接在leetcode测评：
// https://leetcode.com/problems/count-of-range-sum/
//
// 给你一个整数数组 nums 以及两个整数 lower 和 upper 。求数组中，值位于范围 [lower, upper] （包含 lower 和 upper）之内的 区间和的个数 。

// 核心思路如下：

// 1.使用前缀和代替原数组；

// 2.在归并排序合并方法内，统计满足条件的累加和个数 和 合并操作分开。

// 3.每次合并操作，对于右组（前缀和数组）中的每一个数arr[i]
// 求左组（前缀和数组）所有数中有多少个范围在 [arr[i] - upper, arr[i] - lower]上，将满足条件的个数累加起来即为最后的结果

// 这是如何联系起来的？
// 因为在归并的过程中，左组组内是有序的，右组组内也是有序的
// 这样对于右组所有的arr[i]，从左组找到区间[l, r]
// 使得[arr[i] - arr[r], arr[i] - arr[l]]在区间[lower, upper]内就可以了

class CountOfRangeSum
{
 public:
  static int countRangeSum(vector<int> &nums, int lower, int upper)
  {
    if (nums.size() == 0)
    {
      return 0;
    }
    vector<long> sum(nums.size());
    sum[0] = nums[0];
    for (int i = 1; i < nums.size(); i++)
    {
      sum[i] = sum[i - 1] + nums[i];  // 前缀和
    }
    int ans = process(sum, 0, nums.size() - 1, lower, upper);
    return ans;
  }

  static int process(vector<long> sum, int L, int R, int lower, int upper)
  {
    if (L == R)
    {
      // L == R时，sum[L] 表示原数组 [0, L]范围上的累加和
      // 在merge过程中，会忽略掉左组一个数也没有的这种情况，所以在这里补充这种情况
      return sum[L] >= lower && sum[L] <= upper ? 1 : 0;
    }
    int M = L + ((R - L) >> 1);
    return process(sum, L, M, lower, upper) + process(sum, M + 1, R, lower, upper) + merge(sum, L, M, R, lower, upper);
  }

  static int merge(vector<long> arr, int L, int M, int R, int lower, int upper)
  {
    int ans = 0;
    // 左组寻找的左侧位置（肯定是从当前的L位置开始）
    int windowL = L;
    // 左组寻找的右侧位置（肯定也是从当前的L位置开始）
    int windowR = L;
    // [windowL, windowR)
    // 对于右组的每一个数arr[i]，在左组中寻找值在[arr[i]-upper, arr[i]-lower]之间的个数
    for (int i = M + 1; i <= R; i++)
    {
      long min = arr[i] - upper;
      long max = arr[i] - lower;
      // 因为是在左组中寻找，所以下标不能超过mid
      // 寻找当前值比max大的第一个位置（因为等于max的时候右移了一位，所以不包含此位置）
      while (windowR <= M && arr[windowR] <= max)
      {
        windowR++;
      }
      // 寻找当前值大于等于min的第一个位置（因为等于min的时候没有右移，所以包含此位置）
      while (windowL <= M && arr[windowL] < min)
      {
        windowL++;
      }
      // 最后满足要求的累加和个数为 [windowL, windowR)
      // 即 windowR - windowL，windowR是开区间，所以不 +1
      ans += windowR - windowL;
    }

    int len = R - L + 1;
    vector<long> help(len);
    int i = 0;
    int p1 = L;
    int p2 = M + 1;
    while (p1 <= M && p2 <= R)
    {
      help[i++] = arr[p1] <= arr[p2] ? arr[p1++] : arr[p2++];
    }
    while (p1 <= M)
    {
      help[i++] = arr[p1++];
    }
    while (p2 <= R)
    {
      help[i++] = arr[p2++];
    }
    for (i = 0; i < len; i++)
    {
      arr[L + i] = help[i];
    }
    return ans;
  }
};

int main()
{
  vector<int> arr{1, 2, 3, 4, 1, 1, 2};
  cout << CountOfRangeSum::countRangeSum(arr, 3, 4) << endl;
  return 0;
}
