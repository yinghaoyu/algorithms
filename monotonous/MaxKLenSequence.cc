#include <iostream>
#include <random>
#include <set>
#include <vector>

using namespace std;

// 问题描述：
// 给定一个字符串str，和一个正数k
// 返回长度为k的所有子序列中，字典序最大的子序列

// 分析：
// 单调递减栈，头部ASCII码越大，字典序越靠后
// 如果剩余未进栈的字符和栈内字符数量==k，就不用出栈了
class MaxKLenSequence
{
 public:
  static string maxString(string str, int k)
  {
    if (k <= 0 || str.length() < k)
    {
      return "";
    }
    int n = str.length();
    vector<char> stack(n, ' ');
    int size = 0;
    for (int i = 0; i < n; i++)
    {
      // 单调递减栈
      // 栈内有字符
      // 剩余字符和栈内字符数量和与k相等，就不用出栈了
      while (size > 0 && stack[size - 1] < str[i] && size + n - i > k)
      {
        size--;
      }
      // 这时出栈的话，字符长度就不满足k了
      if (size + n - i == k)
      {
        return string(stack.begin(), stack.begin() + size) + str.substr(i);
      }
      stack[size++] = str[i];
    }
    // 如果栈内字符 > k个，取前k个即可，因为是单调递减栈
    return string(stack.begin(), stack.begin() + k);
  }

  // 为了测试
  static string test(string str, int k)
  {
    if (k <= 0 || str.length() < k)
    {
      return "";
    }
    set<string> ans;
    string path(k, ' ');
    process(0, 0, str, path, ans);
    return *ans.rbegin();
  }

  // 为了测试
  // 枚举所有子序列
  // 从str[si]...位置开始选字符，填到path[pi]的位置
  // 直到si越界，如果pi也刚好越界，就将path加入答案中
  static void process(int si, int pi, string &str, string &path, set<string> &ans)
  {
    if (si == str.length())
    {
      if (pi == path.length())
      {
        ans.emplace(path);
      }
    }
    else
    {
      process(si + 1, pi, str, path, ans);
      if (pi < path.length())
      {
        path[pi] = str[si];
        process(si + 1, pi + 1, str, path, ans);
      }
    }
  }

  // 为了测试
  static string randomString(int len, int range)
  {
    string str(len, ' ');
    for (int i = 0; i < len; i++)
    {
      str[i] = getRandom(1, range) + 'a';
    }
    return str;
  }

  static int getRandom(int min, int max)
  {
    random_device seed;                            // 硬件生成随机数种子
    ranlux48 engine(seed());                       // 利用种子生成随机数引
    uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
    int res = distrib(engine);                     // 随机数
    return res;
  }

  static void test()
  {
    int n = 12;
    int r = 5;
    int testTime = 10000;
    cout << "测试开始" << endl;
    for (int i = 0; i < testTime; i++)
    {
      int len = getRandom(1, n);
      string str = randomString(len, r);
      int k = getRandom(1, str.length() - 1);
      string ans1 = maxString(str, k);
      string ans2 = test(str, k);
      // cout << str << endl;
      // cout << ans1 << endl;
      // cout << ans2 << endl;
      if (ans1 != ans2)
      {
        cout << "出错了！" << endl;
        cout << str << endl;
        cout << k << endl;
        cout << ans1 << endl;
        cout << ans2 << endl;
        break;
      }
    }
    cout << "测试结束" << endl;
  }
};

int main()
{
  MaxKLenSequence::test();
  return 0;
}
