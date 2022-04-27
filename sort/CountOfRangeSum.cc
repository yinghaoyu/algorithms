#include <iostream>
#include <stdbool.h>
#include <random>

using namespace std;

// 这道题直接在leetcode测评：
// https://leetcode.com/problems/count-of-range-sum/
//
// 给你一个整数数组 nums 以及两个整数 lower 和 upper 。求数组中，值位于范围 [lower, upper] （包含 lower 和 upper）之内的 区间和的个数 。

// 核心思路如下：

// 1.使用前缀和代替原数组；

// 2.在归并排序合并方法内，统计满足条件的累加和个数 和 合并操作分开。

// 3.每次合并操作，对于右组（前缀和数组）中的每一个数arr[i]，求左组（前缀和数组）所有数中有多少个范围在 [arr[i] - upper, arr[i] - lower]上，将满足条件的个数累加起来即为最后的结果

class CountOfRangeSum
{
  public:
    static int countRangeSum(int nums[], int len, int lower, int upper)
    {
      if (nums == NULL || len == 0)
      {
        return 0;
      }
      long* sum = (long*)malloc(len * sizeof(long));
      sum[0] = nums[0];
      for (int i = 1; i < len; i++)
      {
        sum[i] = sum[i - 1] + nums[i];  // 前缀和
      }
      int ans = process(sum, 0, len - 1, lower, upper);
      free(sum);
      return ans;
    }

    static int process(long sum[], int L, int R, int lower, int upper)
    {
      if (L == R)
      {
        return sum[L] >= lower && sum[L] <= upper ? 1 : 0;
      }
      int M = L + ((R - L) >> 1);
      return process(sum, L, M, lower, upper) + process(sum, M + 1, R, lower, upper)
        + merge(sum, L, M, R, lower, upper);
    }

    static int merge(long arr[], int L, int M, int R, int lower, int upper)
    {
      int ans = 0;
      int windowL = L;  // 左组寻找的左侧位置（肯定是从当前的L位置开始）
      int windowR = L;  // 左组寻找的右侧位置（肯定也是从当前的L位置开始）
      // [windowL, windowR)
      for (int i = M + 1; i <= R; i++)  // 对于右组的每一个数arr[i]，在左组中寻找值在[arr[i]-upper, arr[i]-lower]之间的个数
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
        // 最后满足要求的累加和个数为 [windowL, windowR)，即 windowR - windowL，windowR是开区间，所以不 +1
        ans += windowR - windowL;
      }

      int len = R - L + 1;
      long* help = (long*)malloc(len * sizeof(long));
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
      free(help);
      return ans;
    }
};

int main()
{
  return 0;
}