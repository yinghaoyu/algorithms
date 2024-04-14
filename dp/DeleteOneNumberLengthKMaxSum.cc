#include <cstdint>
#include <vector>

using namespace std;

// 删掉1个数字后长度为k的子数组最大累加和
// 给定一个数组nums，求必须删除一个数字后的新数组中
// 长度为k的子数组最大累加和，删除哪个数字随意
// 对数器验证
class DeleteOneNumberLengthKMaxSum
{
 public:
  // 正式方法
  // 时间复杂度O(N)
  static int maxSum2(vector<int>& nums, int k)
  {
    int n = nums.size();
    if (n <= k)
    {
      return 0;
    }
    // 单调队列 : 维持窗口内最小值的更新结构，讲解054的内容
    vector<int> window(n);
    int l = 0;
    int r = 0;
    // 窗口累加和
    long sum = 0;
    int ans = INT32_MIN;
    for (int i = 0; i < n; i++)
    {
      // 单调队列 : i位置进入单调队列
      while (l < r && nums[window[r - 1]] >= nums[i])
      {
        r--;
      }
      window[r++] = i;
      sum += nums[i];
      if (i >= k)
      {
        ans = std::max(ans, (int) (sum - nums[window[l]]));
        if (window[l] == i - k)
        {
          // 单调队列 : 如果单调队列最左侧的位置过期了，从队列中弹出
          l++;
        }
        sum -= nums[i - k];
      }
    }
    return ans;
  }
};
