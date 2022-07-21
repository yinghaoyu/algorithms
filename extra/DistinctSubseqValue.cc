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

  // 假设str="aba"，
  // 思路：初始时集合为空集，每次遍历字符str[i]，新的集合为上一个集合 + 当前集合(上一个集合所有元素+str[i])
  // 1、{空集}
  // 2、{空集}、{a}  --> 第一个a出现的位置，{空集}+a生成的新集合为{a}
  // 3、{空集}、{a}、{b}、{a, b}  --> 第一个b出现的位置，({空集}、{a}) + b生成的新集合为{b}、{a, b}
  // 4、{空集}、{a}、{b}、{a, b}、{a}、{a, a}、{b, a}、{a, b, a}  -->第二个a出现的位置，此时{a}和{a}重复了
  // 此时需要减去{a}，即上次a出现的位置，生成的新集合的数量

  static int zuo(string str)
  {
    if (str.length() == 0)
    {
      return 0;
    }
    // int m = 1000000007;
    unordered_map<char, int> map;
    int all = 1;  // 一个字符也没遍历的时候，有空集
    for (char x : str)
    {
      int newAdd = all;  // 当前新集合的基础是上一个集合所有元素+str[i]，元素的数量是一样的
      // 有相同字符，修正时，需要减去上次相同字符生成的新集合数量
      int curAll = all + newAdd - (map.find(x) != map.end() ? map.at(x) : 0);
      // int curAll = all;
      // curAll = (curAll + newAdd) % m;
      // 减法可能变负数，+m保证最终的值是正数
      // curAll = (curAll - (map.find(x) != map.end() ? map.at(x) : 0) + m) % m;
      all = curAll;     // 记录本次的集合数量
      map[x] = newAdd;  // 记录当前字符新生成的集合数量
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
