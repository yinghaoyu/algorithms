#include <iostream>
#include <vector>

using namespace std;

// 问题描述：
// 给你一个整数数组 nums ，请你找出数组中乘积最大的非空连续子数组（该子数组中至少包含一个数字），并返回该子数组所对应的乘积。
// 子数组 是数组的连续子序列。

class MaximumProductSubarray
{
 public:
  static double max(vector<double> &arr)
  {
    if (arr.size() == 0)
    {
      return 0;  // 报错！
    }
    int n = arr.size();
    // 上一步的最大
    double premax = arr[0];
    // 上一步的最小
    double premin = arr[0];
    double ans = arr[0];
    for (int i = 1; i < n; i++)
    {
      // dpmax[i]表示必须以i结尾的子数组，最大累乘积
      // dpmin[i]表示必须以i结尾的子数组，最小累成积
      // 可能性一：
      // 0.001 0.01 10
      //            ↑    --> 这个时候10最大
      //            i
      // 可能性二：
      // 1 2 3
      //     ↑            -->这个时候3 * dpmax[i-1]最大
      //     i
      // 可能性三：
      // -12 -2 -3
      //         ↑       --> 这个时候-3 * dpmin[i-1]最大
      double p1 = arr[i];
      double p2 = arr[i] * premax;
      double p3 = arr[i] * premin;
      double curmax = std::max(std::max(p1, p2), p3);
      double curmin = std::min(std::min(p1, p2), p3);
      ans = std::max(ans, curmax);
      premax = curmax;
      premin = curmin;
    }
    return ans;
  }

  static int maxProduct(vector<int> &nums)
  {
    int ans = nums[0];
    int min = nums[0];
    int max = nums[0];
    for (int i = 1; i < nums.size(); i++)
    {
      int curmin = std::min(nums[i], std::min(min * nums[i], max * nums[i]));
      int curmax = std::max(nums[i], std::max(min * nums[i], max * nums[i]));
      min = curmin;
      max = curmax;
      ans = std::max(ans, max);
    }
    return ans;
  }
};

int main()
{
  return 0;
}
