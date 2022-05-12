#include <memory.h>
#include <random>
#include <stdbool.h>
#include <iostream>
#include <math.h>

using namespace std;

// 问题描述：
// 怪兽有hp点血，每次伤害在[0, M]范围上，可以砍times，返回砍死的概率。

class KillMonster
{
  public:
    static double right(int N, int M, int K)
    {
      if (N < 1 || M < 1 || K < 1)
      {
        return 0;
      }
      long all = (long) std::pow(M + 1, K);  // 每次有M+1种方案，K次就有(M+1)^K种方案
      long kill = process(K, M, N);
      return (double) ((double) kill / (double) all);
    }

    // 怪兽还剩hp点血
    // 每次的伤害在[0~M]范围上
    // 还有times次可以砍
    // 返回砍死的情况数！
    static long process(int times, int M, int hp)
    {
      if (times == 0)
      {
        // base case
        return hp <= 0 ? 1 : 0;
      }
      if (hp <= 0)
      {
        // 直接剪枝，减少递归次数
        return (long) std::pow(M + 1, times);  // 血量少于0，后面怎么算都有效
      }
      long ways = 0;
      for (int i = 0; i <= M; i++)
      {
        ways += process(times - 1, M, hp - i);
      }
      return ways;
    }

    static double dp1(int N, int M, int K)
    {
      if (N < 1 || M < 1 || K < 1)
      {
        return 0;
      }
      long all = (long) std::pow(M + 1, K);
      long** dp = mallocArray<long>(K + 1, N + 1);
      dp[0][0] = 1;
      for (int times = 1; times <= K; times++)
      {
        dp[times][0] = (long) std::pow(M + 1, times);
        for (int hp = 1; hp <= N; hp++)
        {
          long ways = 0;
          for (int i = 0; i <= M; i++)
          {
            if (hp - i >= 0)
            {
              ways += dp[times - 1][hp - i];
            }
            else
            {
              ways += (long) std::pow(M + 1, times - 1);
            }
          }
          dp[times][hp] = ways;
        }
      }
      long kill = dp[K][N];
      freeArray<long>(dp, K + 1);
      return (double) ((double) kill / (double) all);
    }

    static double dp2(int N, int M, int K)
    {
      if (N < 1 || M < 1 || K < 1)
      {
        return 0;
      }
      long all = (long) std::pow(M + 1, K);
      long** dp = mallocArray<long>(K + 1, N + 1);
      dp[0][0] = 1;
      for (int times = 1; times <= K; times++)
      {
        dp[times][0] = (long) std::pow(M + 1, times);
        for (int hp = 1; hp <= N; hp++)
        {
          dp[times][hp] = dp[times][hp - 1] + dp[times - 1][hp];
          // 斜率优化
          if (hp - 1 - M >= 0)
          {
            dp[times][hp] -= dp[times - 1][hp - 1 - M];
          }
          else
          {
            dp[times][hp] -= std::pow(M + 1, times - 1);
          }
        }
      }
      long kill = dp[K][N];
      freeArray<long>(dp, K + 1);
      return (double) ((double) kill / (double) all);
    }

    // for test
    template<typename T>
      static T** mallocArray(int row, int column)
      {
        T** arr = (T**)malloc(sizeof(T*) * row);
        for(int i = 0; i < row; i++)
        {
          arr[i] = (T*)malloc(sizeof(T) * column);
          memset(arr[i], 0, sizeof(T) * column);
        }
        return arr;
      }

    template<typename T>
      static void freeArray(T** arr, int row)
      {
        for(int i = 0; i < row; i++)
        {
          free(arr[i]);
        }
        free(arr);
      }

    static int getRandom(int min, int max)
    {
      random_device seed;  // 硬件生成随机数种子
      ranlux48 engine(seed());  // 利用种子生成随机数引
      uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
      int res = distrib(engine);  // 随机数
      return res;
    }

    static void test()
    {
      int NMax = 10;
      int MMax = 10;
      int KMax = 10;
      int testTime = 200;
      cout << "测试开始" << endl;
      for (int i = 0; i < testTime; i++)
      {
        int N = getRandom(0, NMax);
        int M = getRandom(0, MMax);
        int K = getRandom(0, KMax);
        double ans1 = right(N, M, K);
        double ans2 = dp1(N, M, K);
        double ans3 = dp2(N, M, K);
        if (ans1 != ans2 || ans1 != ans3)
        {
          cout << "Oops!" << endl;
          break;
        }
      }
      cout << "测试结束" << endl;
    }
};

int main()
{
  KillMonster::test();
  return 0;
}
