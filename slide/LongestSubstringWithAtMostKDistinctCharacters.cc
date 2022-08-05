#include <iostream>
#include <string>
#include <vector>

using namespace std;

// 问题描述：
// 找出str中包含k个字符的最长子串的长度
// 例如：
// str = aabccd, k = 3
// 返回 5，因为aabcc最长
//
// 分析：
// 因为str增加后，k要么增加，要么不变，包含k个字符的最长子串的长度也是要么增加要么不变
// 符合单调性，第一时间就应该想到滑动窗口

// 测试链接：https://leetcode.cn/problems/longest-substring-with-at-most-k-distinct-characters/

class Problem_0340_LongestSubstringWithAtMostKDistinctCharacters
{
 public:
  static int lengthOfLongestSubstringKDistinct(string str, int k)
  {
    if (str.length() == 0 || k < 1)
    {
      return 0;
    }
    int N = str.length();
    vector<int> count(256);
    int diff = 0;  // 窗口包含的不同字符数
    int R = 0;
    int ans = 0;
    for (int i = 0; i < N; i++)  // i为窗口的左边界
    {
      // R 窗口的右边界
      while (R < N && (diff < k || (diff == k && count[str[R]] > 0)))
      {
        diff += count[str[R]] == 0 ? 1 : 0;
        count[str[R++]]++;  // 字符频率++，R右移
      }
      // R 来到违规的第一个位置
      ans = std::max(ans, R - i);
      diff -= count[str[i]] == 1 ? 1 : 0;
      count[str[i]]--;
    }
    return ans;
  }
};

int main()
{
  return 0;
}
