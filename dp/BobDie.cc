#include <math.h>
#include <memory.h>
#include <stdbool.h>
#include <iostream>
#include <vector>

using namespace std;

using TdArray = vector<vector<vector<long>>>;

// 题目描述：
//  给定5个参数，N，M，row，col，k
// 表示在N*M的区域上，醉汉Bob初始在(row,col)位置
// Bob一共要迈出k步，且每步都会等概率向上下左右四个方向走一个单位
// 任何时候Bob只要离开N*M的区域，就直接死亡
// 返回k步之后，Bob还在N*M的区域的概率

class BobDie
{
 public:
  static double livePosibility1(int row, int col, int k, int N, int M)
  {
    return (double) process(row, col, k, N, M) / std::pow(4, k);  // 每一步有4个方向，k步就有4^k个方案
  }

  // 目前在row，col位置，还有rest步要走，走完了如果还在棋盘中就获得1个生存点，返回总的生存点数
  static long process(int row, int col, int rest, int N, int M)
  {
    if (row < 0 || row == N || col < 0 || col == M)
    {
      return 0;
    }
    // 还在棋盘中！
    if (rest == 0)
    {
      return 1;
    }
    // 还在棋盘中！还有步数要走
    long up = process(row - 1, col, rest - 1, N, M);
    long down = process(row + 1, col, rest - 1, N, M);
    long left = process(row, col - 1, rest - 1, N, M);
    long right = process(row, col + 1, rest - 1, N, M);
    return up + down + left + right;
  }

  static double livePosibility2(int row, int col, int k, int N, int M)
  {
    TdArray dp(N, vector<vector<long>>(M, vector<long>(k + 1)));
    for (int i = 0; i < N; i++)
    {
      for (int j = 0; j < M; j++)
      {
        // base case
        dp[i][j][0] = 1;
      }
    }
    for (int rest = 1; rest <= k; rest++)
    {
      for (int r = 0; r < N; r++)
      {
        for (int c = 0; c < M; c++)
        {
          dp[r][c][rest] = pick(dp, N, M, r - 1, c, rest - 1);
          dp[r][c][rest] += pick(dp, N, M, r + 1, c, rest - 1);
          dp[r][c][rest] += pick(dp, N, M, r, c - 1, rest - 1);
          dp[r][c][rest] += pick(dp, N, M, r, c + 1, rest - 1);
        }
      }
    }
    return (double) dp[row][col][k] / std::pow(4, k);
  }

  static long pick(TdArray &dp, int N, int M, int r, int c, int rest)
  {
    if (r < 0 || r == N || c < 0 || c == M)
    {
      return 0;
    }
    return dp[r][c][rest];
  }

  static void test()
  {
    cout << livePosibility1(6, 6, 10, 50, 50) << endl;
    cout << livePosibility2(6, 6, 10, 50, 50) << endl;
  }
};

int main()
{
  BobDie::test();
  return 0;
}
