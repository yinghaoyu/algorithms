#include <iostream>
#include <string>
#include <vector>

using namespace std;

using TdArray = vector<vector<bool>>;

// 本题测试链接 : https://leetcode.com/problems/interleaving-string/
// 字符串交错组成
// 例如: str1 = 1 a b c   str2 = 1 d e f    str3 = 1 1 a b d c e f
//              ↑                ↑                 ↑
//              p1               p2               p3
// 明显不能用merge方法，因为str3的1你是不知道来自str1还是来自str2，这时指针到底是p1++还是p2++？

class Interleavingstring
{
 public:
  static bool isInterleave(string str1, string str2, string str3)
  {
    if (str3.length() != str1.length() + str2.length())
    {
      return false;
    }
    TdArray dp(str1.length() + 1, vector<bool>(str2.length() + 1));
    // dp[i][j]的含义为，str1取长度i，str2取长度j，能否交错组成长度为i+j的str3
    dp[0][0] = true;  // str1取长度0, str2取长度0, 明显可以组成长度为0的str3
    for (int i = 1; i <= str1.length(); i++)
    {
      // str1取长度i，str2取长度0，能否组成长度为i的str3
      // 长度i，下标为i-1
      if (str1[i - 1] != str3[i - 1])  // 只要有一个字符不相等，后面的dp[i+1...][0]就都是false
      {
        break;
      }
      dp[i][0] = true;  // 字符相等就可以组成
    }
    for (int j = 1; j <= str2.length(); j++)
    {
      // 同理，这次仅取str2
      if (str2[j - 1] != str3[j - 1])
      {
        break;
      }
      dp[0][j] = true;
    }
    for (int i = 1; i <= str1.length(); i++)
    {
      for (int j = 1; j <= str2.length(); j++)
      {
        // 两个条件
        // 1、当前str3末尾i+j(下标为i+j-1)字符来自str1的末尾i(下标为i-1)字符，
        //    并且前一步的长度为i-1的str1和长度为j的str2可以组成长度为i-1+j的str3
        //
        // 2、当前str3末尾i+j(下标为i+j-1)字符来自str2的末尾j(下标为j-1)字符，
        //    并且前一步的长度为i的str1和长度为j-1的str2可以组成长度为i+j-1的str3
        // 那么说明，长度为i的str1和长度为j的str2可以组成长度为i+j的str3
        if ((str1[i - 1] == str3[i + j - 1] && dp[i - 1][j])

            ||

            (str2[j - 1] == str3[i + j - 1] && dp[i][j - 1])

        )
        {
          dp[i][j] = true;
        }
      }
    }
    return dp[str1.length()][str2.length()];
  }
};
