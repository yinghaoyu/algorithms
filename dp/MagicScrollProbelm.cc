#include <algorithm>
#include <cstdint>
#include <vector>

using namespace std;

// 魔法卷轴
// 给定一个数组nums，其中可能有正、负、0
// 每个魔法卷轴可以把nums中连续的一段全变成0
// 你希望数组整体的累加和尽可能大
// 卷轴使不使用、使用多少随意，但一共只有2个魔法卷轴
// 请返回数组尽可能大的累加和

// @sa https://www.bilibili.com/video/BV1qu4y187Xn/
class MagicScrollProbelm
{
  // 暴力方法
 public:
  int maxSum1(vector<int>& nums)
  {
    int p1 = 0;
    for (int num : nums)
    {
      p1 += num;
    }
    int n = nums.size();
    int p2 = mustOneScroll(nums, 0, n - 1);
    int p3 = INT32_MIN;
    for (int i = 1; i < n; i++)
    {
      p3 = std::max(p3, mustOneScroll(nums, 0, i - 1) + mustOneScroll(nums, i, n - 1));
    }
    return std::max({p1, p2, p3});
  }

  // 暴力方法
  // nums[l...r]范围上一定要用一次卷轴情况下的最大累加和
  int mustOneScroll(vector<int>& nums, int l, int r)
  {
    int ans = INT32_MIN;
    // l...r范围上包含a...b范围
    // 如果a...b范围上的数字都变成0
    // 返回剩下数字的累加和
    // 所以枚举所有可能的a...b范围
    // 相当暴力，但是正确
    for (int a = l; a <= r; a++)
    {
      for (int b = a; b <= r; b++)
      {
        // l...a...b...r
        int curAns = 0;
        for (int i = l; i < a; i++)
        {
          curAns += nums[i];
        }
        for (int i = b + 1; i <= r; i++)
        {
          curAns += nums[i];
        }
        ans = std::max(ans, curAns);
      }
    }
    return ans;
  }

  // 正式方法
  // 时间复杂度O(n)
  int maxSum2(vector<int>& nums)
  {
    int n = nums.size();
    if (n == 0)
    {
      return 0;
    }
    // 情况1 : 完全不使用卷轴
    int p1 = 0;
    for (int num : nums)
    {
      p1 += num;
    }
    // prefix[i] : 0~i范围上一定要用1次卷轴的情况下，0~i范围上整体最大累加和多少
    vector<int> prefix(n);
    // 每一步的前缀和
    int sum = nums[0];
    // maxPresum : 之前所有前缀和的最大值
    int maxPresum = std::max(0, nums[0]);
    for (int i = 1; i < n; i++)
    {
      prefix[i] = std::max(prefix[i - 1] + nums[i], maxPresum);
      sum += nums[i];
      maxPresum = std::max(maxPresum, sum);
    }
    // 情况二 : 必须用1次卷轴
    int p2 = prefix[n - 1];
    // suffix[i] : i~n-1范围上一定要用1次卷轴的情况下，i~n-1范围上整体最大累加和多少
    vector<int> suffix(n);
    sum = nums[n - 1];
    maxPresum = std::max(0, sum);
    for (int i = n - 2; i >= 0; i--)
    {
      suffix[i] = std::max(nums[i] + suffix[i + 1], maxPresum);
      sum += nums[i];
      maxPresum = std::max(maxPresum, sum);
    }
    // 情况二 : 必须用2次卷轴
    int p3 = INT32_MIN;
    for (int i = 1; i < n; i++)
    {
      // 枚举所有的划分点i
      // 0~i-1 左
      // i~n-1 右
      p3 = std::max(p3, prefix[i - 1] + suffix[i]);
    }
    return std::max({p1, p2, p3});
  }
};
