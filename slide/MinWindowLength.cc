#include <iostream>
#include <string>
#include <vector>

using namespace std;

// 问题描述：
// 最小覆盖子串
// 给你一个字符串 s 、一个字符串 t 。
// 返回 s 中涵盖 t 所有字符的最小子串。
// 如果 s 中不存在涵盖 t 所有字符的子串，则返回空字符串 "" 。
//
// 例如:
// 输入：s = "ADOBECODEBANC", t = "ABC"
// 输出："BANC"
//
// 测试链接：https://leetcode.cn/problems/minimum-window-substring/

class MinWindowLength
{
 public:
  // 滑动窗口解法，必须观察是否满足递增序
  static int minLength(string str1, string str2)
  {
    if (str1.length() < str2.length())
    {
      return INT32_MAX;
    }
    // ASCII 总共0~255
    vector<int> map(256);  // map[37] = 4 37 4次
    for (int i = 0; i != str2.length(); i++)
    {
      // 先统计字符频率
      map[str2[i]]++;
    }
    int all = str2.length();  // 有效的字符数

    // [L,R-1] R
    // [L,R) -> [0,0)
    int L = 0;
    int R = 0;
    int minLen = INT32_MAX;
    while (R != str1.length())  // 先右边界往左移
    {
      map[str1[R]]--;  // 每次遍历都使得字符频率-1
      if (map[str1[R]] >= 0)
      {
        // 这个字符的频率-1后，频率还>=0，说明这次的字符对目标串的贡献是有效的
        all--;
      }
      if (all == 0)  // 刚好匹配完，就结算本次结果，然后左边界往右移
      {
        // 还完了，L往右移，尝试能否缩短窗口
        while (map[str1[L]] < 0)
        {
          map[str1[L++]]++;
        }
        // [L..R]
        minLen = std::min(minLen, R - L + 1);
        all++;
        map[str1[L++]]++;
      }
      R++;
    }
    return minLen == INT32_MAX ? 0 : minLen;
  }

  static string minWindow(string s, string t)
  {
    if (s.length() < t.length())
    {
      return "";
    }
    vector<int> map(256);
    for (char cha : t)
    {
      map[cha]++;
    }
    int all = t.length();
    int L = 0;
    int R = 0;
    int minLen = INT32_MAX;
    int ansl = -1;
    int ansr = -1;
    while (R != s.length())
    {
      map[s[R]]--;
      if (map[s[R]] >= 0)
      {
        all--;
      }
      if (all == 0)
      {
        while (map[s[L]] < 0)
        {
          map[s[L++]]++;
        }
        if (minLen > R - L + 1)
        {
          minLen = R - L + 1;
          ansl = L;
          ansr = R;
        }
        all++;
        map[s[L++]]++;
      }
      R++;
    }
    return minLen == INT32_MAX ? "" : s.substr(ansl, ansr + 1 - ansl);
  }
};

int main()
{
  return 0;
}
