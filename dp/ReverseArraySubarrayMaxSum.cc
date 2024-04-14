#include <cstdint>
#include <vector>

using namespace std;

// @sa https://www.bilibili.com/video/BV1qu4y187Xn/
class ReverseArraySubarrayMaxSum
{
 public:
  // 为了验证
  int maxSumReverse1(vector<int>& nums)
  {
    int ans = INT32_MIN;
    for (int l = 0; l < nums.size(); l++)
    {
      for (int r = l; r < nums.size(); r++)
      {
        reverse(nums, l, r);
        ans = std::max(ans, maxSum(nums));
        reverse(nums, l, r);
      }
    }
    return ans;
  }

  // nums[l...r]范围上的数字进行逆序调整
  void reverse(vector<int>& nums, int l, int r)
  {
    while (l < r)
    {
      int tmp = nums[l];
      nums[l++] = nums[r];
      nums[r--] = tmp;
    }
  }

  // 返回子数组最大累加和
  int maxSum(vector<int>& nums)
  {
    int n = nums.size();
    int ans = nums[0];
    for (int i = 1, pre = nums[0]; i < n; i++)
    {
      pre = std::max(nums[i], pre + nums[i]);
      ans = std::max(ans, pre);
    }
    return ans;
  }

  // 正式方法
  // 时间复杂度O(n)
  int maxSumReverse2(vector<int>& nums)
  {
    int n = nums.size();
    // start[i] : 所有必须以i开头的子数组中，最大累加和是多少
    vector<int> start(n);
    start[n - 1] = nums[n - 1];
    for (int i = n - 2; i >= 0; i--)
    {
      // nums[i]
      // nums[i] + start[i+1]
      start[i] = std::max(nums[i], nums[i] + start[i + 1]);
    }
    int ans = start[0];
    // end : 子数组必须以i-1结尾，其中的最大累加和
    int end = nums[0];
    // maxEnd :
    // 0~i-1范围上，
    // 子数组必须以0结尾，其中的最大累加和
    // 子数组必须以1结尾，其中的最大累加和
    // ...
    // 子数组必须以i-1结尾，其中的最大累加和
    // 所有情况中，最大的那个累加和就是maxEnd
    int maxEnd = nums[0];
    for (int i = 1; i < n; i++)
    {
      // maxend   i....
      // 枚举划分点 i...
      ans = std::max(ans, maxEnd + start[i]);
      // 子数组必须以i结尾，其中的最大累加和
      end = std::max(nums[i], end + nums[i]);
      maxEnd = std::max(maxEnd, end);
    }
    // 翻转1个数，等于没翻转，比如 [-5, -2, 4, -3, -6]
    // 答案应该是 4
    ans = std::max(ans, maxEnd);
    return ans;
  }
};
