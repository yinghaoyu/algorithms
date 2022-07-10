#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

// 问题描述：
// 给定一个只由小写字母（a~z）组成的字符串str， 返回其中最长无重复字符的子串长度

class LongestNoRepeatSubstring
{
 public:
  // 暴力解O(N^2)
  static int lnrs1(string str)
  {
    if (str.length() == 0)
    {
      return 0;
    }
    int N = str.length();
    int max = 0;
    for (int i = 0; i < N; i++)
    {
      vector<bool> set(26);  // 当前字符是否出现
      for (int j = i; j < N; j++)
      {
        if (set[str[j] - 'a'])  // 重复出现，就结束统计
        {
          break;
        }
        set[str[j] - 'a'] = true;        // 标记已出现
        max = std::max(max, j - i + 1);  // 计算最大长度
      }
    }
    return max;
  }

  // dp[i]表示以i位置结尾的字符串，最长无重复子串的长度
  // last[i]表示i位置的字符，上一次出现的位置
  // 情况一：
  // 当前字符i上次出现的位置距离更近
  // * * b * * * * * * a * * * * b  a
  //       ↑           ↑         ↑  ↑
  //    i-dp[i-1]    last[i]    i-1 i

  // 情况二：
  // i-1位置的最长无重复子串更短           --> 两种情况结合，此时dp[i]的值,
  // * * * a * * * * b * * * * * b  a         应该是i - last[i]与dp[i-1] + 1之间的最小值
  //       ↑           ↑         ↑  ↑
  //     last[i]    i-dp[i-1]   i-1 i

  // 动态规划，时间复杂度O(N)
  static int lnrs2(string str)
  {
    if (str.length() == 0)
    {
      return 0;
    }
    int N = str.length();
    // last[0] a上次出现的位置
    // last[1] b上次出现的位置
    // ...
    // last[25] z上次出现的位置
    vector<int> last(26);  // 记录字符上次出现的位置
    for (int i = 0; i < 26; i++)
    {
      last[i] = -1;  // 开始默认-1
    }
    last[str[0] - 'a'] = 0;  // 第一个字符出现的位置显然为0
    int max = 1;
    // 由于dp[i]只依赖dp[i-1]，这里为了节省空间，优化为单个变量
    int preMaxLen = 1;  // dp[i-1]的值
    for (int i = 1; i < N; i++)
    {
      // i字符上次出现的距离，与dp[i-1] + 1 比较，+ 1目的是加上当前字符长度1
      preMaxLen = std::min(i - last[str[i] - 'a'], preMaxLen + 1);
      max = std::max(max, preMaxLen);
      last[str[i] - 'a'] = i;  // 更新i位置字符上次出现的位置
    }
    return max;
  }

  // for test
  static string getRandomstring(int possibilities, int maxSize)
  {
    string ans(getRandom(1, maxSize), ' ');
    for (int i = 0; i < ans.length(); i++)
    {
      ans[i] = getRandom(0, possibilities - 1) + 'a';
    }
    return ans;
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
    int possibilities = 26;
    int strMaxSize = 100;
    int testTimes = 1000000;
    cout << "test begin, test time : " << testTimes << endl;
    for (int i = 0; i < testTimes; i++)
    {
      string str = getRandomstring(possibilities, strMaxSize);
      int ans1 = lnrs1(str);
      int ans2 = lnrs2(str);
      if (ans1 != ans2)
      {
        cout << "Oops!" << endl;
      }
    }
    cout << "test finish" << endl;
  }
};

int main()
{
  LongestNoRepeatSubstring::test();
  return 0;
}
