#include <algorithm>
#include <queue>
#include <vector>

using namespace std;

//@sa https://www.bilibili.com/video/BV1NN41137jF/
class TopKMinimumSubsequenceSum
{
 public:
  // 为了验证，暴力方法，得到所有子序列的和
  void f1(vector<int>& nums, int index, int sum, vector<int>& ans)
  {
    if (index == nums.size())
    {
      ans.push_back(sum);
    }
    else
    {
      f1(nums, index + 1, sum, ans);
      f1(nums, index + 1, sum + nums[index], ans);
    }
  }

  // 暴力方法，为了验证
  vector<int> topKSum1(vector<int>& nums, int k)
  {
    vector<int> allSubsequences;
    f1(nums, 0, 0, allSubsequences);
    std::sort(allSubsequences.begin(), allSubsequences.end());
    vector<int> ans(k);
    for (int i = 0; i < k; i++)
    {
      ans[i] = allSubsequences[i];
    }
    return ans;
  }

  // 01背包来实现
  // 这种方法此时不是最优解
  // 因为n很大，数值也很大，那么可能的累加和就更大
  // 时间复杂度太差
  vector<int> topKSum2(vector<int>& nums, int k)
  {
    int sum = 0;
    for (int num : nums)
    {
      sum += num;
    }
    // dp[i][j]
    // 1) dp[i-1][j]
    // 2) dp[i-1][j-nums[i]
    vector<int> dp(sum + 1);
    dp[0] = 1;
    for (int num : nums)
    {
      for (int j = sum; j >= num; j--)
      {
        dp[j] += dp[j - num];
      }
    }
    vector<int> ans(k);
    int index = 0;
    for (int j = 0; j <= sum && index < k; j++)
    {
      for (int i = 0; i < dp[j] && index < k; i++)
      {
        ans[index++] = j;
      }
    }
    return ans;
  }

  // 堆方法
  vector<int> topKSum3(vector<int>& nums, int k)
  {
    std::sort(nums.begin(), nums.end());
    // (子序列的最右下标，子序列的累加和)
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> heap;
    heap.push({0, nums[0]});
    vector<int> ans(k);
    for (int i = 1; i < k; i++)
    {
      auto cur = heap.top();
      heap.pop();
      int right = cur.first;
      int sum = cur.second;
      ans[i] = sum;
      if (right + 1 < nums.size())
      {
        heap.push({right + 1, sum - nums[right] + nums[right + 1]});
        heap.push({right + 1, sum + nums[right + 1]});
      }
    }
    return ans;
  }
};
