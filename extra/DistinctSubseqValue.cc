#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

// 问题描述：
// 给定一个字符串 s，计算 s 的 不同非空子序列 的个数。
// 因为结果可能很大，所以返回答案需要对 10^9 + 7 取余。
// 字符串的 子序列 是经由原字符串删除一些（也可能不删除）字符但不改变剩余字符相对位置的一个新字符串。

// 输入：s = "abc"
// 输出：7
// 解释：7 个不同的子序列分别是 "a", "b", "c", "ab", "ac", "bc", 以及 "abc"。

// 测试链接 : https://leetcode.com/problems/distinct-subsequences-ii/
class DistinctSubseqValue
{
 public:
  static int distinctSubseqII(string str)
  {
    if (str.length() == 0)
    {
      return 0;
    }
    int m = 1000000007;
    vector<int> count(26);
    int all = 1;  // 算空集
    for (char x : str)
    {
      int add = (all - count[x - 'a'] + m) % m;
      all = (all + add) % m;
      count[x - 'a'] = (count[x - 'a'] + add) % m;
    }
    return all - 1;
  }

  static int zuo(string str)
  {
    if (str.length() == 0)
    {
      return 0;
    }
    int m = 1000000007;
    unordered_map<char, int> map;
    int all = 1;  // 一个字符也没遍历的时候，有空集
    for (char x : str)
    {
      int newAdd = all;
      //			int curAll = all + newAdd - (map.containsKey(x) ? map.get(x) : 0);
      int curAll = all;
      curAll = (curAll + newAdd) % m;
      curAll = (curAll - (map.find(x) != map.end() ? map.at(x) : 0) + m) % m;
      all = curAll;
      map[x] = newAdd;
    }
    return all;
  }

  static void test()
  {
    string s = "bccaccbaabbc";
    cout << distinctSubseqII(s) + 1 << endl;
    cout << zuo(s) << endl;
  }
};

int main()
{
  DistinctSubseqValue::test();
  return 0;
}
