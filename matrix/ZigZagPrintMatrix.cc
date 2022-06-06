#include <iostream>
#include <vector>

using namespace std;

using TdArray = vector<vector<int>>;

class ZigZagPrintMatrix
{
 public:
  static void printMatrixZigZag(TdArray matrix)
  {
    // 自定义两个坐标(tR, tC)和(dR, dC)
    int tR = 0;
    int tC = 0;
    int dR = 0;
    int dC = 0;
    int endR = matrix.size() - 1;
    int endC = matrix[0].size() - 1;
    bool fromUp = false;
    while (tR != endR + 1)
    {
      // 按层打印
      printLevel(matrix, tR, tC, dR, dC, fromUp);
      tR = tC == endC ? tR + 1 : tR;
      tC = tC == endC ? tC : tC + 1;
      dC = dR == endR ? dC + 1 : dC;
      dR = dR == endR ? dR : dR + 1;
      fromUp = !fromUp;
    }
    cout << endl;
  }

  static void printLevel(TdArray m, int tR, int tC, int dR, int dC, bool f)
  {
    if (f)
    {
      // 按↙打印
      while (tR != dR + 1)
      {
        cout << m[tR++][tC--] << " ";
      }
    }
    else
    {
      // 按↗打印
      while (dR != tR - 1)
      {
        cout << m[dR--][dC++] << " ";
      }
    }
  }

  static void printMatrix(TdArray &matrix)
  {
    for (int i = 0; i != matrix.size(); i++)
    {
      for (int j = 0; j != matrix[0].size(); j++)
      {
        cout << matrix[i][j] << " ";
      }
      cout << endl;
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
        matrix[i][j] = index++;
      }
    }
    printMatrix(matrix);
    cout << "============" << endl;
    printMatrixZigZag(matrix);
  }
};

int main()
{
  ZigZagPrintMatrix::test();
  return 0;
}
