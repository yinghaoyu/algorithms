#include <map>
#include <vector>

using namespace std;

// 返回无序数组中累加和为给定值的最长子数组长度
// 给定一个无序数组arr, 其中元素可正、可负、可0
// 给定一个整数aim
// 求arr所有子数组中累加和为aim的最长子数组长度
// 测试链接 : https://www.nowcoder.com/practice/36fb0fd3c656480c92b569258a1223d5

class LongestSubarraySumEqualsAim
{
 public:
  int compute(vector<int>& arr, int aim)
  {
    map<int, int> m;
    // 0这个前缀和，一个数字也没有的时候，就存在了
    m[0] = -1;
    int ans = 0;
    for (int i = 0, sum = 0; i < arr.size(); i++)
    {
      sum += arr[i];
      if (m.count(sum - aim))
      {
        ans = std::max(ans, i - m[sum - aim]);
      }
      if (!m.count(sum))
      {
        m[sum] = i;
      }
    }
    return ans;
  }
};
