#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

// 返回无序数组中正数和负数个数相等的最长子数组长度
// 给定一个无序数组arr，其中元素可正、可负、可0
// 求arr所有子数组中正数与负数个数相等的最长子数组的长度
// 测试链接 : https://www.nowcoder.com/practice/545544c060804eceaed0bb84fcd992fb
class PositivesEqualsNegtivesLongestSubarray
{
 public:
  int compute(vector<int>& arr)
  {
    unordered_map<int, int> map;
    map[0] = -1;
    int ans = 0;
    for (int i = 0, sum = 0; i < arr.size(); i++)
    {
      sum += arr[i];
      if (map.count(sum))
      {
        // 说明这个区间的和为 0
        ans = std::max(ans, i - map[sum]);
      }
      else
      {
        map[sum] = i;
      }
    }
    return ans;
  }
};
