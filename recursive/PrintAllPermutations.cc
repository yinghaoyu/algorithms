#include <stdbool.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class PrintAllPermutations
{
 public:
  static vector<string> permutation1(string s)
  {
    vector<string> ans;
    if (s == "" || s.length() == 0)
    {
      return ans;
    }
    vector<char> rest;
    for (char cha : s)
    {
      rest.push_back(cha);
    }
    string path = "";
    f(rest, path, ans);
    return ans;
  }

  static void f(vector<char> rest, string path, vector<string> &ans)
  {
    if (rest.empty())
    {
      ans.push_back(path);
    }
    else
    {
      int N = rest.size();
      for (int i = 0; i < N; i++)  // 每个位置尝试
      {
        char cur = rest.at(i);
        rest.erase(rest.begin() + i);  // 尝试添加i位置字符
        f(rest, path + cur, ans);
        rest.insert(rest.begin() + i, cur);  // 恢复i位置字符
      }
    }
  }

  static vector<string> permutation2(string s)
  {
    vector<string> ans;
    if (s == "" || s.length() == 0)
    {
      return ans;
    }
    g1(s, 0, ans);
    return ans;
  }

  static void g1(string str, int index, vector<string> &ans)
  {
    if (index == str.length())
    {
      ans.push_back(str);
    }
    else
    {
      for (int i = index; i < str.length(); i++)
      {
        swap(str, index, i);  // 交换选取i，减少空间
        g1(str, index + 1, ans);
        swap(str, index, i);  // 交换恢复i
      }
    }
  }

  static vector<string> permutation3(string s)
  {
    vector<string> ans;
    if (s == "" || s.length() == 0)
    {
      return ans;
    }
    g2(s, 0, ans);
    return ans;
  }

  static void g2(string str, int index, vector<string> &ans)
  {
    if (index == str.length())
    {
      ans.push_back(str);
    }
    else
    {
      bool *visited = new bool[256];  // 利用ASCII码去重
      for (int i = index; i < str.length(); i++)
      {
        if (!visited[str[i]])
        {
          visited[str[i]] = true;
          swap(str, index, i);
          g2(str, index + 1, ans);
          swap(str, index, i);
        }
      }
    }
  }

  static void swap(string &chs, int i, int j)
  {
    char tmp = chs[i];
    chs[i] = chs[j];
    chs[j] = tmp;
  }
};

int main()
{
  string s = "acc";
  vector<string> ans1 = PrintAllPermutations::permutation1(s);
  for (string str : ans1)
  {
    cout << str << endl;
  }
  cout << "=======" << endl;
  vector<string> ans2 = PrintAllPermutations::permutation2(s);
  for (string str : ans2)
  {
    cout << str << endl;
  }
  cout << "=======" << endl;
  vector<string> ans3 = PrintAllPermutations::permutation3(s);
  for (string str : ans3)
  {
    cout << str << endl;
  }
  return 0;
}
