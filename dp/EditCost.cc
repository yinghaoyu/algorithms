#include <iostream>
#include <vector>

using namespace std;
using TdArray = vector<vector<int>>;

// 问题描述：
// 给你两个单词 word1 和 word2， 请返回将 word1 转换成 word2 所使用的最少代价。
// 你可以对一个单词进行如下三种操作：
// 1、插入一个字符，代价ic
// 2、删除一个字符，代价dc
// 3、替换一个字符，代价rc
// 4、保持不变，代价为0

// 测试链接：https://leetcode.cn/problems/edit-distance

class EditCost
{
 public:
  static int minCost1(string str1, string str2, int ic, int dc, int rc)
  {
    int N = str1.length() + 1;
    int M = str2.length() + 1;
    TdArray dp(N, vector<int>(M));
    // dp[i][j]的含义为取前缀长度i的str1，转换成前缀长度为j的str2，最小需要的代价
    // dp[0][0] = 0
    for (int i = 1; i < N; i++)
    {
      // 长度为i的str1，转成长度为0的str2，只需要删除i个字符，最小代价为dc * i
      dp[i][0] = dc * i;
    }
    for (int j = 1; j < M; j++)
    {
      // 长度为0的str1，转成长度为i的str2，只需要添加i个字符，最小代价为ic * i
      dp[0][j] = ic * j;
    }
    for (int i = 1; i < N; i++)
    {
      for (int j = 1; j < M; j++)
      {
        // 可能性一：str1第i个字符和str2的第j个字符相等，长度为i-1的str1，转成长度为j-1的str2的最小代价，然后对第i个字符保持不变
        // 可能性二：str1第i个字符和str2的第j个字符不相等，长度为i-1的str1，转成长度为j-1的str2的最小代价，然后对第i个字符替换
        // 可能性三：长度为i的str1，转成长度为j-1的str2，此时只需要插入一个字符，让str2长度由j-1变成j
        // 可能性四：长度为i-1的str1，就可以转成长度为j的str2，那么长度为i的str1多出来一个字符，删除就可以了
        dp[i][j] = dp[i - 1][j - 1] + (str1[i - 1] == str2[j - 1] ? 0 : rc);
        dp[i][j] = std::min(dp[i][j], dp[i][j - 1] + ic);
        dp[i][j] = std::min(dp[i][j], dp[i - 1][j] + dc);
      }
    }
    return dp[N - 1][M - 1];
  }

  static int minCost2(string str1, string str2, int ic, int dc, int rc)
  {
    string longs = str1.length() >= str2.length() ? str1 : str2;
    string shorts = str1.length() < str2.length() ? str1 : str2;
    if (str1.length() < str2.length())
    {
      int tmp = ic;
      ic = dc;
      dc = tmp;
    }
    vector<int> dp(shorts.length() + 1);
    for (int i = 1; i <= shorts.length(); i++)
    {
      dp[i] = ic * i;
    }
    for (int i = 1; i <= longs.length(); i++)
    {
      int pre = dp[0];
      dp[0] = dc * i;
      for (int j = 1; j <= shorts.length(); j++)
      {
        int tmp = dp[j];
        if (longs[i - 1] == shorts[j - 1])
        {
          dp[j] = pre;
        }
        else
        {
          dp[j] = pre + rc;
        }
        dp[j] = std::min(dp[j], dp[j - 1] + ic);
        dp[j] = std::min(dp[j], tmp + dc);
        pre = tmp;
      }
    }
    return dp[shorts.length()];
  }

  static void test()
  {
    string str1 = "ab12cd3";
    string str2 = "abcdf";
    cout << minCost1(str1, str2, 5, 3, 2) << endl;
    cout << minCost2(str1, str2, 5, 3, 2) << endl;

    str1 = "abcdf";
    str2 = "ab12cd3";
    cout << minCost1(str1, str2, 3, 2, 4) << endl;
    cout << minCost2(str1, str2, 3, 2, 4) << endl;

    str1 = "";
    str2 = "ab12cd3";
    cout << minCost1(str1, str2, 1, 7, 5) << endl;
    cout << minCost2(str1, str2, 1, 7, 5) << endl;

    str1 = "abcdf";
    str2 = "";
    cout << minCost1(str1, str2, 2, 9, 8) << endl;
    cout << minCost2(str1, str2, 2, 9, 8) << endl;
  }
};

int main()
{
  EditCost::test();
  return 0;
}
