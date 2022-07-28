#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;
using TdArray = vector<vector<int>>;

// 问题描述：
// 给定一个正整数数组 nums和一个整数 k ，返回 num 中 「好子数组」 的数目。
// 如果 nums 的某个子数组中不同整数的个数恰好为 k，则称 nums 的这个连续、不一定不同的子数组为 「好子数组 」。
// 例如，[1,2,3,1,2] 中有 3 个不同的整数：1，2，以及 3。
// 子数组 是数组的 连续 部分。

// 测试链接：https://leetcode.cn/problems/subarrays-with-k-different-integers/

class SubarraysWithKDifferentIntegers
{
 public:
  // nums 数组，题目规定，nums中的数字，不会超过nums的长度
  // [ ]长度为5，0~5
  static int subarraysWithKDistinct1(vector<int> &nums, int k)
  {
    int n = nums.size();
    // k-1种数的窗口词频统计
    vector<int> lessCounts(n + 1);
    // k种数的窗口词频统计
    vector<int> equalCounts(n + 1);
    int lessLeft = 0;
    int equalLeft = 0;
    int lessKinds = 0;
    int equalKinds = 0;
    int ans = 0;
    for (int r = 0; r < n; r++)
    {
      // 当前刚来到r位置！
      if (lessCounts[nums[r]] == 0)
      {
        lessKinds++;
      }
      if (equalCounts[nums[r]] == 0)
      {
        equalKinds++;
      }
      lessCounts[nums[r]]++;
      equalCounts[nums[r]]++;
      while (lessKinds == k)
      {
        if (lessCounts[nums[lessLeft]] == 1)
        {
          lessKinds--;
        }
        lessCounts[nums[lessLeft++]]--;
      }
      while (equalKinds > k)
      {
        if (equalCounts[nums[equalLeft]] == 1)
        {
          equalKinds--;
        }
        equalCounts[nums[equalLeft++]]--;
      }
      ans += lessLeft - equalLeft;
    }
    return ans;
  }

  static int subarraysWithKDistinct2(vector<int> &arr, int k) { return numsMostK(arr, k) - numsMostK(arr, k - 1); }

  static int numsMostK(vector<int> &arr, int k)
  {
    int i = 0, res = 0;
    unordered_map<int, int> count;
    for (int j = 0; j < arr.size(); ++j)
    {
      if (count[arr[j]] == 0)
      {
        k--;
      }
      count[arr[j]]++;
      while (k < 0)
      {
        count[arr[i]]--;
        if (count[arr[i]] == 0)
        {
          k++;
        }
        i++;
      }
      res += j - i + 1;
    }
    return res;
  }
};

int main()
{
  return 0;
}
