#include <memory.h>
#include <iostream>
#include <random>
#include <string>

using namespace std;

// 数字转字母
// 规定1和A对应、2和B对应、3和C对应……26和Z对应。
// 那么一个数字字符串比如"111”就可以转化为：“AAA"、 "KA"和"AK"。
// 给定一个只有数字字符组成的字符串str，返回有多少种转化结果。

class ConvertToLetterString
{
 public:
  // for test
  static int getRandom(int min, int max)
  {
    random_device seed;                            // 硬件生成随机数种子
    ranlux48 engine(seed());                       // 利用种子生成随机数引
    uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
    int res = distrib(engine);                     // 随机数
    return res;
  }

  // str只含有数字字符0~9
  // 返回多少种转化方案
  static int number(string str)
  {
    if (str.length() == 0)
    {
      return 0;
    }
    return process(str, 0);
  }

  // str[0..i-1]转化无需过问
  // str[i.....]去转化，返回有多少种转化方法
  static int process(string str, int i)
  {
    // i已经到了最后位置了，说明之前的转换有效，产生了一种转换方法
    if (i == str.length())
    {
      return 1;
    }
    // i没到最后，说明有字符
    if (str[i] == '0')
    {
      // 之前的决定有问题
      return 0;
    }
    // str[i] != '0'
    // 可能性一，i单转
    int ways = process(str, i + 1);
    // i+1存在，且i和i+1组成的数字字符小于27
    if (i + 1 < str.length() && (str[i] - '0') * 10 + str[i + 1] - '0' < 27)
    {
      ways += process(str, i + 2);
    }
    return ways;
  }

  // 从右往左的动态规划
  // 就是上面方法的动态规划版本
  // dp[i]表示：str[i...]有多少种转化方式
  static int dp1(string s)
  {
    if (s.length() == 0)
    {
      return 0;
    }
    // 1.根据暴力递归的可变参数i的取值范围，确定动态规划的数组大小
    int N = s.length();
    vector<int> dp(N + 1);
    // 2.根据暴力递归的 base case 进行边界赋值
    dp[N] = 1;
    // 3.将暴力递归的递归过程转换为动态规划
    for (int i = N - 1; i >= 0; i--)
    {
      if (s[i] != '0')
      {
        int ways = dp[i + 1];
        if (i + 1 < s.length() && (s[i] - '0') * 10 + s[i + 1] - '0' < 27)
        {
          ways += dp[i + 2];
        }
        dp[i] = ways;
      }
    }
    // 4.根据暴力递归主函数的调用，确定动态规划的返回值
    return dp[0];
  }

  // 从左往右的动态规划
  // dp[i]表示：str[0...i]有多少种转化方式
  static int dp2(string s)
  {
    if (s.length() == 0)
    {
      return 0;
    }
    int N = s.length();
    if (s[0] == '0')
    {
      return 0;
    }
    vector<int> dp(N);
    dp[0] = 1;
    for (int i = 1; i < N; i++)
    {
      if (s[i] == '0')
      {
        // 如果此时str[i]=='0'，那么他是一定要拉前一个字符(i-1的字符)一起拼的，
        // 那么就要求前一个字符，不能也是‘0’，否则拼不了。
        // 前一个字符不是‘0’就够了嘛？不够，还得要求拼完了要么是10，要么是20，如果更大的话，拼不>  了。
        // 这就够了嘛？还不够，你们拼完了，还得要求str[0...i-2]真的可以被分解！
        // 如果str[0...i-2]都不存在分解方案，那i和i-1拼成了也不行，因为之前的搞定不了。
        if (s[i - 1] == '0' || s[i - 1] > '2' || (i - 2 >= 0 && dp[i - 2] == 0))
        {
          return 0;
        }
        else
        {
          dp[i] = i - 2 >= 0 ? dp[i - 2] : 1;
        }
      }
      else
      {
        dp[i] = dp[i - 1];
        if (s[i - 1] != '0' && (s[i - 1] - '0') * 10 + s[i] - '0' <= 26)
        {
          dp[i] += i - 2 >= 0 ? dp[i - 2] : 1;
        }
      }
    }
    return dp[N - 1];
  }

  // 为了测试
  static string randomString(int len)
  {
    string str;
    for (int i = 0; i < len; i++)
    {
      str.push_back(getRandom(0, 9) + '0');
    }
    return str;
  }

  // 为了测试
  static void test()
  {
    int N = 30;
    int testTime = 10000;
    cout << "测试开始" << endl;
    for (int i = 0; i < testTime; i++)
    {
      int len = getRandom(0, N - 1);
      string s = randomString(len);
      int ans0 = number(s);
      int ans1 = dp1(s);
      int ans2 = dp2(s);
      if (ans0 != ans1 || ans0 != ans2)
      {
        cout << s << endl;
        cout << ans0 << endl;
        cout << ans1 << endl;
        cout << ans2 << endl;
        cout << "Oops!" << endl;
        break;
      }
    }
    cout << "测试结束" << endl;
  }
};

int main()
{
  ConvertToLetterString::test();
  return 0;
}
