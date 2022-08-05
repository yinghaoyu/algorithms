#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// 给定一个长度len，表示一共有几位
// 所有字符都是小写(a~z)，可以生成长度为1，长度为2，
// 长度为3...长度为len的所有字符串
// 如果把所有字符串根据字典序排序，每个字符串都有所在的位置。
// 给定一个字符串str，给定len，请返回str是总序列中的第几个
// 比如len = 4，字典序的前几个字符串为:
// a aa aaa aaaa aaab ... aaaz ... azzz b ba baa baaa ... bzzz c ...
// a是这个序列中的第1个，bzzz是这个序列中的第36558个
class StringKth
{
  // 思路：
  // cdb，总共长度为7，请问cdb是第几个？
  // 第一位c :
  // 以a开头，剩下长度为(0~6)的所有可能性有几个
  // +
  // 以b开头，剩下长度为(0~6)的所有可能性有几个
  // +
  // 以c开头，剩下长度为(0)的所有可能性有几个，1个
  // 第二位d :
  // +
  // 以ca开头的情况下，剩下长度为(0~5)的所有可能性有几个
  // +
  // 以cb开头的情况下，剩下长度为(0~5)的所有可能性有几个
  // +
  // 以cc开头的情况下，剩下长度为(0~5)的所有可能性有几个
  // +
  // 以cd开头的情况下，剩下长度为(0)的所有可能性有几个，1个
  // 第三位b
  // +
  // 以cda开头的情况下，剩下长度为(0~4)的所有可能性有几个
  // +
  // 以cdb开头的情况下，剩下长度为(0)的所有可能性有几个
  static int kth(string s, int len)
  {
    if (s.length() == 0 || s.length() > len)
    {
      return -1;
    }
    int ans = 0;
    for (int i = 0, rest = len - 1; i < s.length(); i++, rest--)
    {
      ans += (s[i] - 'a') * f(rest) + 1;
    }
    return ans;
  }

  // 不管以什么开头，剩下长度为(0~len)的所有可能性有几个
  static int f(int len)
  {
    int ans = 1;
    for (int i = 1, base = 26; i <= len; i++, base *= 26)
    {
      ans += base;
    }
    return ans;
  }

  // 为了测试
  static vector<string> all(int len)
  {
    vector<string> ans;
    for (int i = 1; i <= len; i++)
    {
      string path(i, ' ');
      process(path, 0, ans);
    }
    return ans;
  }

  // 为了测试
  static void process(string &path, int index, vector<string> &ans)
  {
    if (index == path.length())
    {
      ans.push_back(path);
    }
    else
    {
      for (char c = 'a'; c <= 'z'; c++)
      {
        path[index] = c;
        process(path, index + 1, ans);
      }
    }
  }

  static void main()
  {
    int len = 4;
    // 暴力方法得到所有字符串
    vector<string> ans = all(len);
    // 根据字典序排序，所有字符串都在其中
    std::sort(ans.begin(), ans.end());

    string test = "bzzz";
    // 根据我们的方法算出test是第几个？
    // 注意我们算出的第几个，是从1开始的
    // 而下标是从0开始的，所以变成index，还需要-1
    int index = kth(test, len) - 1;
    // 验证
    cout << ans.at(index) << endl;
  }
};

int main()
{
  return 0;
}
