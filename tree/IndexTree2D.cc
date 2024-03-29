#include <iostream>
#include <vector>

using namespace std;

using TdArray = vector<vector<int>>;

// 测试链接：https://leetcode.com/problems/range-sum-query-2d-mutable
// 但这个题是付费题目
// 提交时把类名、构造函数名从IndexTree2D改成NumMatrix

// 解决问题：查询矩阵区间内的和
class IndexTree2D
{
 private:
  TdArray tree;
  TdArray nums;
  int N;
  int M;

 public:
  IndexTree2D(int **matrix, int n, int m)
  {
    if (m == 0 || n == 0)
    {
      return;
    }
    N = n;
    M = m;

    tree = TdArray(N + 1, vector<int>(M + 1));
    nums = TdArray(N + 1, vector<int>(M + 1));

    for (int i = 0; i < N; i++)
    {
      for (int j = 0; j < M; j++)
      {
        update(i, j, matrix[i][j]);
      }
    }
  }

  int sum(int row, int col)
  {
    int sum = 0;
    for (int i = row + 1; i > 0; i -= i & (-i))
    {
      for (int j = col + 1; j > 0; j -= j & (-j))
      {
        sum += tree[i][j];
      }
    }
    return sum;
  }

  void update(int row, int col, int val)
  {
    if (N == 0 || M == 0)
    {
      return;
    }
    int add = val - nums[row][col];
    nums[row][col] = val;
    for (int i = row + 1; i <= N; i += i & (-i))
    {
      for (int j = col + 1; j <= M; j += j & (-j))
      {
        tree[i][j] += add;
      }
    }
  }

  int sumRegion(int row1, int col1, int row2, int col2)
  {
    if (N == 0 || M == 0)
    {
      return 0;
    }
    return sum(row2, col2) + sum(row1 - 1, col1 - 1) - sum(row1 - 1, col2) - sum(row2, col1 - 1);
  }
};
