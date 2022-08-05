#include <iostream>
#include <vector>

using namespace std;
using TdArray = vector<vector<int>>;

// 问题描述：
// 给定一个 m x n 的矩阵，如果一个元素为 0 ，则将其所在行和列的所有元素都设为 0 。请使用 原地 算法。

// 思路：
// 如果不考虑限制
// vector<bool> row(m)
// vector<bool> col(n)
// 可以用两个数组标记哪行、哪列需要清0
//
// 考虑限制，就可以在原二维数组上取第0行、第0列代替row、col数组
// 需要注意的是，第0行、第0列本身存在的0需要提前考虑

class SetMatrixZeroes
{
 public:
  static void setZeroes1(TdArray &matrix)
  {
    bool row0Zero = false;
    bool col0Zero = false;
    int i = 0;
    int j = 0;
    for (i = 0; i < matrix[0].size(); i++)
    {
      // 第0行有0，后面这一行都要清0
      if (matrix[0][i] == 0)
      {
        row0Zero = true;
        break;
      }
    }
    for (i = 0; i < matrix.size(); i++)
    {
      // 第0列有0，后面这一列都要清0
      if (matrix[i][0] == 0)
      {
        col0Zero = true;
        break;
      }
    }
    for (i = 1; i < matrix.size(); i++)
    {
      for (j = 1; j < matrix[0].size(); j++)
      {
        if (matrix[i][j] == 0)
        {
          // 记录需要清0的行和列
          matrix[i][0] = 0;
          matrix[0][j] = 0;
        }
      }
    }
    for (i = 1; i < matrix.size(); i++)
    {
      for (j = 1; j < matrix[0].size(); j++)
      {
        // 根据记录把需要的位置清0
        if (matrix[i][0] == 0 || matrix[0][j] == 0)
        {
          matrix[i][j] = 0;
        }
      }
    }
    if (row0Zero)
    {
      // 第0行存在0，就清0
      for (i = 0; i < matrix[0].size(); i++)
      {
        matrix[0][i] = 0;
      }
    }
    if (col0Zero)
    {
      // 第0列存在0，就清0
      for (i = 0; i < matrix.size(); i++)
      {
        matrix[i][0] = 0;
      }
    }
  }

  // 优化
  static void setZeroes2(TdArray &matrix)
  {
    // 第0列的数，表示行要不要清0
    bool col0 = false;
    int i = 0;
    int j = 0;
    for (i = 0; i < matrix.size(); i++)
    {
      for (j = 0; j < matrix[0].size(); j++)
      {
        if (matrix[i][j] == 0)
        {
          matrix[i][0] = 0;
          if (j == 0)
          {
            // 因为第0列的数表示的是行要不要清0，第0列要不要清0用col0记录
            // 例如matrix[1][0]的含义是第1行要不要清0，因此不能改这个值
            col0 = true;
          }
          else
          {
            matrix[0][j] = 0;
          }
        }
      }
    }
    for (i = matrix.size() - 1; i >= 0; i--)
    {
      for (j = 1; j < matrix[0].size(); j++)
      {
        if (matrix[i][0] == 0 || matrix[0][j] == 0)
        {
          matrix[i][j] = 0;
        }
      }
    }
    if (col0)
    {
      for (i = 0; i < matrix.size(); i++)
      {
        matrix[i][0] = 0;
      }
    }
  }
};
