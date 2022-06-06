#include <iostream>
#include <vector>

using namespace std;

using TdArray = vector<vector<int>>;

// 矩阵顺时针旋转90°，要求额外空间复杂度为O(1)
// 思路：
// 先用一个tmp保存一下1这个值
// 然后把13放到1的位置
// 再把16放到13的位置
// 然后4放到16的位置
// 最后再把tmp放到4的位置。
// 依次类推，保存2，9放到2，15放到9，8放到15，2放到8…最外圈操作完了再操作里面一圈。
//
// (1)  2   3  (4)
//  5   6   7   8
//  9   10  11  12
// (13) 14  15 (16)

class RotateMatrix
{
 public:
  static void rotate(TdArray &matrix)
  {
    // 自定义两个坐标(a, b)和(c, d)
    int a = 0;
    int b = 0;
    int c = matrix.size() - 1;
    int d = matrix[0].size() - 1;
    while (a < c)
    {
      rotateEdge(matrix, a++, b++, c--, d--);
      // cout << "--------" << endl;
      // printMatrix(matrix);
    }
  }

  static void rotateEdge(TdArray &m, int a, int b, int c, int d)
  {
    int tmp = 0;
    // 两点横坐标相减，就能得出这一圈需要交换几次
    for (int i = 0; i < d - b; i++)
    {
      // 以最外一圈第一次交换为例，一次需要交换4个数
      tmp = m[a][b + i];          // 这里保存1
      m[a][b + i] = m[c - i][b];  // 将13放到1位置
      m[c - i][b] = m[c][d - i];  // 将16放到13位置
      m[c][d - i] = m[a + i][d];  // 将4放到16位置
      m[a + i][d] = tmp;          // 将temp的1放到4位置
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
        matrix[i][j] = ++index;
      }
    }
    printMatrix(matrix);
    rotate(matrix);
    cout << "=========" << endl;
    printMatrix(matrix);
  }
};

int main()
{
  RotateMatrix::test();
  return 0;
}
