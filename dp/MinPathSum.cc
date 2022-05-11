#include <iostream>
#include <random>
#include <stdbool.h>
#include <memory.h>

using namespace std;

// 测试链接：https://leetcode.cn/problems/minimum-path-sum/

// 最小路径和
// 给定一个包含非负整数的 m * n 网格 grid ，请找出一条从左上角到右下角的路径，使得路径上的数字总和为最小。
// 说明：每次只能向下或者向右移动一步。

// 1 3 1
// 1 5 1
// 4 2 1

// 输出7，因为路径 1→3→1→1→1 的总和最小

class MinPathSum
{
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

    static int getRandom(int min, int max)
    {
      random_device seed;  // 硬件生成随机数种子
      ranlux48 engine(seed());  // 利用种子生成随机数引
      uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
      int res = distrib(engine);  // 随机数
      return res;
    }

    // dp[i][j] 表示从 (0, 0) 走到 (i,j) 时的最小路径和
    static int minPathSum1(int** m, int N, int M)
    {
      if (m == nullptr || N == 0 || m[0] == nullptr || M == 0)
      {
        return 0;
      }
      int row = N;
      int col = M;
      int** dp = mallocArray(row, col);
      // base case
      dp[0][0] = m[0][0]; // 起始时，当前节点就是最小值
      for (int i = 1; i < row; i++)
      {
        // 第一行只能从左往右走
        dp[i][0] = dp[i - 1][0] + m[i][0];
      }
      for (int j = 1; j < col; j++)
      {
        // 第一列只能从上往下走
        dp[0][j] = dp[0][j - 1] + m[0][j];
      }
      for (int i = 1; i < row; i++)
      {
        for (int j = 1; j < col; j++)
        {
          // 依赖左边和上边的值
          dp[i][j] = std::min(dp[i - 1][j], dp[i][j - 1]) + m[i][j];
        }
      }
      int ans = dp[row - 1][col - 1];
      freeArray(dp, N);
      return ans;
    }

    // 如果不要求打印路径，可以进行状态压缩，空间优化
    // 当走到第i行的时候，最短路径和只与第i-1行和第i行有关(以列考虑结果一样)
    // 当我们计算走到第i行的最短路径时，dp[i]目前存的其实还是上一行的最短路径
    static int minPathSum2(int** m, int N, int M)
    {
      if (m == nullptr || N == 0 || m[0] == nullptr || M == 0)
      {
        return 0;
      }
      int row = N;
      int col = M;
      int* dp = (int*)malloc(sizeof(int) * col);
      dp[0] = m[0][0];
      for (int j = 1; j < col; j++)
      {
        // 第 0 列
        // 从 (0, 0) 到 (0, j) 需要的最短路径
        dp[j] = dp[j - 1] + m[0][j];
      }
      for (int i = 1; i < row; i++)
      {
        // 第 1 列开始
        dp[0] += m[i][0];  // 从 (1, 0) 到 (i, 0) 需要的最短路径
        for (int j = 1; j < col; j++)
        {
          // 取上一行dp[j-1]或者上一列dp[j]的最小值
          dp[j] = std::min(dp[j - 1], dp[j]) + m[i][j];
        }
      }
      int ans = dp[col - 1];
      free(dp);
      return ans;
    }

    // for test
    static int** generateRandomMatrix(int rowSize, int colSize)
    {
      if (rowSize < 0 || colSize < 0)
      {
        return nullptr;
      }
      int** result = mallocArray(rowSize, colSize);
      for (int i = 0; i != rowSize; i++)
      {
        for (int j = 0; j != colSize; j++)
        {
          result[i][j] = getRandom(0, 100);
        }
      }
      return result;
    }

    // for test
    static void printMatrix(int** matrix, int N, int M)
    {
      for (int i = 0; i != N; i++)
      {
        for (int j = 0; j != M; j++)
        {
          cout << matrix[i][j] << " ";
        }
        cout << endl;
      }
    }

    static void test()
    {
      int rowSize = 10;
      int colSize = 10;
      int** m = generateRandomMatrix(rowSize, colSize);
      cout << minPathSum1(m, rowSize, colSize) << endl;
      cout << minPathSum2(m, rowSize, colSize) << endl;
    }
};

int main()
{
  MinPathSum::test();
  return 0;
}
