#include <iostream>
#include <random>
#include <string>

using namespace std;

// Manacher求最长回文子串半径
// 子序列和子串的定义：
// 子序列：删除若干元素后得到的序列；
// 子串：字符串中任意个连续的字符组成的子序列。
// 可见子串是子序列的真子集
//
// 具体步骤：
// 为防止字符串如 abba 的对称中心在两个 b 之间无法表示位置，对字符串先用#拼接
// #a#b#b#a 这样对称中心就是中间的#
// 需要用到的变量
// 1、回文半径数组pArr记录以每个位置的字符为回文中心求出的回文半径长度
//         # a # c # b # b # c # b # d # s #
//  pArr=[ 1 2 1 2 1 2 5 2 1 4 1 2 1 2 1 2 1 ]
//                     ↓
//            (# c # b # b # c #)
// 2、最右回文右边界 R 、最右回文右边界的对称中心 C
//    在求解 pArr[i] 的值更新 R 和 C
//           0 1 2 3 4 5 6 7 8 9 ...
//           # a # b # a # b # c # b # d # s #
// C=-1 R=-1 ↑
//      pArr[0]
//
//           # a # b # a # b # c # b # d # s #
// C=0  R=1    ↑
//        pArr[1]
//
// 分支：
// 1、当前要计算的位置i在R的右边，即 R < i
//    pArr[i] = 1 向两边扩，更新 R 和 C
// 2、i <= R
//    i' 是 i 以 C为对称中心的对称点
//    m 是 i' 回文子串的左边界
//    n 是 C 回文子串的左边界
//    ① n < m
//        n   m                   R
//        ↓   ↓                   ↓
//    # a # b # c # d # d # c # b # e # f #
//              ↑     ↑     ↑
//              i'    C     i
//    此时 pArr[i] = pArr[i'] = C - (i - C) = 2*C - i
//    ② m < n
//    m       n                                           R
//    ↓       ↓                                           ↓
//    # a # b # c # d # c # b # a # f # a # b # c # d # c # k #
//                  ↑               ↑               ↑
//                  i'              C               i
//    此时 pArr[i] = R - i + 1
//    ③ n = m
//        m                           R
//        ↓                           ↓
//    # m # a # b # a # k # a # b # a # t #
//        ↑     ↑       ↑       ↑
//        n     i'      C       i
//    此时 pArr[i]需要继续往外扩，只需从R之后往外扩，pArr[i]的初值为R - i + 1
//
class Manacher
{
 public:
  // 时间复杂度log(N)
  static int manacher(string s)
  {
    if (s.length() == 0)
    {
      return 0;
    }
    // "12132" -> "#1#2#1#3#2#"
    string str = manacherString(s);
    // 回文半径数组
    int *pArr = new int[str.length()]();
    int C = -1;
    // 讲述中：R代表最右的扩成功的位置
    // coding：最右的扩成功位置的，再下一个位置
    int R = -1;
    int max = INT32_MIN;
    for (int i = 0; i < str.length(); i++)
    {
      // 0 1 2
      // R第一个违规的位置，i>= R
      // i位置扩出来的答案，i位置扩的区域，至少是多大。
      pArr[i] = R > i ? std::min(pArr[2 * C - i], R - i) : 1;
      while (i + pArr[i] < str.length() && i - pArr[i] > -1)  // 未超出长度边界
      {
        if (str[i + pArr[i]] == str[i - pArr[i]])  // 如果对称字符相等
        {
          pArr[i]++;  // 当前位置的半径+1
        }
        else
        {
          break;
        }
      }
      if (i + pArr[i] > R)
      {
        // 更新最右回文右边界R，这里没-1，R是第一个违规的位置
        R = i + pArr[i];
        // 更新最右回文右边界的对称中心C
        C = i;
      }
      max = std::max(max, pArr[i]);
    }
    return max - 1;
  }

  // 给字符串str加上#
  // str --> #s#t#r#
  static string manacherString(string str)
  {
    string res;
    res.reserve(str.length() * 2 + 1);
    int index = 0;
    for (int i = 0; i != res.length(); i++)
    {
      res[i] = (i & 1) == 0 ? '#' : str[index++];
    }
    return res;
  }

  // for test
  // 暴力解求最长回文子串半径
  // 时间复杂度log(N^2)
  static int right(string s)
  {
    if (s.length() == 0)
    {
      return 0;
    }
    string str = manacherString(s);
    int max = 0;
    for (int i = 0; i < str.length(); i++)
    {
      int L = i - 1;
      int R = i + 1;
      while (L >= 0 && R < str.length() && str[L] == str[R])
      {
        L--;
        R++;
      }
      max = std::max(max, R - L - 1);
    }
    return max / 2;
  }

  static int getRandom(int min, int max)
  {
    random_device seed;                            // 硬件生成随机数种子
    ranlux48 engine(seed());                       // 利用种子生成随机数引
    uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
    int res = distrib(engine);                     // 随机数
    return res;
  }

  // for test
  static string getRandomString(int possibilities, int size)
  {
    string ans;
    ans.reserve(getRandom(1, size));
    for (int i = 0; i < ans.length(); i++)
    {
      ans[i] = (char) (getRandom(0, possibilities) + 'a');
    }
    return ans;
  }

  static void test()
  {
    int possibilities = 5;
    int strSize = 20;
    int testTimes = 5000000;
    cout << "test begin" << endl;
    for (int i = 0; i < testTimes; i++)
    {
      string str = getRandomString(possibilities, strSize);
      if (manacher(str) != right(str))
      {
        cout << "Oops!" << endl;
      }
    }
    cout << "test finish" << endl;
  }
};

int main()
{
  Manacher::test();
  return 0;
}
