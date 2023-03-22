#include <iostream>
#include <vector>

using namespace std;

// 杭州人称那些傻乎乎粘嗒嗒的人为62（音：laoer）。
// 杭州交通管理局经常会扩充一些的士车牌照，新近出来一个好消息，以后上牌照，不再含有不吉利的数字了，这样一来，就可以消除个别的士司机和乘客的心理障碍，更安全地服务大众。
// 不吉利的数字为所有含有4或62的号码。例如：
// 62315 73418 88914
// 都属于不吉利号码。但是，61152虽然含有6和2，但不是62连号，所以不属于不吉利数字之列。
// 你的任务是，对于每次给出的一个牌照区间号[n, m]，推断出交管局今次又要实际上给多少辆新的士车上牌照了。
// 0 <= n <= 1e6
// 0 <= m <= 1e6

// 树形dp

// 设 N 有 n 位数，每一位数从高到低为A(n-1)，A(n-2)，... A(1)，A(0)
// 从最高位 n-1 开始填，
// 填[0, A(n-1) - 1]之间的数可以通过dp计算得到
// 填A(n-1)，就要考虑第 n-2 位该如何填，再分左右子树，直到最后1位，
// 整个计算过程很像树的展开
//                  全集
//                ↙      ↘
//        0 ~ A(n-1)-1   A(n-1)
//                     ↙       ↘
//              0 ~ A(n-2)-1     A(n-2)
//                             ↙       ↘
//                  0 ~ A(n-3)-1        A(n-3)
//                                        ...
//                                        ...
//                                     ↙      ↘
//                              0~A(0)-1      A(0)
class DoNotSixTwo
{
 public:
  static vector<vector<int>> getDp()
  {
    // 区间的值，范围为1e6，7位十进制数，开辟比7大的，我们取8位数空间即可
    int N = 8;
    // 对于每一位数，可以填 0 1 2 ... 9 一共可以填10位不同的数
    int M = 10;
    // dp[i][j]的含义为：
    // 一共有i位数，且最高位i是j的数中，有多少个符合条件的数
    vector<vector<int>> dp(N, vector<int>(M));
    for (int i = 0; i < M; i++)
    {
      if (i != 4)
      {
        // 当只有1位数，且不等于4，有1种车牌号
        dp[1][i] = 1;
      }
    }
    // 2位数及以上的情况
    for (int i = 2; i < N; i++)
    {
      for (int j = 0; j < M; j++)
      {
        // 求 dp[i][j]
        if (j == 4)
        {
          // j这位数是4，不符合条件
          continue;
        }
        for (int k = 0; k < M; k++)
        {
          if (k == 4 || (j == 6 && k == 2))
          {
            // dp[i][j]表示在最高位i填j，dp[i-1][k]表示在次高位i-1填k
            // k这位数是4，或者jk组成了62，不符合条件
            continue;
          }
          // j和k满足条件时，dp[i][j]是可以累加上dp[i-1][k]的方法数的
          dp[i][j] += dp[i - 1][k];
        }
      }
    }
    return dp;
  }

  // 返回[0, n]区间，有多少个合法的车牌号
  static int treeDp(int n, vector<vector<int>> &dp)
  {
    if (n == 0)
    {
      // 0 是合法的车牌号
      return 1;
    }
    vector<int> bits;
    while (n != 0)
    {
      // 保存这个数的每一位
      bits.push_back(n % 10);
      n /= 10;
    }
    int ans = 0;
    int last = 0;
    // 从最高位开始填
    for (int i = bits.size() - 1; i >= 0; i--)
    {
      int x = bits[i];
      // 求树的左分支
      // 第 i 位填 0, 1, 2, ... x-2, x - 1
      for (int j = 0; j < x; j++)
      {
        if (j == 4 || last == 6 && j == 2)
        {
          continue;
        }
        // 0 1 ... i总共有 i + 1 个数，在最高位填j
        ans += dp[i + 1][j];
      }
      // 求树的右分支
      if (x == 4 || (last == 6 && x == 2))
      {
        // 如果右分支不符合条件，即 n 包含了 4 或者 62
        // 这个树可以不用展开计算了
        break;
      }
      // 记录当前右分支填的数，供下次参考
      last = x;
      if (i == 0)
      {
        // 所有分支检查完，说明n也符合条件
        ans++;
      }
    }
    return ans;
  }

  static int doNotSixTwo(int n, int m)
  {
    vector<vector<int>> dp = getDp();
    return treeDp(m, dp) - treeDp(n - 1, dp);
  }

  static void test()
  {
    vector<vector<int>> in = {{3860, 8188}, {4069, 17928}, {8654, 31058}, {7508, 38410}, {253, 1608}, {1340, 23823}};
    vector<int> out = {2318, 8318, 14384, 19661, 920, 14357};
    cout << "test begin" << endl;
    for (int i = 0; i < in.size(); i++)
    {
      if (out[i] != doNotSixTwo(in[i][0], in[i][1]))
      {
        cout << "Opps!" << endl;
      }
    }
    cout << "test end" << endl;
  }
};

int main()
{
  DoNotSixTwo::test();
  return 0;
}
