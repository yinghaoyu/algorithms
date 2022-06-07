#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

using namespace std;
using TdArray = vector<vector<int>>;

// leetcode：https://leetcode.com/problems/super-egg-drop

// 方法1和方法2会超时
// 方法3勉强通过
// 方法4打败100%
// 方法5打败100%，方法5是在方法4的基础上做了进一步的常数优化
class ThrowChessPiecesProblem
{
 public:
  static int superEggDrop1(int kChess, int nLevel)
  {
    if (nLevel < 1 || kChess < 1)
    {
      return 0;
    }
    return Process1(nLevel, kChess);
  }

  // rest还剩多少层楼需要去验证
  // k还有多少颗棋子能够使用
  // 一定要验证出最高的不会碎的楼层！但是每次都是坏运气。
  // 返回至少需要扔几次？
  static int Process1(int rest, int k)
  {
    if (rest == 0)
    {
      // 没有楼层需要验证，不需要扔棋子了
      return 0;
    }
    if (k == 1)
    {
      // 只剩一颗棋子，不知道至少要几次，由于取最小值，这里返回至多需要扔rest次
      return rest;
    }
    int min = INT32_MAX;
    for (int i = 1; i != rest + 1; i++)
    {
      // 第一次扔的时候，仍在了i层
      // 棋子碎了，此时 i+1 ~ rest层都不需要验证，只需要验证0 ~ i-1层，剩余棋子k-1
      // 棋子没碎，此时 1 ~ i-1层都不需要验证，只需验证rest - i ~ rest层，剩余棋子k
      min = std::min(min, std::max(Process1(i - 1, k - 1), Process1(rest - i, k)));
    }
    return min + 1;  // 子过程扔的次数 + 当前次数
  }

  static int superEggDrop2(int kChess, int nLevel)
  {
    if (nLevel < 1 || kChess < 1)
    {
      return 0;
    }
    if (kChess == 1)
    {
      return nLevel;
    }
    TdArray dp(nLevel + 1, vector<int>(kChess + 1));
    for (int i = 1; i != dp.size(); i++)
    {
      // base case
      dp[i][1] = i;
    }
    for (int i = 1; i != dp.size(); i++)
    {
      for (int j = 2; j != dp[0].size(); j++)
      {
        int min = INT32_MAX;
        for (int k = 1; k != i + 1; k++)
        {
          min = std::min(min, std::max(dp[k - 1][j - 1], dp[i - k][j]));
        }
        dp[i][j] = min + 1;
      }
    }
    return dp[nLevel][kChess];
  }

  // 四边形不等式优化
  static int superEggDrop3(int kChess, int nLevel)
  {
    if (nLevel < 1 || kChess < 1)
    {
      return 0;
    }
    if (kChess == 1)
    {
      return nLevel;
    }
    TdArray dp(nLevel + 1, vector<int>(kChess + 1));
    for (int i = 1; i != dp.size(); i++)
    {
      dp[i][1] = i;
    }
    TdArray best(nLevel + 1, vector<int>(kChess + 1));
    for (int i = 1; i != dp[0].size(); i++)
    {
      dp[1][i] = 1;
      best[1][i] = 1;
    }
    for (int i = 2; i < nLevel + 1; i++)
    {
      for (int j = kChess; j > 1; j--)
      {
        int ans = INT32_MAX;
        int bestChoose = -1;
        int down = best[i - 1][j];
        int up = j == kChess ? i : best[i][j + 1];
        for (int first = down; first <= up; first++)
        {
          int cur = std::max(dp[first - 1][j - 1], dp[i - first][j]);
          if (cur <= ans)
          {
            ans = cur;
            bestChoose = first;
          }
        }
        dp[i][j] = ans + 1;
        best[i][j] = bestChoose;
      }
    }
    return dp[nLevel][kChess];
  }

  static int superEggDrop4(int kChess, int nLevel)
  {
    if (nLevel < 1 || kChess < 1)
    {
      return 0;
    }
    int *dp = new int[kChess]();
    int res = 0;
    while (true)
    {
      res++;
      int previous = 0;
      for (int i = 0; i < kChess; i++)
      {
        int tmp = dp[i];
        dp[i] = dp[i] + previous + 1;
        previous = tmp;
        if (dp[i] >= nLevel)
        {
          return res;
        }
      }
    }
  }

  static int superEggDrop5(int kChess, int nLevel)
  {
    if (nLevel < 1 || kChess < 1)
    {
      return 0;
    }
    int bsTimes = log2N(nLevel) + 1;
    if (kChess >= bsTimes)
    {
      return bsTimes;
    }
    int *dp = new int[kChess]();
    int res = 0;
    while (true)
    {
      res++;
      int previous = 0;
      for (int i = 0; i < kChess; i++)
      {
        int tmp = dp[i];
        dp[i] = dp[i] + previous + 1;
        previous = tmp;
        if (dp[i] >= nLevel)
        {
          return res;
        }
      }
    }
  }

  static int log2N(int n)
  {
    int res = -1;
    while (n != 0)
    {
      res++;
      n = static_cast<unsigned int>(n) >> 1;
    }
    return res;
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
    int maxN = 500;
    int maxK = 30;
    int testTime = 1000;
    cout << "测试开始" << endl;
    for (int i = 0; i < testTime; i++)
    {
      int N = getRandom(1, maxN);
      int K = getRandom(1, maxK);
      int ans2 = superEggDrop2(K, N);
      int ans3 = superEggDrop3(K, N);
      int ans4 = superEggDrop4(K, N);
      int ans5 = superEggDrop5(K, N);
      if (ans2 != ans3 || ans4 != ans5 || ans2 != ans4)
      {
        cout << "出错了!" << endl;
      }
    }
    cout << "测试结束" << endl;
  }
};

int main()
{
  ThrowChessPiecesProblem::test();
  return 0;
}
