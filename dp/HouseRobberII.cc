#include <iostream>
#include <vector>

using namespace std;

// 问题描述：
// 打家劫舍

// 测试链接：https://leetcode.cn/problems/house-robber-ii/

class HouseRobberII
{
 public:
  // arr 长度大于等于1
  // 从arr中选择子序列，元素不能相邻，求最大和
  static int pickMaxSum(vector<int> &arr)
  {
    int n = arr.size();
    // dp[i] : arr[0..i]范围上，随意选择，但是，任何两数不能相邻。得到的最大累加和是多少？
    vector<int> dp(n);
    dp[0] = arr[0];
    dp[1] = std::max(arr[0], arr[1]);
    for (int i = 2; i < n; i++)
    {
      int p1 = arr[i];
      int p2 = dp[i - 1];
      int p3 = arr[i] + dp[i - 2];
      dp[i] = std::max(p1, std::max(p2, p3));
    }
    return dp[n - 1];
  }

  // 升级，环形数组
  static int rob(vector<int> &nums)
  {
    if (nums.size() == 0)
    {
      return 0;
    }
    if (nums.size() == 1)
    {
      return nums[0];
    }
    if (nums.size() == 2)
    {
      return std::max(nums[0], nums[1]);
    }
    // 分两种情况
    // 如果选了0位置，就不能取n-1位置
    // 如果没选0位置，就可以选n-1位置
    // 取两种情况的最大值
    // 0...n-2
    int pre2 = nums[0];
    int pre1 = std::max(nums[0], nums[1]);
    for (int i = 2; i < nums.size() - 1; i++)
    {
      int tmp = std::max(pre1, nums[i] + pre2);
      pre2 = pre1;
      pre1 = tmp;
    }
    int ans1 = pre1;
    // 1...n-1
    pre2 = nums[1];
    pre1 = std::max(nums[1], nums[2]);
    for (int i = 3; i < nums.size(); i++)
    {
      int tmp = std::max(pre1, nums[i] + pre2);
      pre2 = pre1;
      pre1 = tmp;
    }
    int ans2 = pre1;
    return std::max(ans1, ans2);
  }
};

int main()
{
  return 0;
}
