#include <iostream>

using namespace std;

//对于N个格子（从1~N标号），机器人最开始在Start（1<=Start<=N）位置，要求在走过K（K>=1）步后（一次一格），来到aim位置（1<=aim<=N），问机器人有多少种走法？

//（注：在两端的格子只能往中间走，在中间的任意一个格子，可以选择左走或右走）

class RobotWalk
{
  public:
    static int ways1(int N, int start, int aim, int K)
    {
      if (N < 2 || start < 1 || start > N || aim < 1 || aim > N || K < 1)
      {
        return -1;
      }
      return process1(start, K, aim, N);
    }

    // 机器人当前来到的位置是cur，
    // 机器人还有rest步需要去走，
    // 最终的目标是aim，
    // 有哪些位置？1~N
    // 返回：机器人从cur出发，走过rest步之后，最终停在aim的方法数，是多少？
    static int process1(int cur, int rest, int aim, int N)
    {
      if (rest == 0)
      {
        // 如果已经不需要走了，走完了！
        return cur == aim ? 1 : 0;
      }
      // (cur, rest)
      if (cur == 1)
      {
        // 1 -> 2
        return process1(2, rest - 1, aim, N);
      }
      // (cur, rest)
      if (cur == N)
      {
        // N-1 <- N
        return process1(N - 1, rest - 1, aim, N);
      }
      // (cur, rest)
      return process1(cur - 1, rest - 1, aim, N) + process1(cur + 1, rest - 1, aim, N);
    }

    static int ways2(int N, int start, int aim, int K)
    {
      if (N < 2 || start < 1 || start > N || aim < 1 || aim > N || K < 1)
      {
        return -1;
      }
      int** dp = (int**)malloc(sizeof(int*) * (N + 1));
      for(int i = 0; i < N + 1; i++)
      {
        dp[i] = (int*)malloc(sizeof(int) * (K + 1));
      }
      for (int i = 0; i <= N; i++)
      {
        for (int j = 0; j <= K; j++)
        {
          dp[i][j] = -1;
        }
      }
      // dp就是缓存表
      // dp[cur][rest] == -1 -> process1(cur, rest)之前没算过！
      // dp[cur][rest] != -1 -> process1(cur, rest)之前算过！返回值，dp[cur][rest]
      // N+1 * K+1
      return process2(start, K, aim, N, (int**)dp);
    }

    // cur 范: 1 ~ N
    // rest 范：0 ~ K
    static int process2(int cur, int rest, int aim, int N, int** dp)
    {
      if (dp[cur][rest] != -1)  // 傻缓存优化
      {
        return dp[cur][rest];
      }
      // 之前没算过！
      int ans = 0;
      if (rest == 0)
      {
        ans = cur == aim ? 1 : 0;
      }
      else if (cur == 1)
      {
        ans = process2(2, rest - 1, aim, N, dp);
      }
      else if (cur == N)
      {
        ans = process2(N - 1, rest - 1, aim, N, dp);
      }
      else
      {
        ans = process2(cur - 1, rest - 1, aim, N, dp) + process2(cur + 1, rest - 1, aim, N, dp);
      }
      dp[cur][rest] = ans;
      return ans;
    }

    // N = 5，start = 2，aim = 4，rest = 6
    // 两个可变参数，取cur为纵坐标，rest为横坐标
    //    0  1  2  3  4  5  6
    // 0  ×  ×  ×  ×  ×  ×  ×  这一行用不到
    //
    // 1  0  ↙  ↙  ↙  ↙  ↙  ↙  这一行只能往左下走
    //
    // 2  0  ↖                 中间的行可以往左下、左上走
    //       ↙
    // 3  0
    //
    // 4  1
    //
    // 5  0  ↖  ↖  ↖  ↖  ↖  ↖  这一行只能往左上走
    // 根据这个规律可以从上往下、从左往右填dp表。其他类似dp表还可以进行斜率优化
    static int ways3(int N, int start, int aim, int K)
    {
      if (N < 2 || start < 1 || start > N || aim < 1 || aim > N || K < 1)
      {
        return -1;
      }
      int** dp = (int**)malloc(sizeof(int*) * (N + 1));
      for(int i = 0; i < N + 1; i++)
      {
        dp[i] = (int*)malloc(sizeof(int) * (K + 1));
      }
      dp[aim][0] = 1;  // 只有到达aim且rest为0，是有效的
      for (int rest = 1; rest <= K; rest++)
      {
        dp[1][rest] = dp[2][rest - 1];  // 第一行只能往左下走
        for (int cur = 2; cur < N; cur++)
        {
          // 中间可以往左上、左下走
          dp[cur][rest] = dp[cur - 1][rest - 1] + dp[cur + 1][rest - 1];
        }
        dp[N][rest] = dp[N - 1][rest - 1];  // 最后一行只能往左上走
      }
      return dp[start][K];
    }
};

int main()
{
  cout << RobotWalk::ways1(5, 2, 4, 6) << endl;
  cout << RobotWalk::ways2(5, 2, 4, 6) << endl;
  cout << RobotWalk::ways3(5, 2, 4, 6) << endl;
  return 0;
}

