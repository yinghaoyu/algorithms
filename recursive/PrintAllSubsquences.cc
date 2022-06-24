#include <stdbool.h>
#include <iostream>
#include <list>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

class PrintAllSubsquences
{
 public:
  // s -> "abc" ->
  static list<string> subs(string s)
  {
    string path = "";
    list<string> ans;
    process1(s, 0, ans, path);
    return ans;
  }

  // str 固定参数
  // 来到了str[index]字符，index是位置
  // str[0..index-1]已经走过了！之前的决定，都在path上
  // 之前的决定已经不能改变了，就是path
  // str[index....]还能决定，之前已经确定，而后面还能自由选择的话，
  // 把所有生成的子序列，放入到ans里去
  static void process1(const string &str, int index, list<string> &ans, string path)
  {
    if (index == str.length())
    {
      ans.push_back(path);
      return;
    }
    // 没有要index位置的字符
    process1(str, index + 1, ans, path);
    // 要了index位置的字符
    process1(str, index + 1, ans, path + str[index]);
  }

  static list<string> subsNoRepeat(string s)
  {
    string path = "";
    unordered_set<string> set;  // mapreduce
    process2(s, 0, set, path);
    list<string> ans;
    for (string cur : set)
    {
      ans.push_back(cur);
    }
    return ans;
  }

  static void process2(const string &str, int index, unordered_set<string> &set, string path)
  {
    if (index == str.length())
    {
      set.emplace(path);
      return;
    }
    string no = path;
    process2(str, index + 1, set, no);
    string yes = path + str[index];
    process2(str, index + 1, set, yes);
  }
};

int main()
{
  string test = "acccc";
  list<string> ans1 = PrintAllSubsquences::subs(test);
  list<string> ans2 = PrintAllSubsquences::subsNoRepeat(test);

  for (string str : ans1)
  {
    cout << str << endl;
  }
  cout << "=================" << endl;
  for (string str : ans2)
  {
    cout << str << endl;
  }
  cout << "=================" << endl;
  return 0;
}
