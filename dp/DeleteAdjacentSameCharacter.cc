#include <cstdint>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

using TtArray = vector<vector<vector<int>>>;

// 如果一个字符相邻的位置没有相同字符，那么这个位置的字符出现不能被消掉
// 比如:"ab"，其中a和b都不能被消掉
// 如果一个字符相邻的位置有相同字符，就可以一起消掉
// 比如:"abbbc"，中间一串的b是可以被消掉的，消除之后剩下"ac"
// 某些字符如果消掉了，剩下的字符认为重新靠在一起
// 给定一个字符串，你可以决定每一步消除的顺序，目标是请尽可能多的消掉字符，返回最少的剩余字符数量
// 比如："aacca", 如果先消掉最左侧的"aa"，那么将剩下"cca"，然后把"cc"消掉，剩下的"a"将无法再消除>，返回1
// 但是如果先消掉中间的"cc"，那么将剩下"aaa"，最后都消掉就一个字符也不剩了，返回0，这才是最优解。
// 再比如："baaccabb"，
// 如果先消除最左侧的两个a，剩下"bccabb"，
// 如果再消除最左侧的两个c，剩下"babb"，
// 最后消除最右侧的两个b，剩下"ba"无法再消除，返回2
// 而最优策略是：
// 如果先消除中间的两个c，剩下"baaabb"，
// 如果再消除中间的三个a，剩下"bbb"，
// 最后消除三个b，不留下任何字符，返回0，这才是最优解

class DeleteAdjacentSameCharacter
{
 public:
  // 暴力解
  static int restMin1(string s)
  {
    if (s.length() < 2)
    {
      return s.length();
    }
    int minLen = s.length();
    for (int L = 0; L < s.length(); L++)
    {
      for (int R = L + 1; R < s.length(); R++)
      {
        // [L, R + 1)之间的字符串
        if (canDelete(s.substr(L, R + 1 - L)))
        {
          // 排除掉[L, R + 1)之间的字符串
          // 继续计算[0, L)和[R+1, len)之间的字符串
          minLen = std::min(minLen, restMin1(s.substr(0, L) + s.substr(R + 1, s.length() - (R + 1))));
        }
      }
    }
    return minLen;
  }

  // 判断字符串所有字符是否一样
  static bool canDelete(string str)
  {
    for (int i = 1; i < str.length(); i++)
    {
      if (str[i - 1] != str[i])
      {
        return false;
      }
    }
    return true;
  }

  // 优良尝试的暴力递归版本
  static int restMin2(string s)
  {
    if (s.length() < 2)
    {
      return s.length();
    }
    return process(s, 0, s.length() - 1, false);
  }

  // str[L...R] 前面有没有跟着[L]字符，has T 有 F 无
  // L,R,has
  // 最少能剩多少字符，消不了
  static int process(string str, int L, int R, bool has)
  {
    if (L > R)
    {
      return 0;
    }
    if (L == R)
    {
      // 只剩一个字符，前面字符跟[L]相同，说明这个字符也可以删除
      return has ? 0 : 1;
    }
    int index = L;
    int K = has ? 1 : 0;  // 如果前面字符跟[L]相同，[L]字符数从1开始计数
    while (index <= R && str[index] == str[L])
    {
      K++;  // [L...R]间有多少个[L]字符
      index++;
    }
    // 方案一：
    // index表示，第一个不是[L]字符的位置
    // K > 1表示[L]字符数 > 1，可以删除，那么剩余的字符当然为0了
    int way1 = (K > 1 ? 0 : 1) + process(str, index, R, false);
    int way2 = INT32_MAX;
    for (int split = index; split <= R; split++)
    {
      // 方案二：
      // [index...R]之前存在[split]和[L]字符相同，
      // 并且[split]和[split - 1]不相同，按照方案1的话[split]字符可能删不掉
      // 那么先尝试把[index, split - 1]删除
      if (str[split] == str[L] && str[split] != str[split - 1])
      {
        if (process(str, index, split - 1, false) == 0)
        {
          // 如果能删除，说明[split]字符可以纳入[L]，然后删除掉
          // 只需要计算[split, R]的结果
          way2 = std::min(way2, process(str, split, R, K != 0));
        }
      }
    }
    return std::min(way1, way2);
  }

  // 优良尝试的动态规划版本
  static int restMin3(string str)
  {
    if (str.length() < 2)
    {
      return str.length();
    }
    int N = str.length();
    TtArray dp(N, vector<vector<int>>(N, vector<int>(2)));
    for (int i = 0; i < N; i++)
    {
      for (int j = 0; j < N; j++)
      {
        for (int k = 0; k < 2; k++)
        {
          dp[i][j][k] = -1;
        }
      }
    }
    return dpProcess(str, 0, N - 1, false, dp);
  }

  static int dpProcess(string str, int L, int R, bool has, TtArray &dp)
  {
    if (L > R)
    {
      return 0;
    }
    int K = has ? 1 : 0;
    if (dp[L][R][K] != -1)
    {
      return dp[L][R][K];
    }
    int ans = 0;
    if (L == R)
    {
      ans = (K == 0 ? 1 : 0);
    }
    else
    {
      int index = L;
      int all = K;
      while (index <= R && str[index] == str[L])
      {
        all++;
        index++;
      }
      int way1 = (all > 1 ? 0 : 1) + dpProcess(str, index, R, false, dp);
      int way2 = INT32_MAX;
      for (int split = index; split <= R; split++)
      {
        if (str[split] == str[L] && str[split] != str[split - 1])
        {
          if (dpProcess(str, index, split - 1, false, dp) == 0)
          {
            way2 = std::min(way2, dpProcess(str, split, R, all > 0, dp));
          }
        }
      }
      ans = std::min(way1, way2);
    }
    dp[L][R][K] = ans;
    return ans;
  }

  static string randomstring(int len, int variety)
  {
    string str(len, ' ');
    for (int i = 0; i < len; i++)
    {
      str[i] = getRandom(0, variety) + 'a';
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
    int maxLen = 16;
    int variety = 3;
    int testTime = 100000;
    cout << "测试开始" << endl;
    for (int i = 0; i < testTime; i++)
    {
      int len = getRandom(0, maxLen);
      string str = randomstring(len, variety);
      int ans1 = restMin1(str);
      int ans2 = restMin2(str);
      int ans3 = restMin3(str);
      if (ans1 != ans2 || ans1 != ans3)
      {
        cout << str << endl;
        cout << ans1 << endl;
        cout << ans2 << endl;
        cout << ans3 << endl;
        cout << "出错了！" << endl;
        break;
      }
    }
    cout << "测试结束" << endl;
  }
};

int main()
{
  DeleteAdjacentSameCharacter::test();
  return 0;
}
