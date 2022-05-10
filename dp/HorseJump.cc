#include <iostream>
#include <memory.h>

using namespace std;

#define N 10
#define M 9

class HorseJump
{
  // 当前来到的位置是（x,y）
  // 还剩下rest步需要跳
  // 跳完rest步，正好跳到a，b的方法数是多少？
  // 10 * 9
  public:

    static int** mallocArray(int row, int column)
    {
      int** arr = (int**)malloc(sizeof(int*) * row);
      for(int i = 0; i < row; i++)
      {
        arr[i] = (int*)malloc(sizeof(int) * column);
        memset(arr[i], 0, sizeof(int) * column);
      }
      return arr;
    }

    static void freeArray(int** arr, int row)
    {
      for(int i = 0; i < row; i++)
      {
        free(arr[i]);
      }
      free(arr);
    }

    static int jump(int a, int b, int k)
    {
      return process(0, 0, k, a, b);
    }

    static int process(int x, int y, int rest, int a, int b)
    {
      if (x < 0 || x >= N || y < 0 || y >= M)
      {
        return 0;
      }
      if (rest == 0)
      {
        return (x == a && y == b) ? 1 : 0;
      }
      int ways = process(x + 2, y + 1, rest - 1, a, b);
      ways += process(x + 1, y + 2, rest - 1, a, b);
      ways += process(x - 1, y + 2, rest - 1, a, b);
      ways += process(x - 2, y + 1, rest - 1, a, b);
      ways += process(x - 2, y - 1, rest - 1, a, b);
      ways += process(x - 1, y - 2, rest - 1, a, b);
      ways += process(x + 1, y - 2, rest - 1, a, b);
      ways += process(x + 2, y - 1, rest - 1, a, b);
      return ways;
    }

    static int dp(int a, int b, int k)
    {
      int*** dp = (int***)malloc(sizeof(int**) * N);
      for (int i = 0; i < N; i++)
      {
        dp[i] = mallocArray(M, k + 1);
      }
      dp[a][b][0] = 1;
      for (int rest = 1; rest <= k; rest++)
      {
        for (int x = 0; x < N; x++)
        {
          for (int y = 0; y < M; y++)
          {
            int ways = pick(dp, x + 2, y + 1, rest - 1);
            ways += pick(dp, x + 1, y + 2, rest - 1);
            ways += pick(dp, x - 1, y + 2, rest - 1);
            ways += pick(dp, x - 2, y + 1, rest - 1);
            ways += pick(dp, x - 2, y - 1, rest - 1);
            ways += pick(dp, x - 1, y - 2, rest - 1);
            ways += pick(dp, x + 1, y - 2, rest - 1);
            ways += pick(dp, x + 2, y - 1, rest - 1);
            dp[x][y][rest] = ways;
          }
        }
      }
      int ans = dp[0][0][k];
      for (int i = 0; i < N; i++)
      {
        freeArray(dp[i], M);
      }
      free(dp);
      return ans;
    }

    static int pick(int*** dp, int x, int y, int rest)
    {
      if (x < 0 || x >= N || y < 0 || y >= M)
      {
        return 0;
      }
      return dp[x][y][rest];
    }

    static int ways(int a, int b, int step)
    {
      return f(0, 0, step, a, b);
    }

    static int f(int i, int j, int step, int a, int b)
    {
      if (i < 0 || i >= N || j < 0 || j >= M)
      {
        return 0;
      }
      if (step == 0)
      {
        return (i == a && j == b) ? 1 : 0;
      }
      return f(i - 2, j + 1, step - 1, a, b) +
              f(i - 1, j + 2, step - 1, a, b) +
              f(i + 1, j + 2, step - 1, a, b) +
              f(i + 2, j + 1, step - 1, a, b) +
              f(i + 2, j - 1, step - 1, a, b) +
              f(i + 1, j - 2, step - 1, a, b) +
              f(i - 1, j - 2, step - 1, a, b) +
              f(i - 2, j - 1, step - 1, a, b);

    }

    static int waysdp(int a, int b, int s)
    {
      int*** dp = (int***)malloc(sizeof(int**) * N);
      for (int i = 0; i < N; i++)
      {
        dp[i] = mallocArray(M, s + 1);
      }
      dp[a][b][0] = 1;
      for (int step = 1; step <= s; step++)
      {
        // 按层来
        for (int i = 0; i < N; i++)
        {
          for (int j = 0; j < M; j++)
          {
            dp[i][j][step] = getValue(dp, i - 2, j + 1, step - 1) + getValue(dp, i - 1, j + 2, step   - 1)
              + getValue(dp, i + 1, j + 2, step - 1) + getValue(dp, i + 2, j + 1, step - 1)
              + getValue(dp, i + 2, j - 1, step - 1) + getValue(dp, i + 1, j - 2, step - 1)
              + getValue(dp, i - 1, j - 2, step - 1) + getValue(dp, i - 2, j - 1, step - 1);
          }
        }
      }
      int ans = dp[0][0][s];
      for (int i = 0; i < N; i++)
      {
        freeArray(dp[i], M);
      }
      free(dp);
      return ans;
    }

    // 在dp表中，得到dp[i][j][step]的值，但如果(i，j)位置越界的话，返回0；
    static int getValue(int*** dp, int i, int j, int step)
    {
      if (i < 0 || i >= N || j < 0 || j >= M)
      {
        return 0;
      }
      return dp[i][j][step];
    }

    static void test()
    {
      int x = 7;
      int y = 7;
      int step = 10;
      cout << ways(x, y, step) << endl;
      cout << waysdp(x, y, step) << endl;
      cout << dp(x, y, step) << endl;
      cout << jump(x, y, step) << endl;
    }
};

int main()
{
  HorseJump::test();
  return 0;
}
