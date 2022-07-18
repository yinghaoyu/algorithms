#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// 问题描述：
// 给你一个由若干括号和字母组成的字符串 s ，删除最小数量的无效括号，使得输入的字符串有效。
// 返回所有可能的结果。答案可以按 任意顺序 返回。

// 测试链接 : https://leetcode.com/problems/remove-invalid-parentheses/

class RemoveInvalidParentheses
{
 public:
  static vector<string> removeInvalidParentheses(string s)
  {
    vector<string> ans;
    string op = "()";
    remove(s, ans, 0, 0, op);
    return ans;
  }

  // modifyIndex <= checkIndex
  // 只查s[checkIndex....]的部分，因为之前的一定已经调整对了
  // 但是之前的部分是怎么调整对的，调整到了哪？就是modifyIndex
  // 比如：
  // ( ( ) ( ) ) ) ...
  // 0 1 2 3 4 5 6
  // 一开始当然checkIndex = 0，modifyIndex = 0
  // 当查到6的时候，发现不对了，
  // 然后可以去掉2位置、4位置的 )，都可以，后面的字符往前移动
  // 如果去掉2位置的 ), 那么下一步就是
  // ( ( ( ) ) ) ...
  // 0 1 2 3 4 5 6
  // checkIndex = 6 ，modifyIndex = 2
  // 如果去掉4位置的 ), 那么下一步就是
  // ( ( ) ( ) ) ...
  // 0 1 2 3 4 5 6
  // checkIndex = 6 ，modifyIndex = 4
  // 也就是说，
  // checkIndex和modifyIndex，分别表示查的开始 和 调的开始，之前的都不用管了  par  (  )
  static void remove(string s, vector<string> &ans, int checkIndex, int deleteIndex, string par)
  {
    for (int count = 0, i = checkIndex; i < s.length(); i++)
    {
      if (s[i] == par[0])
      {
        count++;
      }
      if (s[i] == par[1])
      {
        count--;
      }
      // i check计数<0的第一个位置，即违规位置
      if (count < 0)
      {
        // 从第一个能删的位置开始检查能不能删除，不能超过检查的位置
        for (int j = deleteIndex; j <= i; ++j)
        {
          // s[j]是)，并且j==deleteIndex
          // s[j]是)，并且s[j-1]不是)，这个条件是为了减支
          // 因为s[j-1]和s[j]都是)的话，那么这两个任意删除一个得到的结果一样
          if (s[j] == par[1] && (j == deleteIndex || s[j - 1] != par[1]))
          {
            remove(s.substr(0, j) + s.substr(j + 1, s.length() - j - 1), ans, i, j, par);
          }
        }
        return;
      }
    }
    // 到这里，说明(的数量 >= )的数量，比如((((()，或者()
    string reversed = s;
    std::reverse(reversed.begin(), reversed.end());
    if (par[0] == '(')  // 如果第一个字符是(，说明没经历过reverse过程
    {
      remove(reversed, ans, 0, 0, string(")("));
    }
    else  // 正序，逆序都到这步就可以收集答案了
    {
      ans.push_back(reversed);
    }
  }
};

int main()
{
  string str = "((((((()";
  vector<string> res = RemoveInvalidParentheses::removeInvalidParentheses(str);
  for (string s : res)
  {
    cout << s << endl;
  }
  return 0;
}
