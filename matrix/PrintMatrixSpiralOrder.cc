#include <iostream>
#include <vector>

using namespace std;
using TdArray = vector<vector<int>>;

// 按螺旋序打印矩阵
// 1  2  3  4
// 5  6  7  8
// 9  10 11 12
// 13 14 15 16
//
// 输出：1 2 3 4 8 12 16 15 14 13 9 5 6 7 11 10

class PrintMatrixSpiralOrder
{
 public:
  static void spiralOrderPrint(TdArray matrix)
  {
    int tR = 0;
    int tC = 0;
    int dR = matrix.size() - 1;
    int dC = matrix[0].size() - 1;
    while (tR <= dR && tC <= dC)
    {
      printEdge(matrix, tR++, tC++, dR--, dC--);
    }
  }

  static void printEdge(TdArray m, int tR, int tC, int dR, int dC)
  {
    if (tR == dR)
    {
      // →
      for (int i = tC; i <= dC; i++)
      {
        cout << m[tR][i] << " ";
      }
    }
    else if (tC == dC)
    {
      // ↓
      for (int i = tR; i <= dR; i++)
      {
        cout << m[i][tC] << " ";
      }
    }
    else
    {
      int curC = tC;
      int curR = tR;
      // →
      while (curC != dC)
      {
        cout << m[tR][curC] << " ";
        curC++;
      }
      // ↓
      while (curR != dR)
      {
        cout << m[curR][dC] << " ";
        curR++;
      }
      // ←
      while (curC != tC)
      {
        cout << m[dR][curC] << " ";
        curC--;
      }
      // ↑
      while (curR != tR)
      {
        cout << m[curR][tC] << " ";
        curR--;
      }
    }
  }

  static void test()
  {
    int row = 4;
    int column = 4;
    int index = 0;
    TdArray matrix(row, vector<int>(column));
    for (int i = 0; i < row; i++)
    {
      for (int j = 0; j < column; j++)
      {
        matrix[i][j] = ++index;
      }
    }
    spiralOrderPrint(matrix);
  }
};

int main()
{
  PrintMatrixSpiralOrder::test();
  return 0;
}
