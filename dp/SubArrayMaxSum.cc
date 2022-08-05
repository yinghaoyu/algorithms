#include <iostream>
#include <vector>

using namespace std;

// 问题描述：
// 给你一个整数数组arr，请你找出一个具有最大和的连续子数组（子数组最少包含一个元素），返回其最大和。
// 子数组是数组中的一个连续部分。
// arr = {-2,1,-3,4,-1,2,1,-5,4}
// dp[i]表示必须以i位置为结尾的子数组的最大和
// 如果0...i-1的子数组最大和为负数，那么dp[i]明显取arr[i]
// 如果0...i-1的子数组最大和为正数，那么dp[i]明显取arr[i]+dp[i-1]
// dp = {-2, 1, -2, 4, 3, 5, 6, 1, 5}
// 由于dp[i]依赖于dp[i-1]，因此可以空间优化

// 本题测试链接 : https://leetcode.com/problems/maximum-subarray/

class SubArrayMaxSum
{
  static int maxSubArray(vector<int> &arr)
  {
    if (arr.size() == 0)
    {
      return 0;
    }
    int max = INT32_MIN;
    int cur = 0;  // 用单个变量优化dp[]数组空间
    for (int i = 0; i < arr.size(); i++)
    {
      cur += arr[i];
      max = std::max(max, cur);
      cur = cur < 0 ? 0 : cur;
    }
    return max;
  }
};
